#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#pragma comment(lib,"ws2_32")
#include <WinSock2.h>
#include <iostream>
#include <time.h>
#include <random>
#include "protocol.h"
using namespace std;

#define MAXTIME 100
#define INFINITE -9999
#define INITPOSX 0
#define INITPOSY 0
#define INITPOSZ 0
#define USERSIZE 0.5
#define OBJECTSIZE 1.5
#define OBJECTSIZE2 0.5
#define COUNTDOWN 10
#define OBJECTPOSX 5

CLIENT_INFO clientInfo[3];
// 0: 보스 1: 트래커 2, 3: 장애물
OBJECT_INFO objectInfo[4];

bool gameStart = false;
bool goal = false;
float dx = 0.12;
float dz = 0.12;

default_random_engine dre{ random_device{}() };
uniform_int_distribution<> uid{ BALL, BULLDOZER };
uniform_int_distribution<int> IntUid{ 1, 3 };

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
    // 전달할 유저의 정보 셋팅
    SERVER_DATA server_data;
    server_data.dataType = LOGIN;
    server_data.id = clientInfo[id].id;
    server_data.x = clientInfo[id].x;
    server_data.y = clientInfo[id].y;
    server_data.z = clientInfo[id].z;
    cout << "[" << server_data.id << "]번 클라 로그인" << endl;

    for (const auto& clients : clientInfo) {
        if (id == clients.id) {
            server_data.subDataType = SELF;
            send(clients.sock, (char*)&server_data, sizeof(SERVER_DATA), 0);
        }
        for (const auto& other : clientInfo) {
            if (clients.id != other.id) {
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

}

void SC_INIT() {
    gameStart = false;

    for (auto& clients : clientInfo) {
        clients.x = -3 + (float)(clients.id * 3);
        clients.y = INITPOSY;
        clients.z = INITPOSZ;
    }
    for (const auto& clients : clientInfo) {
        for (const auto& client : clientInfo) {
            SERVER_DATA server_data;
            server_data.dataType = LOCATION;
            server_data.subDataType = PLAYER;
            server_data.id = client.id;
            server_data.x = client.x;
            server_data.y = client.y;
            server_data.z = client.z;
            send(clients.sock, (char*)&server_data, sizeof(SERVER_DATA), 0);

            SERVER_DATA server_data2;
            server_data2.dataType = RESTART;
            server_data2.id = client.id;
            send(clients.sock, (char*)&server_data2, sizeof(SERVER_DATA), 0);
        }
    }

    for (int i = 0; i < 4; ++i) {
        objectInfo[i].id = i;
        objectInfo[i].objectType = NULL;
    }

    objectInfo[0].x = INITPOSX;
    objectInfo[0].y = INITPOSY;
    objectInfo[0].z = -20;

    objectInfo[1].x = INITPOSX;
    objectInfo[1].y = INITPOSY;
    objectInfo[1].z = 10;

    for (int i = 2; i < 4; ++i) {
        objectInfo[i].x = INFINITE;
        objectInfo[i].y = INITPOSY;
        objectInfo[i].z = INFINITE;
        objectInfo[i].moving = false;
    }

    for (const auto& clients : clientInfo) {
        SERVER_DATA server_data;
        server_data.dataType = LOCATION;
        server_data.subDataType = OBJECT;
        for (const auto& objects : objectInfo) {
            server_data.objectInfo = objects;
            send(clients.sock, (char*)&server_data, sizeof(SERVER_DATA), 0);
        }
    }
}

void SC_COLL(int num)
{
    SERVER_DATA server_data;
    server_data.dataType = GAME_OVER;
    server_data.id = num;
    clientInfo[num].alive = false;
    for (const auto& clients : clientInfo) {
        send(clients.sock, (char*)&server_data, sizeof(SERVER_DATA), 0);
    }
}

void GOAL_CHECK(CLIENT_INFO client_Info)
{
    if (client_Info.z <= -GOALLINE) {
        for (auto& clients : clientInfo) {
            SERVER_DATA server_data;
            if (clients.id == client_Info.id) {
                server_data.dataType = GAME_CLEAR;
            }
            else {
                server_data.dataType = GAME_OVER;
            }
            server_data.id = clients.id;
            send(clients.sock, (char*)&server_data, sizeof(SERVER_DATA), 0);
            clients.alive = false;
        }
        goal = true;
        SC_INIT();
    }
}

void COLL_CHECK(OBJECT_INFO object_info)
{
    for (int i = 0; i < 3; ++i) {
        if (clientInfo[i].alive) {
            if (object_info.id == 1) {
                if (object_info.z <= clientInfo[i].z) {
                    cout << "[" << clientInfo[i].id << "]번 클라 트래커 충돌" << endl;
                    SC_COLL(i);
                }
            }
            else if (object_info.id != 0 && object_info.id != 1) {

                if (object_info.objectType != NULL) {

                    if ((clientInfo[i].x - USERSIZE < object_info.x - OBJECTSIZE) &&
                        (clientInfo[i].x + USERSIZE > object_info.x - OBJECTSIZE) &&
                        (object_info.x - OBJECTSIZE < clientInfo[i].x + USERSIZE) &&
                        (clientInfo[i].x + USERSIZE < object_info.x + OBJECTSIZE) &&
                        (object_info.z - OBJECTSIZE2 <= clientInfo[i].z - USERSIZE) &&
                        (clientInfo[i].z - USERSIZE <= object_info.z + OBJECTSIZE2)) {
                        cout << "[" << clientInfo[i].id << "]번 클라 장애물 충돌" << endl;
                        SC_COLL(i);
                    }
                    else if ((object_info.x - OBJECTSIZE <= clientInfo[i].x - USERSIZE) &&
                        (clientInfo[i].x - USERSIZE <= object_info.x + OBJECTSIZE) &&
                        (object_info.x - OBJECTSIZE <= clientInfo[i].x + USERSIZE) &&
                        (clientInfo[i].x + USERSIZE <= object_info.x + OBJECTSIZE) &&
                        (object_info.z - OBJECTSIZE2 <= clientInfo[i].z - USERSIZE) &&
                        (clientInfo[i].z - USERSIZE <= object_info.z + OBJECTSIZE2)) {
                        cout << "[" << clientInfo[i].id << "]번 클라 장애물 충돌" << endl;
                        SC_COLL(i);
                    }
                    else if ((clientInfo[i].x - USERSIZE < object_info.x + OBJECTSIZE) &&
                        (object_info.x + OBJECTSIZE < clientInfo[i].x + USERSIZE) &&
                        (object_info.x - OBJECTSIZE < clientInfo[i].x - USERSIZE) &&
                        (clientInfo[i].x - USERSIZE < object_info.x + OBJECTSIZE) &&
                        (object_info.z - OBJECTSIZE2 <= clientInfo[i].z - USERSIZE) &&
                        (clientInfo[i].z - USERSIZE <= object_info.z + OBJECTSIZE2)) {
                        cout << "[" << clientInfo[i].id << "]번 클라 장애물 충돌" << endl;
                        SC_COLL(i);
                    }
                }
            }
        }
    }

    if (!clientInfo[0].alive && !clientInfo[1].alive && !clientInfo[2].alive) {
        SC_INIT();
    }

}

bool RESET_OBJECT(OBJECT_INFO object)
{
    if (objectInfo[1].z < object.z && true == object.moving) {
        return true;
    }

    return false;
}

void SC_SEND(CLIENT_DATA clientData)
{
    int id = clientData.id;
    if (LOGOUT == clientData.type) {
        clientInfo[id].state = NONE;
        clientInfo[id].alive = false;

        SERVER_DATA server_data;
        server_data.dataType = LOGOUT;
        server_data.id = id;
        for (const auto& clients : clientInfo) {
            send(clients.sock, (char*)&server_data, sizeof(server_data), 0);
        }
    }

    else if (GAME_START == clientData.type) {
        clientInfo[id].alive = true;

        clientInfo[id].x = -3 + (clientInfo[id].id * 3);
        clientInfo[id].y = 0;
        clientInfo[id].z = 0;

        // 3명의 유저가 접속하면 1초 기다렸다가 게임 시작
        if (clientInfo[0].alive && clientInfo[1].alive && clientInfo[2].alive) {
            int start_time = clock();
            int end_time = clock();
            while (end_time - start_time < 1000) {
                end_time = clock();
            }
            for (int i = 0; i < 4; ++i)
                objectInfo[i].id = i;

            objectInfo[0].x = 0;
            objectInfo[0].y = 0;
            objectInfo[0].z = -20;

            objectInfo[1].x = 0;
            objectInfo[1].y = 0;
            objectInfo[1].z = 10;

            for (int i = 2; i < 4; ++i) {
                objectInfo[i].x = -9999;
                objectInfo[i].y = 0;
                objectInfo[i].z = -9999;
                objectInfo[i].moving = false;
                objectInfo[i].objectType = NULL;
            }
            SC_GAMESTART();
        }
    }
    else {
        SERVER_DATA server_data;
        server_data.dataType = LOCATION;
        server_data.subDataType = PLAYER;
        switch (clientData.type) {
        case MOVE_FRONT:
            clientInfo[id].z -= dz;
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
        GOAL_CHECK(clientInfo[id]);
    }
}

DWORD WINAPI SC_TIME(LPVOID arg)
{
    // 전달할 유저의 정보 셋팅
    SERVER_DATA server_data;
    server_data.dataType = TIME;
    server_data.time = MAXTIME;      // 초기 시간 제한 값 세팅
    auto start = INFINITE;
    int send_time = MAXTIME + 1;
    while (true) {
        if (true == gameStart) {
            if (INFINITE == start)
                start = clock();
            if (server_data.time != send_time) {
                for (const auto& clients : clientInfo)
                    send(clients.sock, (char*)&server_data, sizeof(SERVER_DATA), 0);
            }
            if (0 == send_time) {
                server_data.dataType = GAME_OVER;
                for (const auto& clients : clientInfo) {
                    if (true == clients.alive)
                        send(clients.sock, (char*)&server_data, sizeof(SERVER_DATA), 0);
                }
            }
        }
        send_time = server_data.time;
        auto end = clock();
        server_data.time = MAXTIME - (int)((end - start) / 1000);
        if (true == goal && server_data.time > COUNTDOWN)
            server_data.time = COUNTDOWN;
    }
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
        if (SOCKET_ERROR == retval)
            break;

        SC_SEND(clientData);
    }
    closesocket(client_sock);
    return 0;
}

DWORD WINAPI SC_OBJECT_MOVE(LPVOID arg)
{
    auto start_wait = INFINITE;
    auto end_wait = INFINITE;
    int firstline = 0;
    SERVER_DATA server_data;
    server_data.dataType = LOCATION;
    server_data.subDataType = OBJECT;

    while (true) {
        if (true == gameStart) {
            if (start_wait == INFINITE) {
                start_wait = clock();
            }
            else {
                end_wait = clock();
                if (end_wait - start_wait > 100) {
                    for (auto& objects : objectInfo) {
                        switch (objects.id) {
                        case 0:   // BOSS
                            objects.objectType = BOSS;
                            objects.z -= 0.6;
                            break;
                        case 1:   // TRACKER
                            objects.objectType = TRACKER;
                            objects.z -= 0.6;
                            break;
                        default:
                            if (objects.objectType == NULL) {
                                if (2 == objects.id || 3 == objects.id) {
                                    if (2 == objects.id) {   // id:2 먼저 라인 정하기 
                                        firstline = IntUid(dre);
                                    }
                                    switch (firstline)
                                    {
                                    case 1:
                                        objects.line = objects.id;
                                        objects.x = OBJECTPOSX * (objects.id - 2);   //0  //4
                                        break;
                                    case 2:
                                        if (2 == objects.id) {
                                            objects.line = 1;
                                            objects.x = -OBJECTPOSX;      //-4
                                        }
                                        else {
                                            objects.line = objects.id;
                                            objects.x = OBJECTPOSX;         //4
                                        }
                                        break;
                                    case 3:
                                        objects.line = objects.id - 1;
                                        objects.x = -OBJECTPOSX * (3 - objects.id);  //-4   //0 
                                        break;
                                    }

                                    objects.z = objectInfo[0].z - 3;        //장애물 종류 정함 
                                    if (2 == objects.id)
                                        objects.objectType = BULLDOZER;
                                    else
                                        objects.objectType = BALL;
                                    objects.moving = true;
                                }
                            }
                            else {   // 장애물 종류 다 정해지면 움직여
                                if (true == objects.moving) {
                                    objects.z += 0.8;
                                }
                            }
                            break;
                        }
                        server_data.objectInfo = objects;

                        for (const auto& clients : clientInfo)
                            send(clients.sock, (char*)&server_data, sizeof(SERVER_DATA), 0);

                        COLL_CHECK(objects);

                        if (objects.id != 0 && objects.id != 1) {
                            if (true == RESET_OBJECT(objects)) {
                                objects.moving = false;
                                objects.objectType = NULL;
                                objects.x = INITPOSX;
                                objects.y = INITPOSY;
                                objects.z = objectInfo[0].z - 9999;
                                SERVER_DATA reset_server_data;
                                reset_server_data.dataType = LOCATION;
                                reset_server_data.subDataType = OBJECT;
                                reset_server_data.objectInfo = objects;
                                for (const auto& clients : clientInfo)
                                    send(clients.sock, (char*)&server_data, sizeof(SERVER_DATA), 0);
                            }
                        }
                    }
                    start_wait = INFINITE;
                }
            }
        }
        else {
            start_wait = INFINITE;
            end_wait = INFINITE;
            firstline = 0;
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
        cout << "소켓에러" << endl;

    SOCKADDR_IN serveraddr;
    ZeroMemory(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(SERVERPORT);
    retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
    if (SOCKET_ERROR == retval)
        cout << "주소할당에러" << endl;

    retval = listen(listen_sock, SOMAXCONN);
    if (SOCKET_ERROR == retval)
        cout << "연결요청에러" << endl;

    SOCKET client_sock;
    SOCKADDR_IN clientaddr;
    int addrlen;
    HANDLE hThread;
    int userCount = 0;

    // 시간 스레드, 오븍젝트 이동 스레드 생성
    hThread = CreateThread(NULL, 0, SC_TIME, NULL, 0, NULL);
    hThread = CreateThread(NULL, 0, SC_OBJECT_MOVE, NULL, 0, NULL);

    while (1) {
        addrlen = sizeof(clientaddr);
        client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
        if (INVALID_SOCKET == client_sock) {
            cout << "연결수락에러" << endl;
            break;
        }
        hThread = CreateThread(NULL, 0, S_RECV_PACKET, (LPVOID)client_sock, 0, NULL);
        if (NULL == hThread)
            closesocket(client_sock);
        else
            CloseHandle(hThread);

        int cnt = 0;
        bool enter = false;
        for (auto& clients : clientInfo) {
            if (FULL == clients.state) {
                cnt++;
                continue;
            }
            if (false == gameStart) {
                //// 접속한 유저 초기 정보 셋팅
                clients.sock = client_sock;
                clients.state = FULL;
                clients.id = cnt;

                clients.x = -3 + (cnt * 3);
                clients.y = 0;
                clients.z = 0;
                enter = true;
                SC_LOGIN(cnt);
            }
            break;
        }
        if (false == enter) {
            SERVER_DATA server_data;
            server_data.dataType = REJECT;
            send(client_sock, (char*)&server_data, sizeof(SERVER_DATA), 0);
        }
    }

    closesocket(listen_sock);

    WSACleanup();
    return 0;
}