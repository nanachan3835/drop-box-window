#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <Windows.h>
#include <future>
#include "LocalFileTracking.h"
#include <Poco/Net/HTTPSClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/StreamCopier.h>
#include <shellapi.h>
#include <Poco/Net/SSLManager.h>
#include <Poco/Net/AcceptCertificateHandler.h>
#include <Poco/Net/Context.h>

using namespace Poco::Net;
using namespace Poco;
using namespace std;

static void RestartApp()
{
	std::wstring commandLine = GetCommandLine();
	std::wstring executablePath = commandLine.substr(0, commandLine.find(L" "));
	std::wstring arguments = commandLine.substr(executablePath.size());
	ShellExecute(NULL, L"open", executablePath.c_str(), arguments.c_str(), NULL, SW_SHOWNORMAL);
	exit(0);
}

static void testFileTrackerAsync(LocalFileTracking& fileTracker) 
{
	fileTracker.onFileCreate([](std::wstring path) {
		std::wcout << L"File added: " << path << std::endl;
		});

	fileTracker.onFileModification([](std::wstring path) {
		std::wcout << L"File mod: " << path << std::endl;
		});
}

static void testRestApi() 
{

}

static void testR2() 
{

}

int main()
{
	LocalFileTracking fileTracker(L".\\");
	testFileTrackerAsync(fileTracker);

	SharedPtr<InvalidCertificateHandler> certHandler = new AcceptCertificateHandler(false);
	Context::Ptr context = new Context(Context::CLIENT_USE, "");
	SSLManager::instance().initializeClient(0, certHandler, context);

	HTTPSClientSession session("r2.nduc.workers.dev", 443);
	HTTPRequest request(HTTPRequest::HTTP_GET, "/shared-bucket");
	session.sendRequest(request);
	HTTPResponse response;
	istream& responseStream = session.receiveResponse(response);
	string responseStr;
	StreamCopier::copyToString(responseStream, responseStr);
	cout << responseStr << endl;
	cin.get();
}