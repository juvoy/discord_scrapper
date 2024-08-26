#pragma once

#include <string>
#include <map>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <wininet.h>

#pragma comment(lib, "wininet.lib")

#include <iostream>

class Request {
public:
	Request(std::string web_address, std::string headers);

	Request(std::string web_address);
	Request(std::string web_address, std::map<std::string, std::string> headers);


	std::string Get();
private:
	std::string web_address, headers;

	HINTERNET hInternet = NULL, hConnect = NULL;
};