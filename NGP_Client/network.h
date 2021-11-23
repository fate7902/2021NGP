#pragma once
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