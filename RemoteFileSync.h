#pragma once

class RemoteFileSync
{
public:
	virtual void uploadFile(const char* localPath, const char* remotePath) = 0; // push
	virtual void downloadFile(const char* remotePath, const char* localPath) = 0; // pull
	virtual void deleteFile(const char* remotePath) = 0; // rm

	virtual void fetchFileList(const char* remotePath, char* buffer, int bufferSize) = 0; // fetch

    // note: feasibility still ??
	virtual void fetchModificationList(const char* remotePath, char* buffer, int bufferSize) = 0;

	virtual ~RemoteFileSync() {}
};