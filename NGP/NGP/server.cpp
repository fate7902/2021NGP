#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#pragma comment(lib,"ws2_32")
#include <WinSock2.h>
#include <iostream>
#include <time.h>
#include <random>
//#include <queue>
//#include <mutex>
#include "protocol.h"
using namespace std;

#define MAXTIME 100
#define INFINITE 9999

//queue<CLIENT_DATA*> recvQueue;
//mutex recvLock;
CLIENT_INFO clientInfo[3];
// 0: ���� 1: ������� ��ֹ� 2~5: ��ֹ�
OBJECT_INFO objectInfo[6];

bool gameStart = false;
/* �ǹ� - �̵��� �̵� ũ�� �����ϱ�(���� Ÿ�� ���� ����) */
float dx = 0.2;
//short dy = 1;
float dz = 0.2;

default_random_engine dre;
uniform_int_distribution<> uid(BALL, BULLDOZER);

int recvn(SOCKET s, char* buf, int len, int flags) {
	int received;
	char* ptr = buf;
	int left = len;

	while (left > 0) {
		received = recv(s, ptr, left, flags);
		if (SOCKET_ERROR == received)
			return SOCKET_ERROR;
		else if (0 == received)
			break;
		left -= received;
		ptr += received;
	}
	return (len - left);
}

void SC_GAMESTART()
{
	SERVER_DATA server_data;
	server_data.dataType = GAME_START;
	gameStart = true;
	for (const auto& clients : clientInfo)
		send(clients.sock, (char*)&server_data, sizeof(SERVER_DATA), 0);
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

	for (const auto& clients : clientInfo) {
		if (true == clients.alive) {
			// ���ο��� ����
			if (id == clients.id) {
				server_data.subDataType = SELF;
				for (const auto& other : clientInfo) {
					if (clients.id > other.id) {
						SERVER_DATA other_server_data;
						other_server_data.dataType = LOGIN;
						other_server_data.subDataType = OTHER;
						other_server_data.id = clientInfo[other.id].id;
						other_server_data.x = clientInfo[other.id].x;
						other_server_data.y = clientInfo[other.id].y;
						other_server_data.z = clientInfo[other.id].z;
						send(clients.sock, (char*)&other_server_data, sizeof(SERVER_DATA), 0);
					}
				}
			}
			// �ٸ� �������� ����
			else
				server_data.subDataType = OTHER;
			send(clients.sock, (char*)&server_data, sizeof(SERVER_DATA), 0);
		}
	}
}

/* �ǹ� -  Goal_Check()�� Coll_check() �ۼ��ϱ� */
bool Goal_Check()
{
	return false;
}

bool Coll_check()
{
	return false;
}

void SC_SEND(CLIENT_DATA clientData)
{
	int id = clientData.id;
	SERVER_DATA server_data;
	server_data.dataType = LOCATION;
	server_data.subDataType = PLAYER;
	switch (clientData.type) {
	case MOVE_FRONT:
		clientInfo[id].z -= dz;
		cout << clientInfo[id].x << " " << clientInfo[id].y << " " << clientInfo[id].z << "  id: " << id << endl;
		break;
	case MOVE_BACK:
		clientInfo[id].z += dz;
		break;
	case MOVE_RIGHT:
		clientInfo[id].x += dx;
		break;
	case MOVE_LEFT:
		clientInfo[id].x -= dx;
		break;
	}

	server_data.x = clientInfo[id].x;
	server_data.y = clientInfo[id].y;
	server_data.z = clientInfo[id].z;
	server_data.id = id;

	for (const auto& clients : clientInfo)
		send(clients.sock, (char*)&server_data, sizeof(SERVER_DATA), 0);
	
	// ���� �̵���Ų ���� ����� ������� �Ǵ�
	if (true == Goal_Check()) {
		server_data.dataType = GAME_RESULT;
		server_data.mission_result = true;
		clientInfo[id].alive = false;
		send(clientInfo[id].sock, (char*)&server_data, sizeof(SERVER_DATA), 0);
	}
	
	// ���� �̵���Ų ���� �浹���� �Ǵ�
	if (true == Coll_check()) {
		server_data.dataType = GAME_RESULT;
		server_data.mission_result = false;
		/* 
		�ǹ� - �浹�� ��ġ�����̳� ��� �������� ������ �κ��� �������
		for (auto& clients : clientInfo){
			������ ����
			send();
		}
		�������� �����ϱ�
		*/
		//clientInfo[id].alive = false;		// �浹�� ���� ������ ��� �ּ� Ǯ�� ���� ���� �ƴ� ��� ����
		send(clientInfo[id].sock, (char*)&server_data, sizeof(SERVER_DATA), 0);
	}
}

DWORD WINAPI SC_TIME(LPVOID arg)
{
	// ������ ������ ���� ����
	SERVER_DATA server_data;
	server_data.dataType = TIME;
	server_data.time = MAXTIME;		// �ʱ� �ð� ���� �� ����
	auto start = INFINITE;
	int send_time = MAXTIME + 1;
	while (true) {
		if (true == gameStart){
			if (INFINITE == start)
				start = clock();
			if (server_data.time != send_time) {
				for (const auto& clients : clientInfo)
					send(clients.sock, (char*)&server_data, sizeof(SERVER_DATA), 0);
			}
			if (0 == send_time) {
				server_data.dataType = GAME_OVER;
				for (const auto& clients : clientInfo) {
					if(true == clients.alive)
						send(clients.sock, (char*)&server_data, sizeof(SERVER_DATA), 0);
				}
			}
		}
		send_time = server_data.time;
		auto end = clock();
		server_data.time = MAXTIME - (int)((end - start) / 1000);
	}
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
		if (SOCKET_ERROR == retval)
			break;
		
		// ť ����
		//recvLock.lock();
		//recvQueue.emplace(clientData);
		//recvLock.unlock();

		// ť �̻��� - ���� ������ ���� �� send()
		SC_SEND(clientData);
	}
	closesocket(client_sock);
	return 0;
}

DWORD WINAPI SC_OBJECT_MOVE(LPVOID arg)
{
	SERVER_DATA server_data;
	server_data.dataType = LOCATION;
	server_data.subDataType = OBJECT;

	while (true) {
		if (true == gameStart) {
			/* �ǹ� - ������Ʈ �ڷᱸ�� ���� �� �� �����Ӵ� ������ ���� �� ������ �����Ͽ� ��� */
			// -6.25, 0 , 6.25
			for (auto& objects : objectInfo) {
				switch (objects.id) {
				case 0:	// BOSS
					objects.objectType = BOSS;
					objects.z += 0.2;
					break;
				case 1:	// TRACKER
					objects.objectType = TRACKER;
					objects.z += 0.2;
					break;
				case 2:
					if (objects.objectType == NULL)
						objects.objectType = uid(dre);
					else
						objects.z -= 0.2;
					
					break;
				/*case 3:
					if (objects.objectType == NULL)
						objects.objectType = uid(dre);
					break;
				case 4:
					if (objects.objectType == NULL)
						objects.objectType = uid(dre);
					break;
				case 5:
					if (objects.objectType == NULL)
						objects.objectType = uid(dre);
					break;*/
				}
				//objects.x += odx;
				//objects.y += ody;
				//objects.z += odz;
				server_data.id = objects.id;
				server_data.objectType = objects.objectType;
				server_data.x = objects.x;
				server_data.y = objects.y;
				server_data.z = objects.z;

				if (objectInfo[1].z > objectInfo[2].z) {
					objectInfo[2].objectType = NULL;
					objectInfo[2].z = objectInfo[0].z + 10;
				}

				for (const auto& clients : clientInfo)
					send(clients.sock, (char*)&server_data, sizeof(SERVER_DATA), 0);

			}
		}
	}
}

int main(int argc, char* argv[]) {
	int retval;

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == listen_sock)
		cout << "���Ͽ���" << endl;

	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (SOCKET_ERROR == retval)
		cout << "�ּ��Ҵ翡��" << endl;

	retval = listen(listen_sock, SOMAXCONN);
	if (SOCKET_ERROR == retval)
		cout << "�����û����" << endl;

	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	HANDLE hThread;
	int userCount = 0;

	// �ð� ������, ������Ʈ �̵� ������ ����
	hThread = CreateThread(NULL, 0, SC_TIME, NULL, 0, NULL);
	hThread = CreateThread(NULL, 0, SC_OBJECT_MOVE, NULL, 0, NULL);

	while (1) {
		// 3���� ������ ������ ������
		if (false == gameStart) {
			addrlen = sizeof(clientaddr);
			client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
			if (INVALID_SOCKET == client_sock) {
				cout << "�����������" << endl;
				break;
			}

			hThread = CreateThread(NULL, 0, S_RECV_PACKET, (LPVOID)client_sock, 0, NULL);
			if (NULL == hThread)
				closesocket(client_sock);
			else
				CloseHandle(hThread);

			//// ������ ���� �ʱ� ���� ����
			clientInfo[userCount].sock = client_sock;
			clientInfo[userCount].alive = true;
			clientInfo[userCount].id = userCount;
			/* �ǹ� - �ʱ� ���� ���� ��ġ ���� */
			clientInfo[userCount].x = -3 + (userCount  * 3);
			clientInfo[userCount].y = 0;
			clientInfo[userCount].z = 0;

			// ������ ���� ���� ����
			SC_LOGIN(userCount++);

			// 3���� ������ �����ϸ� ���� ����
			if (clientInfo[0].alive && clientInfo[1].alive && clientInfo[2].alive) {
				objectInfo[0].x = 0;
				objectInfo[0].y = 0;
				objectInfo[0].z = 10;

				objectInfo[1].x = 0;
				objectInfo[1].y = 0;
				objectInfo[1].z = -10;			// BOSS�� 20���̰� ���� �Ѵ�.

				for (int i = 2; i < 6; ++i) {
					objectInfo[i].x = 0;
					objectInfo[i].y = 0;
					objectInfo[i].z = 20;
				}

				SC_GAMESTART();
			}
		}
		// 3�� ���� �� ���� ����
		else {

		}
	}
	closesocket(listen_sock);

	WSACleanup();
	return 0;
}