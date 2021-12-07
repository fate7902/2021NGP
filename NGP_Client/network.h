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
	bool m_start = false;
	HANDLE m_hThread;
	int m_id;


	SOCKET m_sock;
public:
	CLIENT_INFO clients[3];
	OBJECT_INFO objects[6];

	void C_UPDATE(SERVER_DATA server_data);
	void network();
	void CS_MOVE();

	int getMyId() { return m_id; }
	bool getStart() { return m_start; }
	void setStart(bool end) { m_start = end; }
	SOCKET getSock() { return m_sock; }
	bool gameover = false;
	bool gameclear = false;
	int game_time;
};
