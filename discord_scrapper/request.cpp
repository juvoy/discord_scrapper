#include "request.h"

Request::Request(std::string web_address) : Request(web_address, "")
{
}

Request::Request(std::string web_address, std::map<std::string, std::string> headers)
{
    std::string header = "";
    for (auto const& [key, val] : headers)
    {
        header += key;
        header += ": ";
        header += val;
        header += "\r\n";
    }

    this->headers = header;
    this->web_address = web_address;
}

Request::Request(std::string web, std::string h)
{
    this->web_address = web;
    this->headers = h;
}

std::string Request::Get()
{
    std::string returnBody = "";

    CHAR buffer[4096];
    DWORD bytesRead;
    hInternet = InternetOpenA("Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:129.0) Gecko/20100101 Firefox/129.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (!hInternet) {
        std::cerr << "InternetOpen failed: " << GetLastError() << std::endl;
        return "";
    }

    std::wstring web_address(this->web_address.begin(), this->web_address.end());
    std::wstring header(this->headers.begin(), this->headers.end());
    hConnect = InternetOpenUrlW(hInternet, web_address.c_str(), header.c_str(), 0, INTERNET_FLAG_RELOAD | INTERNET_FLAG_SECURE, 0); // DONT CHANGE TO ASCII IT DOESNT WORK WITH ASCII
    if (!hConnect) {
        std::cerr << "InternetOpenUrl failed: " << GetLastError() << std::endl;
        InternetCloseHandle(hInternet);
        return "";
    }


    while (InternetReadFile(hConnect, buffer, sizeof(buffer) - 1, &bytesRead) && bytesRead > 0) {
        buffer[bytesRead] = '\0';
        returnBody += buffer;
    }

    if (GetLastError() != ERROR_SUCCESS) {
        std::cerr << "InternetReadFile failed: " << GetLastError() << std::endl;
    }

    // Clean up
    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);

    return returnBody;
}
