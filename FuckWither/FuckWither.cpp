#include "pch.h"
#include <fstream>
#include <rapidjson/document.h>

bool enableWitherDestroy;
bool enableSkullDestroy;

void entry()
{
	std::string config_file = "config\\fuckwither.json";
	std::ifstream fs;
	fs.open(config_file, std::ios::in);
	enableWitherDestroy = false;
	enableSkullDestroy = false;
	if (!fs)
	{
		std::cout << "[FuckWither] " << config_file <<" not found, creating file(default value used)\n";
		std::ofstream of(config_file);
		if (of)
		{
			of << "{\"enableWitherDestroy\": false";
		}
		else 
		{
			std::cout << "[FuckWither] " << "config file creation failed, plase create \"config\" folder\n";
		}
	}
	else
	{
		std::string json;
		char buf[1024];
		while (fs.getline(buf, 1024))
		{
			json.append(buf);
		}
		rapidjson::Document document;
		document.Parse(json.c_str());
		enableWitherDestroy = document["enableWitherDestroy"].GetBool();
	}

	std::cout << "[FuckWither] Loaded!\n";
}

THook(bool, "?canDestroy@WitherBoss@@SA_NAEBVBlock@@@Z", void* blockpos)
{
	if (!enableWitherDestroy) return false;
	return original(blockpos);
}