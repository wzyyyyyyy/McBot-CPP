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
#include <thread>
#define log std::cout
#ifndef access
#  define access(ptr, type, off) (*((type *) (((uintptr_t) ptr) + off)))
#endif
SOCKET client;
std::list<std::string> playerlist;
bool client_first = true;
void initialization();
bool s_send(std::string msg);
void CreatSocket();
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
	return "["+str+" BOT]";
}
THook(void*,"?createChat@TextPacket@@SA?AV1@AEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@000@Z", void* _this, std::string& player_name, std::string& target, std::string& msg, std::string& chat_style) {
	string msg_ = u8"{\"type\":\"ChatEvent\",\"PlayerName\":\"" + player_name + "\",\"msg\":\"" + target + "\"}";
	s_send(msg_);
	return original(_this, player_name, target, msg, chat_style);
}
THook(void, "?onPlayerJoined@ServerScoreboard@@UEAAXAEBVPlayer@@@Z",
	void* a1, Player* p) {
	playerlist.push_back(p->getNameTag());
	string msg = u8"{\"type\":\"PlayerJoinEvent\",\"PlayerName\":\"" + p->getNameTag() + "\",\"onlinePlayer\":\"" + getOnlinePlayer() + "\"}"; 
	s_send(msg);
	return original(a1, p);
}
THook(void, "?die@Mob@@UEAAXAEBVActorDamageSource@@@Z",
	Mob* mob, void* a1) {
	int mobid = access(mob, int, 948); //IDA Actor::getEntityType
	if (mobid == 1 && mob->getNameTag() != "") {
		string msg = u8"{\"type\":\"PlayerDieEvent\",\"PlayerName\":"+mob->getNameTag()+"\",\"pos\":\"" + std::to_string((int)mob->getPos().x) + ";" + std::to_string((int)mob->getPos().y) + ";" + std::to_string((int)mob->getPos().z) + "\"}";
		s_send(msg);
	}
	return original(mob, a1);
}
THook(void, "?_onPlayerLeft@ServerNetworkHandler@@AEAAXPEAVServerPlayer@@_N@Z",
	void* _this, Player* p, char v3) {
	playerlist.remove((p->getNameTag()));
	string msg_ = u8"{\"type\":\"PlayerLeftEvent\",\"PlayerName\":\"" + p->getNameTag() + "\",\"onlinePlayer\":\""+getOnlinePlayer()+"\"}";
	s_send(msg_);
	return original(_this, p, v3);
}
void entry() {
	CreatSocket();
	std::cout << getTime() << " [McBot] Loaded!\n";
}
void CreatSocket() {
	SOCKADDR_IN server;
	if (client_first) {
		initialization();
		client_first = false;
	}
	server.sin_family = AF_INET;
	server.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	server.sin_port = htons(61046);
	client = socket(AF_INET, SOCK_STREAM, 0);
	if (connect(client, (SOCKADDR*)&server, sizeof(SOCKADDR)) == SOCKET_ERROR) {
		cout << getTime() << u8" Socket连接失败！" << endl;
		WSACleanup();
	}
	else {
		cout << getTime() << u8" Socket连接成功！" << endl;
	}
}
bool s_send(std::string msg) {
	int i = send(client, msg.c_str(), msg.length(), 0);
	if (!i) {
		std::cout << getTime() << u8" Socket连接已断开！正在尝试重连......" << endl;
		CreatSocket();
		send(client, msg.c_str(), msg.length(), 0);
		return 0;
	}
	else if (GetLastError()) {
		std::cout << getTime() << u8" Socket连接已断开！正在尝试重连......" << endl;
		CreatSocket();
		send(client, msg.c_str(), msg.length(), 0);
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
		cout << u8"初始化Socket失败！" << endl;
	}
}