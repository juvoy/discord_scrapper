#include <json/json.hpp>
#include <iostream>
#include <filesystem>
#include <fstream>

#include "request.h"
#include "cryptography.h"
#include "utility.h"




int main(int argc, char** argv) {
	std::string token = "", file_key = utility::GetWorkingDirectory().append("/authorization");

	if (!std::filesystem::exists(file_key)) {
		std::cout << "token: ";

		std::cin >> token;
		std::cout << std::endl;

		system("cls");

		std::ofstream keyFile(file_key);
		keyFile << token;
		keyFile.close();

	} else {
		std::ifstream file(file_key);
		std::getline(file, token);

		file.close();
	}

	std::map<std::string, std::string> headers = {
		{"Authorization", token},
		{"Content-Type", "application/json"},
		{"User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:129.0) Gecko/20100101 Firefox/129.0"},
	};

	
	std::string channel_id = "";
	std::cout << "channel_id: ";
	std::cin >> channel_id;


	char web_url[256];
	sprintf_s(web_url, "https://discord.com/api/v9/channels/%s/messages?limit=100", channel_id.c_str());

	Request* r = new Request(web_url, headers);

	std::string body = r->Get();

	std::map<std::string, std::string> data = {};
	nlohmann::json json = nlohmann::json::parse(body);

	for (const auto& obj : json) {
		data.insert({ obj["author"].at("id"), obj["author"].at("username").get<std::string>() });
		std::cout << obj["author"].at("id") << ":" << obj["author"].at("username").get<std::string>() << std::endl;
	}

	std::cout << "=================================================================" << std::endl <<
		"[0] Export to File" << std::endl;
	
	std::string decision = "";
	std::cin >> decision;

	if (decision == "0") {
		std::ofstream export_file(utility::GetWorkingDirectory().append("/exported_").append(std::to_string(time(nullptr))).append(".txt"));

		for (auto& obj : data) {
			export_file << obj.first << ":" << obj.second << std::endl;
		}

		export_file.close();
	}
}