#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#pragma comment(lib,"ws2_32")
#include <WinSock2.h>
#include <iostream>
//#include <queue>
//#include <mutex>
#include "protocol.h"
using namespace std;

#define SERVERPORT 9000
#define BUFSIZE 512

//queue<CLIENT_DATA*> recvQueue;
//mutex recvLock;
CLIENT_INFO clientInfo[3];

int recvn(SOCKET s, char* buf, int len, int flags) {
	int received;
	char* ptr = buf;
	int left = len;

	while (left > 0) {
		received = recv(s, ptr, left, flags);
		if (SOCKET_ERROR == received) {
			return SOCKET_ERROR;
		}
		else if (0 == received) {
			break;
		}
		left -= received;
		ptr += received;
	}
	return (len - left);
}

void SC_GAMESTART()
{
	SERVER_DATA server_data;
	server_data.dataType = GAME_START;
	for (auto& clients : clientInfo) {
		send(clients.sock, (char*)&server_data, sizeof(SERVER_DATA), 0);
	}
}

void SC_LOGIN(int id)
{
	// ������ ������ ���� ����
	SERVER_DATA server_data;
	server_data.dataType = LOGIN;
	server_data.id = clientInfo[id].id;	
	server_data.x = clientInfo[id].x;
	server_data.y = clientInfo[id].y;
	server_data.z = clientInfo[id].z;

	for (auto& clients : clientInfo) {
		if (true == clients.alive) {
			// ���ο��� ����
			if (id == clients.id) {
				server_data.subDataType = SELF;				
			}
			// �ٸ� �������� ����
			else {
				server_data.subDataType = OTHER;
			}
			send(clients.sock, (char*)&server_data, sizeof(SERVER_DATA), 0);
		}
	}
}

void SC_SEND(CLIENT_DATA clientData)
{
	SERVER_DATA server_data;
	server_data.dataType = LOCATION;
	switch (clientData.type) {
	case MOVE_FRONT:
		break;
	case MOVE_BACK:
		break;
	case MOVE_RIGHT:
		break;
	case MOVE_LEFT:
		break;
	}
	server_data.id = clientData.id;
	//for (int i = 0; i < size(clients); ++i)
	//	send(server_data);
	//
	//if (true == Goal_Check) {
	//	SERVER_DATA server_data;
	//	server_data.dataType = GAME_RESULT;
	//	server_data.mission_result = true;
	//	send(server_data);
	//}
	//
	//if (true == Coll_check) {
	//	SERVER_DATA server_data;
	//	server_data.dataType = GAME_RESULT;
	//	server_data.mission_result = false;
	//	send(server_data);
	//}
}

DWORD WINAPI S_RECV_PACKET(LPVOID arg) 
{
	SOCKET client_sock = (SOCKET)arg;
	int retval;
	SOCKADDR_IN clientaddr;
	int addrlen;
	CLIENT_DATA clientData;

	// Ŭ���̾�Ʈ ���� ���
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (SOCKADDR*)&clientaddr, &addrlen);
	while (true) {
		retval = recvn(client_sock, (char*)&clientData, sizeof(CLIENT_DATA), 0);
		if (SOCKET_ERROR == retval) {
			break;
		}

		// ť ����
		//recvLock.lock();
		//recvQueue.emplace(clientData);
		//recvLock.unlock();

		// ť �̻��� - ���� ������ ���� �� send()
	}
	closesocket(client_sock);
	return 0;
}

int main(int argc, char* argv[]) {
	int retval;

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		return 1;
	}

	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == listen_sock) {
		printf("���Ͽ���\n");
	}

	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (SOCKET_ERROR == retval) {
		printf("�ּ��Ҵ翡��\n");
	}

	retval = listen(listen_sock, SOMAXCONN);
	if (SOCKET_ERROR == retval) {
		printf("�����û����\n");
	}

	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	HANDLE hThread;
	int userCount = 0;

	while (1) {
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
		if (INVALID_SOCKET == client_sock) {
			printf("�����������\n");
			break;
		}

		hThread = CreateThread(NULL, 0, S_RECV_PACKET, (LPVOID)client_sock, 0, NULL);
		if (NULL == hThread) {
			closesocket(client_sock);
		}
		else {
			CloseHandle(hThread);
		}

		// ������ ���� �ʱ� ���� ����
		clientInfo[userCount].sock = client_sock;
		clientInfo[userCount].alive = true;
		clientInfo[userCount].id = userCount;
		clientInfo[userCount].x = 0;
		clientInfo[userCount].y = 0;
		clientInfo[userCount].z = 0;
		
		// ������ ���� ���� ����
		SC_LOGIN(userCount++);

		// 3���� ������ �����ϸ� ���� ����
		if(clientInfo[0].alive && clientInfo[1].alive && clientInfo[2].alive)
			SC_GAMESTART();
	}
	closesocket(listen_sock);

	WSACleanup();
	return 0;
}