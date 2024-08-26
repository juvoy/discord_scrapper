#include "cryptography.h"

Cryptography::Cryptography(std::vector<BYTE> key)
{
    NTSTATUS status = BCryptOpenAlgorithmProvider(&this->hAlg, BCRYPT_AES_ALGORITHM, NULL, 0);
    if (!BCRYPT_SUCCESS(status)) {
        std::cerr << "BCryptOpenAlgorithmProvider failed: " << status << std::endl;
        return;
    }

    // Generate a symmetric key
    status = BCryptGenerateSymmetricKey(this->hAlg, &this->hKey, NULL, 0, const_cast<BYTE*>(key.data()), (ULONG)key.size(), 0);
    if (!BCRYPT_SUCCESS(status)) {
        std::cerr << "BCryptGenerateSymmetricKey failed: " << status << std::endl;
        return;
    }


}

Cryptography::~Cryptography()
{
    BCryptDestroyKey(this->hKey);
    BCryptCloseAlgorithmProvider(this->hAlg, 0);
}

std::vector<BYTE> Cryptography::Encrypt(std::vector<BYTE> data)
{
    DWORD cbData = (DWORD)data.size();
    std::vector<BYTE> encryptedData(data.size() + 16); // Buffer for encrypted data

    NTSTATUS status = BCryptEncrypt(this->hKey, const_cast<BYTE*>(data.data()), cbData, NULL, NULL, 0, encryptedData.data(), (ULONG)encryptedData.size(), &cbResult, BCRYPT_BLOCK_PADDING);
    if (!BCRYPT_SUCCESS(status)) {
        std::cerr << "BCryptEncrypt failed: " << status << std::endl;
        return{};
    }

    std::cout << cbResult << std::endl;

    return encryptedData;
}

std::string Cryptography::Decrypt(size_t size, std::vector<BYTE> encryptedData, DWORD cbCustomResult)
{
    if (cbCustomResult == -1) {
        cbCustomResult = this->cbResult;
    }
    else {
        this->cbResult = 0;
    }

    std::vector<BYTE> decryptedData(255);

    NTSTATUS status = BCryptDecrypt(hKey, encryptedData.data(), cbCustomResult, NULL, NULL, 0, decryptedData.data(), (ULONG)decryptedData.size(), &cbResult, BCRYPT_BLOCK_PADDING);
    if (!BCRYPT_SUCCESS(status)) {
        std::cerr << "BCryptDecrypt failed: " << status << std::endl;
        return "N/A";
    }

    std::string decryptedText(decryptedData.begin(), decryptedData.begin() + cbResult);
    return decryptedText;
}

std::vector<BYTE> Cryptography::GenerateCustomKey(std::string str)
{
    std::vector<BYTE> ret = {};

    if (strlen(str.c_str()) > 32) {
        str = str.substr(0, 32);
    }

    if (strlen(str.c_str()) < 32) {
        int diff = 32 - strlen(str.c_str());

        for (int i = 0; i < diff; i++) {
            str.append("0");
        }
    }

    for (BYTE b : str) {
        ret.push_back(b);
    }

    return ret;
}

DWORD Cryptography::GetEncryptedBytes()
{
    return this->cbResult;
}
