#pragma once

#include <string>
#include <vector>

class RemoteFileSync
{
public:
	virtual void uploadFile(const std::wstring& localPath, const std::wstring& remotePath) = 0; // push
	virtual void downloadFile(const std::wstring& remotePath, const std::wstring& localPath) = 0; // pull
	virtual void deleteFile(const std::wstring& remotePath) = 0; // rm
	virtual void renameFile(const std::wstring& remotePath, const std::wstring& newName) = 0; // mv

	virtual std::vector<std::wstring> fetchFileList(const std::wstring& remotePath) = 0; // fetch

	// note: feasibility still ??
	virtual std::vector<std::wstring> fetchModificationList(const std::wstring& remotePath) = 0;

	virtual ~RemoteFileSync() = default;
};