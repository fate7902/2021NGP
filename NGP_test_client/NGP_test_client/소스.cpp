#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib,"ws2_32")
#include <WinSock2.h>
#include <iostream>
#include <string>
#include "C:\Users\Lenovo\Desktop\2021NGP-master\2021NGP-master\NGP\NGP\protocol.h"
using namespace std;

#define SERVERIP "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE 512

CLIENT_INFO clients[3];
int start, game_time;
int id;

void C_UPDATE(SERVER_DATA server_data);
void CS_MOVE(int key, SOCKET socket);

DWORD WINAPI C_SAVE_PACKET(LPVOID arg)
{
	SOCKET sock = (SOCKET)arg;
	SERVER_DATA server_data;

	while (true) {
		recv(sock, (char*)&server_data, sizeof(SERVER_DATA), 0);
		
		C_UPDATE(server_data);
	}

	closesocket(sock);
	return 0;
}

int main(int argc, char* argv[])
{
	int retval;

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == sock)
		cout << "socket 에러" << endl;

	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.S_un.S_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (SOCKET_ERROR == retval)
		cout << "connect 에러" << endl;

	char filename[BUFSIZE];
	char buf[BUFSIZE];
	char* data[2]{ 0 };
	int len;

	HANDLE hThread1;
	HANDLE hThread2;

	while (true) {
		CLIENT_DATA clientData;
		clientData.id = 23;
		clientData.type = MOVE_RIGHT;

		// 고정 길이
		retval = send(sock, (char*)&clientData, sizeof(CLIENT_DATA), 0);
		if (SOCKET_ERROR == retval) {
			cout << "send 에러" << endl;
			break;
		}

		hThread1 = CreateThread(NULL, 0, C_SAVE_PACKET, (LPVOID)sock, 0, NULL);
		if (NULL == hThread1) {
			closesocket(sock);
		}
		else {
			CloseHandle(hThread1);
		}

		if (start != true)
			continue;

		while (true) {
			int key;
			key = getchar();
			CS_MOVE(key, sock);
		}
	}
	//WSACleanup();
	return 0;
}

void C_UPDATE(SERVER_DATA server_data)
{
	switch (server_data.dataType)
	{
	case LOCATION: // 플레이어 위치값
		clients[server_data.id].x = server_data.x;
		clients[server_data.id].y = server_data.y;
		clients[server_data.id].z = server_data.z;
		break;
	case LOGIN: // 플레이어 접속
		id = server_data.id;
		clients[server_data.id].alive = true;
		clients[server_data.id].x = 0;
		clients[server_data.id].y = 0;
		clients[server_data.id].z = 0;
		break;
	case GAME_START:;   // 게임 시작
		start = true;
		break;
	case TIME: // 시간
		game_time = server_data.time;
		break;
	}
}

void CS_MOVE(int key, SOCKET socket)
{
	CLIENT_DATA client_data;
	switch (key) {
	case 'w':
		client_data.id = id;
		client_data.type = MOVE_FRONT;
		send(socket, (char*)&client_data, sizeof(CLIENT_DATA), 0);
		break;
	case 'S':
	case 's':
		client_data.id = id;
		client_data.type = MOVE_BACK;
		send(socket, (char*)&client_data, sizeof(CLIENT_DATA), 0);
		break;
	default:
		break;
	}
}

