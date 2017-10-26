// Written by John (10/20/17)

#include <iostream>
#include <Windows.h>
#include <iostream>
#include <string>
#include <sstream>
#include <WinINet.h>
#include <vector>
#include <future>
#pragma comment(lib, "wininet.lib")
#pragma warning(disable: 4996) // get rid of that one pesky warning that hates me for using deprecated functions

std::string MakeInternetRequest(const char* useragent, const char* url) {
	HINTERNET open = InternetOpenA(useragent, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, NULL);
	if (!open)
	{
		printf("An error occurred while attempting to establish a connection to the Internet. Error code: %u\n", GetLastError());
		InternetCloseHandle(open);
	}
	HINTERNET addr = InternetOpenUrlA(open, url, NULL, 0, INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_KEEP_CONNECTION, 0);
	if (!addr)
	{
		printf("An error occurred while attempting to establish a connection to the Internet. Error code: %u\n", GetLastError());
		InternetCloseHandle(addr);
		InternetCloseHandle(open);
	}

	char data[40000];
	DWORD bytesread = 0;
	std::string buf;
	ZeroMemory(data, sizeof(data));
	while (InternetReadFile(addr, data, 40000, &bytesread) && bytesread)
	{
		buf += data;
		ZeroMemory(data, sizeof(data));
	}
	InternetCloseHandle(addr);
	InternetCloseHandle(open);
	return buf;
}
int main() {
	SetConsoleTitle("Chat v1.0");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD)10);
	std::cout << "Hello, and welcome to Chat v1.0! Made by John." << std::endl << "Type \"exit\" without the quotes when you are typing messages to exit." << std::endl;
	std::cout << "Type in your username." << std::endl;
	std::string u;
	getline(std::cin, u);
	std::cout << "Now, type something." << std::endl;
	while (true) {
		std::string x;
		getline(std::cin, x);
		system("cls");
		if (x == "exit") {
			MakeInternetRequest("Chat v1.0", "https://[ domain redacted for privacy]/chat/connect2.php?delete=true");
			break;
		}
		std::stringstream url;
		url << "https://[ domain redacted for privacy]/chat/connect2.php?username=";
		url << u;
		if (x != "") {
			url << "&message=";
			url << x;
		}
		std::string y = MakeInternetRequest("Chat v1.0", url.str().c_str());
		std::cout << y.replace(y.end()-5, y.end(), "") << std::endl;
	}
	return 0;
}