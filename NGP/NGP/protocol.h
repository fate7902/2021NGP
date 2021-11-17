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

// LOCATION 하위 TYPE
constexpr unsigned char PLAYER		=	0x21;
constexpr unsigned char OBJECT		=	0x22;

#pragma pack(push,1)
// DATA 형식
struct SERVER_DATA {
	unsigned char dataType;
	unsigned char objectType;
	unsigned short id;
	unsigned short rank;
	int x, y, z;
	int time;
};

// CLIENT가 보내는 DATA 형식
struct CLIENT_DATA {
	unsigned char type;
	unsigned short id;
};

// CLIENT INFO
struct CLIENT_INFO {
	unsigned short id;
	bool alive;
	int x, y, z;
};
#pragma pack(pop)