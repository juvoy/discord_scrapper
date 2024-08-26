#pragma once

#include <windows.h>
#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <intrin.h>


namespace utility {

	std::string GetWorkingDirectory();

	std::vector<BYTE> StringToByte(std::string s);

	std::string HexToString(std::vector<BYTE> vec);

	std::string GetCPUID();


}