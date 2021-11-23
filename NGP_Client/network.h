#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib,"ws2_32")
#include <WinSock2.h>
#include <iostream>
#include "..\NGP\NGP\protocol.h"
using namespace std;

DWORD WINAPI C_SAVE_PACKET(LPVOID arg);

class Network {
	bool start = false;
	HANDLE hThread;
	CLIENT_INFO clients[3];
	int id;
	int game_time;
public:

	void C_UPDATE(SERVER_DATA server_data);
	void network();
};