#include "utility.h"

std::string utility::GetWorkingDirectory()
{
    char buffer[MAX_PATH] = { 0 };
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    std::string::size_type pos = std::string(buffer).find_last_of("\\/");
    return std::string(buffer).substr(0, pos);
}

std::vector<BYTE> utility::StringToByte(std::string s) {
    std::vector<BYTE> ret = {};
    for (int i = 0; i < strlen(s.c_str()); i++) {
        ret.push_back(s[i]);
    }
    return ret;
}

std::string utility::HexToString(std::vector<BYTE> vec)
{
    std::stringstream ss;
    for (const auto& byte : vec) {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
    }
    return ss.str();
}

std::string utility::GetCPUID()
{
    int cpuInfo[4] = { -1 };
    __cpuid(cpuInfo, 0);
    std::ostringstream oss;
    oss << std::hex << cpuInfo[1] << cpuInfo[3] << cpuInfo[2];
    return oss.str();
}
