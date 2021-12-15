#pragma once
#define SERVERIP "127.0.0.1"
#define BUFSIZE 512
#define GOALLINE 100
constexpr int SERVERPORT = 9000;

// CLIENT DATA TYPE
constexpr unsigned char MOVE_FRONT = 0x01;
constexpr unsigned char MOVE_BACK = 0x02;
constexpr unsigned char MOVE_RIGHT = 0x03;
constexpr unsigned char MOVE_LEFT = 0x04;
constexpr unsigned char LOGOUT = 0x05;

// SERVER DATA TYPE
constexpr unsigned char LOCATION = 0x11;
constexpr unsigned char LOGIN = 0x12;
constexpr unsigned char GAME_START = 0x13;
constexpr unsigned char GAME_OVER = 0x14;
constexpr unsigned char GAME_CLEAR = 0x15;
constexpr unsigned char TIME = 0x16;
constexpr unsigned char REJECT = 0x17;
constexpr unsigned char RESTART = 0x18;

// LOCATION 하위 SUBDATATYPE
constexpr unsigned char PLAYER = 0x21;
constexpr unsigned char OBJECT = 0x22;

// LOGIN 하위 SUBDATATYPE
constexpr unsigned char SELF = 0x31;
constexpr unsigned char OTHER = 0x32;

// OBJECT DATA TYPE
constexpr unsigned char BOSS = 0x41;
constexpr unsigned char TRACKER = 0x42;
constexpr unsigned char BALL = 0x43;
constexpr unsigned char BULLDOZER = 0x44;

enum STATE { NONE, FULL };
#pragma pack(push,1)
// CLIENT가 보내는 DATA 형식
struct CLIENT_DATA {
	unsigned char type;
	unsigned short id;
};

// CLIENT INFO
struct CLIENT_INFO {
	SOCKET sock;
	STATE state;
	unsigned short id;
	bool alive;
	float x, y, z;
};

struct OBJECT_INFO {
	unsigned char objectType;
	bool moving;
	unsigned short line;
	unsigned short id;
	float x, y, z;
};

// DATA 형식
struct SERVER_DATA {
	unsigned char dataType;
	unsigned char subDataType;
    OBJECT_INFO objectInfo;	
	unsigned short id;
	float x, y, z;
	int time;
};
#pragma pack(pop)