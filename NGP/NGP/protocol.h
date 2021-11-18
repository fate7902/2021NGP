#pragma once
// CLIENT DATA TYPE
constexpr unsigned char MOVE_FRONT	=	0x01;
constexpr unsigned char MOVE_BACK	=	0x02;
constexpr unsigned char MOVE_RIGHT	=	0x03;
constexpr unsigned char MOVE_LEFT	=	0x04;

// SERVER DATA TYPE
constexpr unsigned char LOCATION	=	0x11;
constexpr unsigned char LOGIN		=	0x12;
constexpr unsigned char GAME_START	=	0x13;
constexpr unsigned char GAME_OVER	=	0x14;
constexpr unsigned char GAME_RESULT =	0x15;
constexpr unsigned char TIME		=	0x16;

// LOCATION ���� SUBDATATYPE
constexpr unsigned char PLAYER		=	0x21;
constexpr unsigned char OBJECT		=	0x22;

// LOGIN ���� SUBDATATYPE
constexpr unsigned char SELF		=	0x31;
constexpr unsigned char OTHER		=	0x32;

#pragma pack(push,1)
// DATA ����
struct SERVER_DATA {
	unsigned char dataType;
	unsigned char subDataType;
	unsigned short id;
	bool mission_result;
	int x, y, z;
	int time;
};

// CLIENT�� ������ DATA ����
struct CLIENT_DATA {
	unsigned char type;
	unsigned short id;
};

// CLIENT INFO
struct CLIENT_INFO {
	SOCKET sock;
	unsigned short id;
	bool alive;
	int x, y, z;
};
#pragma pack(pop)