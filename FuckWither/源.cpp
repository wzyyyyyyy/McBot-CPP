#include "pch.h"
#include<JsonLoader.h>
#include<rapidjson/document.h>
#include<rapidjson/writer.h>
#include <Windows.h>
#include <time.h>
#include "BDSCLASS.hpp"
#include <chrono>
#include <stdio.h>
#include <filesystem>
#include<List>
#define log std::cout
#ifndef access
#  define access(ptr, type, off) (*((type *) (((uintptr_t) ptr) + off)))
#endif
SOCKET client;
std::list<std::string> playerlist;
void initialization();
bool s_send(std::string msg);
std::string getOnlinePlayer();
template <size_t size> void UtoA_Fill(char(&buf)[size], int num) {
	int nt = size - 1;
	buf[nt] = 0;
	for (auto i = nt - 1; i >= 0; --i) {
		char d = '0' + (num % 10);
		num /= 10;
		buf[i] = d;
	}
}
string getTime() {
	auto timet = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	tm time;
	char buf[3] = { 0 };
	localtime_s(&time, &timet);
	std::string str(std::to_string((time.tm_year + 1900)));
	str += "-";
	UtoA_Fill(buf, time.tm_mon + 1);
	str += buf; str += "-";
	UtoA_Fill(buf, time.tm_mday);
	str += buf; str += " ";
	UtoA_Fill(buf, time.tm_hour);
	str += buf; str += ":";
	UtoA_Fill(buf, time.tm_min);
	str += buf; str += ":";
	UtoA_Fill(buf, time.tm_sec);
	str += buf;
	return "["+str+"]";
}
THook(void*,"?createChat@TextPacket@@SA?AV1@AEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@000@Z", void* _this, std::string& player_name, std::string& target, std::string& msg, std::string& chat_style) {
	
	log << getTime() <<
		" <" <<
		player_name <<
		"> : " <<
		target << 
		std::endl;
	return original(_this, player_name, target, msg, chat_style);
}
THook(void, "?onPlayerJoined@ServerScoreboard@@UEAAXAEBVPlayer@@@Z",
	void* a1, Player* p) {
	playerlist.push_back(p->getNameTag());
	string msg = u8"{\"type\":\"PlayerJoinEvent\",\"PlayerName\":\"" + p->getNameTag() + "\",\"onlinePlayer\":\"" + getOnlinePlayer() + "\"}"; 
	s_send(msg);
	return original(a1, p);
}
THook(void, "?_onPlayerLeft@ServerNetworkHandler@@AEAAXPEAVServerPlayer@@_N@Z",
	void* _this, Player* p, char v3) {
	playerlist.remove((p->getNameTag()));
	string msg = u8"{\"type\":\"PlayerLeftEvent\",\"PlayerName\":\"" + p->getNameTag() + "\",\"onlinePlayer\":\""+getOnlinePlayer()+"\"}";
	s_send(msg);
	return original(_this, p, v3);
}
void entry() {
	SOCKADDR_IN server;
	initialization();
	server.sin_family = AF_INET;
	server.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	server.sin_port = htons(19133);
	client = socket(AF_INET, SOCK_STREAM, 0);
	if (connect(client, (SOCKADDR*)&server, sizeof(SOCKADDR)) == SOCKET_ERROR) {
		cout << u8"Socket连接失败！" << endl;
		WSACleanup();
	}
	else {
		cout << u8"Socket连接成功！" << endl;
	}
	std::cout << getTime() << " [McBot]Loaded!\n";
}
bool s_send(std::string msg) {
	int i = send(client, msg.c_str(), msg.length(), 0);
	if (i == 0) {
		return 0;
	}
	return 1;
}
std::string getOnlinePlayer() {
	string re = "";
	for (auto i = playerlist.begin(); i != playerlist.end(); i++) {
		re.append(*i+",");
	}
	re = re.substr(0, re.length() - 1);
	return re;
}
void initialization() {
	WORD w_req = MAKEWORD(2, 2);
	WSADATA wsadata;
	int err;
	err = WSAStartup(w_req, &wsadata);
	if (err != 0) {
		cout << u8"初始化Sovket失败！" << endl;
	}
}