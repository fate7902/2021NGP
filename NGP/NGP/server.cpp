#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#pragma comment(lib,"ws2_32")
#include <WinSock2.h>
#include <iostream>
#include <queue>
#include <mutex>
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

void SC_LOGIN(CLIENT_INFO clientInfo)
{
	SERVER_DATA server_data;
	server_data.dataType = LOGIN;
	for (int i = 0; i < size(clients); ++i) {
		send(server_data);
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

	// 클라이언트 정보 얻기
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (SOCKADDR*)&clientaddr, &addrlen);
	while (true) {
		retval = recvn(client_sock, (char*)&clientData, sizeof(CLIENT_DATA), 0);
		if (SOCKET_ERROR == retval) {
			break;
		}

		// 큐 사용시
		//recvLock.lock();
		//recvQueue.emplace(clientData);
		//recvLock.unlock();

		// 큐 미사용시 - 받은 데이터 적용 후 send()
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
		printf("소켓에러\n");
	}

	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (SOCKET_ERROR == retval) {
		printf("주소할당에러\n");
	}

	retval = listen(listen_sock, SOMAXCONN);
	if (SOCKET_ERROR == retval) {
		printf("연결요청에러\n");
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
			printf("연결수락에러\n");
			break;
		}

		hThread = CreateThread(NULL, 0, S_RECV_PACKET, (LPVOID)client_sock, 0, NULL);
		if (NULL == hThread) {
			closesocket(client_sock);
		}
		else {
			CloseHandle(hThread);
		}

		// 접속한 유저 초기 정보 셋팅
		clientInfo[userCount].alive = true;
		clientInfo[userCount].id = userCount;
		clientInfo[userCount].x = 0;
		clientInfo[userCount].y = 0;
		clientInfo[userCount].z = 0;
		

	}
	closesocket(listen_sock);

	WSACleanup();
	return 0;
}