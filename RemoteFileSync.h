#pragma once

#include <string>
#include <vector>

class RemoteFileSync
{
public:
	virtual void uploadFile(std::string localPath, std::string remotePath) = 0; // push
	virtual void downloadFile(std::string remotePath, std::string localPath) = 0; // pull
	virtual void deleteFile(std::string remotePath) = 0; // rm
	virtual void renameFile(std::string remotePath, std::string newName) = 0; // mv

	virtual std::vector<std::string> fetchFileList(std::string remotePath) = 0; // fetch

	// note: feasibility still ??
	virtual std::vector<std::string> fetchModificationList(std::string remotePath) = 0;

	virtual ~RemoteFileSync() = 0;
};