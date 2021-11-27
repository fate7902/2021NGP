#include "network.h"

void Network::C_UPDATE(SERVER_DATA server_data)
{
	switch (server_data.dataType)
	{
	case LOCATION: // 플레이어 위치값
		switch (server_data.subDataType)
		{
		case PLAYER:
			clients[server_data.id].x = server_data.x;
			clients[server_data.id].y = server_data.y;
			clients[server_data.id].z = server_data.z;
			break;
		case OBJECT:
			break;
		}
		break;
	case LOGIN: // 플레이어 접속
		switch (server_data.subDataType) {
		// m_id : 자신 클라이언트 id
		// m_clients : 모든 클라이언트
		case SELF:
			m_id = server_data.id;
		case OTHER:
			clients[server_data.id].id = server_data.id;
			clients[server_data.id].alive = true;
			clients[server_data.id].x = server_data.x;
			clients[server_data.id].y = server_data.y;
			clients[server_data.id].z = server_data.z;
			break;
		}
		for (int i = 0; i < 3; ++i) {
			cout << i << " - " << clients[i].x << endl;
		}

		break;
	case GAME_START:   // 게임 시작
		m_start = true;
		break;
	case TIME: // 시간
		m_game_time = server_data.time;
		break;
	}
}

DWORD WINAPI C_SAVE_PACKET(LPVOID arg)
{
	//SOCKET sock = (SOCKET)arg;
	//Network network;
	SERVER_DATA server_data;
	Network* network = (Network*)arg;
	SOCKET sock = network->getSock();
	while (true) {
		recv(sock, (char*)&server_data, sizeof(SERVER_DATA), 0);

		network->C_UPDATE(server_data);
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

	m_sock = socket(AF_INET, SOCK_STREAM, 0);
	//SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == m_sock)
		cout << "socket 에러" << endl;

	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.S_un.S_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(m_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (SOCKET_ERROR == retval)
		cout << "connect 에러" << endl;

	m_hThread = CreateThread(NULL, 0, C_SAVE_PACKET, (LPVOID)this, NULL, NULL);
	
	//while (true) {
		//CLIENT_DATA clientData;
		//clientData.id = 23;
		//clientData.type = MOVE_RIGHT;

		//// 고정 길이
		//retval = send(sock, (char*)&clientData, sizeof(CLIENT_DATA), 0);
		//if (SOCKET_ERROR == retval) {
		//	cout << "send 에러" << endl;
		//	break;
		//}

		//if (start != true)
		//	continue;
	//}
}

void Network::CS_MOVE(char key)
{
	CLIENT_DATA client_data;
	client_data.id = m_id;

	// switch -> if로 바꿔야 한다. : 동시에 누를 수 있게 ( if문 각각에 send를 해야 한다.)
	switch (key) {
	case 'w':
	case 'W':
		client_data.type = MOVE_FRONT;
		break;
	case 's':
	case 'S':
		client_data.type = MOVE_BACK;
		break;
	case 'a':
	case 'A':
		client_data.type = MOVE_LEFT;
		break;
	case 'd':
	case 'D':
		client_data.type = MOVE_RIGHT;
		break;
	}

	send(m_sock, (char*)&client_data, sizeof(CLIENT_DATA), 0);
}