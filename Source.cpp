#include <iostream>
#include <Windows.h>
#include <future>
#include "LocalFileTracking.h"
using namespace std;

static void RestartApp()
{
	std::wstring commandLine = GetCommandLine();
	std::wstring executablePath = commandLine.substr(0, commandLine.find(L" "));
	std::wstring arguments = commandLine.substr(executablePath.size());
	ShellExecute(NULL, L"open", executablePath.c_str(), arguments.c_str(), NULL, SW_SHOWNORMAL);
	exit(0);
}

int main()
{
	LocalFileTracking fileTracker(L".\\");
	fileTracker.onFileCreate([](std::wstring path) {
		std::wcout << L"File added: " << path << std::endl;
		});

	fileTracker.onFileModification([](std::wstring path) {
		std::wcout << L"File mod: " << path << std::endl;
		});
	
	std::cin.get();
}