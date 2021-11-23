#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib,"ws2_32")
#include <WinSock2.h>
#include <iostream>
#include "..\NGP\NGP\protocol.h"
using namespace std;

class Network {
	bool start = false;
public:
	void network();
};