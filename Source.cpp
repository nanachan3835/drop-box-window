#include <iostream>
#include <Windows.h>

int main()
{
	std::cout << "Hello, World!" << std::endl;
	MessageBoxA(NULL, "Hello, World!", "Hello, World!", MB_OK);
	return 0;
}