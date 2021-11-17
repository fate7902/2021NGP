// recv thread function
void RecvThread()
{
	SERVER_DATA server_data;
	while (true) {
		recv(server_data);
		lock();
		queue.input(server_data);
		unlock();
	}
}

void KeyInput()
{
	CLIENT_DATA client_data;
	switch (key) {
	case VK_UP:
		client_data.type = MOVE_FRONT;
		send(client_data);
		break;
	case VK_DOWN:
		client_data.type = MOVE_BACK;
		send(client_data);
		break;
	case VK_LEFT:
		client_data.type = MOVE_LEFT;
		send(client_data);
		break;
	case VK_RIGHT:
		client_data.type = MOVE_RIGHT;
		send(client_data);
		break;
	}
}

void update_login(SERVER_DATA server_data)
{
	clients[server_data.id].alive = true;
	clients[server_data.id].x = 0;
	clients[server_data.id].y = 0;
	clients[server_data.id].z = 0;
}

void update_location(SERVER_DATA server_data)
{
	clients[server_data.id].x = server_data.x;
	clients[server_data.id].y = server_data.y;
	clients[server_data.id].z = server_data.z;
}

void update_gameover(SERVER_DATA server_data)
{
	clients[server_data.id].alive = false;
}

void update_time(SERVER_DATA server_data)
{
	time = server_data.time;
}

void update_result(SERVER_DATA server_data)
{
	switch (server_data.rank)
	{
	case 1:
		openFirstScene();
		break;
	case 2:
		openSecondScene();
		break;
	case 3:
		openThirdScene();
		break;
	}
}

void update_gamestart()
{
	bool start = true;
}