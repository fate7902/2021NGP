#include "network.h"

void Network::C_UPDATE(SERVER_DATA server_data)
{
	cout << "update" << endl;
	switch (server_data.dataType)
	{
	case LOCATION: // �÷��̾� ��ġ��
		clients[server_data.id].x = server_data.x;
		clients[server_data.id].y = server_data.y;
		clients[server_data.id].z = server_data.z;
		break;
	case LOGIN: // �÷��̾� ����
		id = server_data.id;
		clients[server_data.id].alive = true;
		clients[server_data.id].x = server_data.x;
		clients[server_data.id].y = server_data.y;
		clients[server_data.id].z = server_data.z;
		break;
	case GAME_START:   // ���� ����
		start = true;
		break;
	case TIME: // �ð�
		game_time = server_data.time;
		break;
	}
}

DWORD WINAPI C_SAVE_PACKET(LPVOID arg)
{
	SOCKET sock = (SOCKET)arg;
	SERVER_DATA server_data;

	Network network;

	while (true) {
		recv(sock, (char*)&server_data, sizeof(SERVER_DATA), 0);
		
		network.C_UPDATE(server_data);
	}

	closesocket(sock);
	return 0;
}

void Network::network()
{
	int retval;

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		exit(-1);

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == sock)
		cout << "socket ����" << endl;

	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.S_un.S_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (SOCKET_ERROR == retval)
		cout << "connect ����" << endl;

	hThread = CreateThread(NULL, 0, C_SAVE_PACKET, (LPVOID)sock, NULL, NULL);
	
	while (true) {
		//CLIENT_DATA clientData;
		//clientData.id = 23;
		//clientData.type = MOVE_RIGHT;

		//// ���� ����
		//retval = send(sock, (char*)&clientData, sizeof(CLIENT_DATA), 0);
		//if (SOCKET_ERROR == retval) {
		//	cout << "send ����" << endl;
		//	break;
		//}

		//if (start != true)
		//	continue;
	}
}