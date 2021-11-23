#include "network.h"

void Network::network()
{
	int retval;

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		exit(-1);

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

		if (start != true)
			continue;
	}
}
