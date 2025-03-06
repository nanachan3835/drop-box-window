#pragma once
#include "RemoteFileSync.h"

#include "PocoNet.h"

class R2FileSync :
    public RemoteFileSync
{
public:
	R2FileSync(const std::wstring& remoteHost);
	~R2FileSync() override;

	void uploadFile(const std::wstring& localPath, const std::wstring& remotePath) override;
	void downloadFile(const std::wstring& remotePath, const std::wstring& localPath) override; 
	void deleteFile(const std::wstring& remotePath) override;
	void renameFile(const std::wstring& remotePath, const std::wstring& newName) override;

	virtual std::vector<std::wstring> fetchFileList(const std::wstring& remotePath) override;

	// note: feasibility still ??
	virtual std::vector<std::wstring> fetchModificationList(const std::wstring& remotePath) override;
private:
	std::wstring remoteHost;
	Poco::Net::HTTPSClientSession session;

	std::wstring getPreviousList();
	bool saveList();
};
