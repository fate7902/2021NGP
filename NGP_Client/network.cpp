#include "network.h"

void Network::C_UPDATE(SERVER_DATA server_data)
{
	switch (server_data.dataType)
	{
	case LOCATION:
		switch (server_data.subDataType)
		{
		case PLAYER:
			clients[server_data.id].x = server_data.x;
			clients[server_data.id].y = server_data.y;
			clients[server_data.id].z = server_data.z;
			break;
		case OBJECT:

			objects[server_data.objectInfo.id].id = server_data.objectInfo.id;
			objects[server_data.objectInfo.id].objectType = server_data.objectInfo.objectType;
			objects[server_data.objectInfo.id].line = server_data.objectInfo.line;
			objects[server_data.objectInfo.id].moving = server_data.objectInfo.moving;
			
			objects[server_data.objectInfo.id].x = server_data.objectInfo.x;
			objects[server_data.objectInfo.id].y = server_data.objectInfo.y;
			objects[server_data.objectInfo.id].z = server_data.objectInfo.z;
			break;
		}
		break;
	case LOGIN:
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
		break;
	case LOGOUT:
		clients[server_data.id].alive = false;
		break;
	case GAME_START:
		gameover = false;
		gameclear = false;
		m_start = true;
		break;
	case TIME:
		game_time = server_data.time;
		break;
	case GAME_OVER:
		clients[server_data.id].alive = false;
		if (server_data.id == m_id) {
			m_start = false;
			gameover = true;
			setStart(false);
		}
		break;
	case GAME_CLEAR:
		m_start = false;
		gameclear = true;
		setStart(false);
		break;
	case REJECT:
		exit(0);
		break;
	case RESTART:
		if (server_data.id == m_id) {
			restart = true;
		}
		clients[server_data.id].alive = true;
		break;
	}
}

DWORD WINAPI C_SAVE_PACKET(LPVOID arg)
{
	SERVER_DATA server_data;
	Network* network = (Network*)arg;
	SOCKET sock = network->getSock();
	while (true) {
		int ret = recv(sock, (char*)&server_data, sizeof(SERVER_DATA), 0);
		if (ret == SOCKET_ERROR)
			cout << "Recv ERROR!" << endl;

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

}

void Network::CS_MOVE()
{
	CLIENT_DATA client_data;
	client_data.id = m_id;

	if (GetAsyncKeyState('W')) {
		client_data.type = MOVE_FRONT;
		send(m_sock, (char*)&client_data, sizeof(CLIENT_DATA), 0);
	}

	if (GetAsyncKeyState('S')) {
		client_data.type = MOVE_BACK;
		send(m_sock, (char*)&client_data, sizeof(CLIENT_DATA), 0);
	}

	if (GetAsyncKeyState('A')) {
		client_data.type = MOVE_LEFT;
		send(m_sock, (char*)&client_data, sizeof(CLIENT_DATA), 0);
	}

	if (GetAsyncKeyState('D')) {
		client_data.type = MOVE_RIGHT;
		send(m_sock, (char*)&client_data, sizeof(CLIENT_DATA), 0);
	}
}

void Network::CS_START()
{
	CLIENT_DATA client_data;
	client_data.id = m_id;
	client_data.type = GAME_START;
	send(m_sock, (char*)&client_data, sizeof(CLIENT_DATA), 0);
}