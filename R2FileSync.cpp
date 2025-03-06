#include "R2FileSync.h"
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/StreamCopier.h>
#include <sstream>

R2FileSync::R2FileSync(const std::wstring& remoteHost)
{
	this->remoteHost = remoteHost;
	// todo: need to replace with variable from env or any type of config file (yaml, json, etc)
	//session("r2.nduc.workers.dev", 443);
}

R2FileSync::~R2FileSync()
{
}

std::wstring R2FileSync::getPreviousList()
{
	return std::wstring();
}

bool R2FileSync::saveList()
{
	return false;
}

void R2FileSync::uploadFile(const std::wstring& localPath, const std::wstring& remotePath) {

}

void R2FileSync::downloadFile(const std::wstring& remotePath, const std::wstring& localPath)
{
}

void R2FileSync::deleteFile(const std::wstring& remotePath)
{
}

void R2FileSync::renameFile(const std::wstring& remotePath, const std::wstring& newName)
{
}

std::vector<std::wstring> R2FileSync::fetchFileList(const std::wstring& remotePath)
{
	return std::vector<std::wstring>();
}

std::vector<std::wstring> R2FileSync::fetchModificationList(const std::wstring& remotePath)
{
	return std::vector<std::wstring>();
}
