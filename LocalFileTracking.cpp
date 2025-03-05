#include "LocalFileTracking.h"

#include <iostream>
#include <filesystem>
#include <exception>
#include <future>

static bool directoryExists(const std::wstring& path)
{
	return std::filesystem::exists(path) && std::filesystem::is_directory(path);
}

LocalFileTracking::LocalFileTracking(const std::wstring& folderPath) : m_folderPath(folderPath)
{
	if (!directoryExists(folderPath))
	{
		throw std::invalid_argument("Folder path does not exist");
	}

	hDir = CreateFileW(
		folderPath.c_str(),
		FILE_LIST_DIRECTORY,
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
		NULL
	);

	if (hDir == INVALID_HANDLE_VALUE) {
		DWORD errorCode = GetLastError();
		throw std::runtime_error("Failed to open directory. Error Code: " + std::to_string(errorCode));
	}

	fileNotificationFuture = std::async([this]() {
		while (!stopRequested.load())
		{
			if (!ReadDirectoryChangesW(
				hDir,
				buffer, sizeof(buffer),
				TRUE,
				FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME | FILE_NOTIFY_CHANGE_LAST_WRITE,
				&bytesReturned,
				NULL, NULL
			)) {
				DWORD errorCode = GetLastError();
				std::cerr << "Failed to read directory changes. Error Code: " << errorCode << std::endl;
				continue;
			}
			FILE_NOTIFY_INFORMATION* fileNotifyInfo = (FILE_NOTIFY_INFORMATION*)buffer;

			std::wstring oldPath;

			while (true) {
				std::wstring fileName(fileNotifyInfo->FileName, fileNotifyInfo->FileName + (fileNotifyInfo->FileNameLength / sizeof(WCHAR)));

				switch (fileNotifyInfo->Action) {
				case FILE_ACTION_ADDED:
					callbackNew(fileName);
					break;
				case FILE_ACTION_REMOVED:
					callbackDel(fileName);
					break;
				case FILE_ACTION_MODIFIED:
					callbackMod(fileName);
					break;
				case FILE_ACTION_RENAMED_OLD_NAME:
					oldPath = fileName;
					break;
				case FILE_ACTION_RENAMED_NEW_NAME:
					if (!oldPath.empty())
					{
						callbackMod(oldPath, fileName);
						oldPath.clear();
					}
					break;
				}

				if (fileNotifyInfo->NextEntryOffset == 0)
					break;

				fileNotifyInfo = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(
					reinterpret_cast<BYTE*>(fileNotifyInfo) + fileNotifyInfo->NextEntryOffset
					);
			}
		}
		});
}

LocalFileTracking::~LocalFileTracking()
{
	stopRequested.store(true);
	CloseHandle(hDir);
}

void LocalFileTracking::onFileCreate(FileCallback listCreateCallback)
{
	listCreateCallbacks.push_back(listCreateCallback);
}

void LocalFileTracking::onFileDelete(FileCallback listDeleteCallback)
{
	listDeleteCallbacks.push_back(listDeleteCallback);
}

void LocalFileTracking::onFileModification(FileCallback listModificationCallback)
{
	listModificationCallbacks.push_back(listModificationCallback);
}

void LocalFileTracking::callbackNew(std::wstring path)
{
	for (auto& callback : listCreateCallbacks)
	{
		callback(std::wstring(L"New file/folder created: ") += path);
	}
}

void LocalFileTracking::callbackDel(std::wstring path)
{
	for (auto& callback : listDeleteCallbacks)
	{
		callback(std::wstring(L"File/folder deleted: ") += path);
	}
}

void LocalFileTracking::callbackMod(std::wstring path, std::optional<std::wstring> newPath)
{
	for (auto& callback : listModificationCallbacks)
	{
		if (newPath.has_value())
		{
			callback(std::wstring(L"File/folder renamed: ").append(path).append(L" to ").append(newPath.value()));
			continue;
		}
		callback(std::wstring(L"File/folder modified: ") += path);
	}
}

