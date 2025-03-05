#pragma once

#include <string>
#include <functional>
#include <vector>
#include <future>
#include <optional>

#include <windows.h>

class LocalFileTracking
{
public:
	using FileCallback = std::function<void(std::wstring path)>;

	LocalFileTracking() = delete;

	LocalFileTracking(const std::wstring& folderPath);
	~LocalFileTracking();
	void onFileCreate(FileCallback listCreateCallback);
	void onFileDelete(FileCallback listDeleteCallback);
	void onFileModification(FileCallback listModificationCallback);

private:
	const std::wstring m_folderPath;
	std::vector<FileCallback> listCreateCallbacks;
	std::vector<FileCallback> listDeleteCallbacks;
	std::vector<FileCallback> listModificationCallbacks;

	HANDLE hDir;
	std::future<void> fileNotificationFuture;
	char buffer[2048] = {};
	DWORD bytesReturned = 0; // todo: check bytesReturned every callback to check error
	std::atomic<bool> stopRequested{ false }; // to stop the fileNotificationFuture

	void callbackNew(std::wstring);
	void callbackDel(std::wstring);
	void callbackMod(std::wstring, std::optional<std::wstring> = {});
};