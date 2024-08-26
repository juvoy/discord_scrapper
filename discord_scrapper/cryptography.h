#pragma once

#include <vector>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <bcrypt.h>
#pragma comment (lib, "bcrypt.lib")

#include <iostream>
#include <format> 

class Cryptography {
public:
	Cryptography(std::vector<BYTE> key);
	~Cryptography();


	std::vector<BYTE> Encrypt(std::vector<BYTE> data);
	std::string Decrypt(size_t size, std::vector<BYTE> data, DWORD cbCustomResult = -1);

	static std::vector<BYTE> GenerateCustomKey(std::string string);

	DWORD GetEncryptedBytes();
private:
	BCRYPT_ALG_HANDLE hAlg = NULL;
	BCRYPT_KEY_HANDLE hKey = NULL;
	DWORD cbResult = 0;
};