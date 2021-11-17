// recv thread function
void recvThread()
{
	CLIENT_DATA client_data;
	while (true) {
		recv(client_data);
		lock();
		queue.input(client_data);
		unlock();
	}
}

void send_gameover()
{
	bool alive = true;
	int cnt = 0;
	for (int i = 0; i < size(clients); ++i)
		if (false == clients[i].alive)
			cnt++;
	if (size(clients) == cnt) {
		SERVER_DATA server_data;
		server_data.dataType = GAME_OVER;
		for (int i = 0; i < size(clients); ++i)
			send(server_data);
		send_rank();
	}
}

void send_login()
{
	SERVER_DATA server_data;
	server_data.dataType = LOGIN;
	for (int i = 0; i < size(clients); ++i) {
		send(server_data);		
	}
}

void send_gamestart()
{
	SERVER_DATA server_data;
	server_data.dataType = GAME_START;
	for (int i = 0; i < size(clients); ++i) {
		send(server_data);
	}
}

void send_rank()
{
	int ranked[3] = sort(clients.x);
	int rank = 1;
	SERVER_DATA server_data;
	server_data.dataType = GAME_RESULT;
	for (int i = 0; i < size(clients); ++i) {
		if (ranked[rank - 1] == clients[i].x) {
			server_data.rank = rank++;
			send(server_data);
			i = 0;
		}
	}
}

void send_object_move(int o_id)
{
	SERVER_DATA server_data;
	server_data.dataType = LOCATION;
	server_data.id = o_id;
	server_data.z += dz;
	for (int i = 0; i < size(clients); ++i)
		send(server_data);
}

void send_player_move(CLIENT_DATA client_data)
{
	SERVER_DATA server_data;
	server_data.dataType = LOCATION;
	switch (client_data.type)
	{
	case MOVE_FRONT:	
		server_data.z += dz;		
		break;
	case MOVE_BACK:
		server_data.z -= dz;
		break;
	case MOVE_RIGHT:
		server_data.x += dx;
		break;
	case MOVE_LEFT:
		server_data.x -= dx;
		break;
	}
	server_data.id = client_data.id;
	for (int i = 0; i < size(clients); ++i)
		send(server_data);
}

void TIme_Thread()
{
	server_Data.type = TIME;
	server_Data.time = 100;
	while (true) {
		for (모든 유저)
			sned(server_Data);
		server_Data.time--;
	}
}

void Coll_check(clients user, OBJ obj)
{
	if (true == Coll(user, obj) && user.alive) {
		SERVER_DATA server_data;
		server_data.dataType = GAME_OVER;
		for (int i = 0; i < size(clients); ++i)
			send(server_data);
	}
}