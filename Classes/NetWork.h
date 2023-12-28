#include <iostream>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#define MAX_NIKELEN 64
#define BUFF_LEN 64
#define MAX_NIKELEN 64

class Network
{
private:
	Network();
	Network(const Network&) = delete;
	Network& operator =(const Network&) = delete;


	char nikename[MAX_NIKELEN]; //存储玩家昵称
	SOCKET serverSocket; //不用管
	char buff[BUFF_LEN];//不用管
	int isHost = 0; //是否是房主
	//初始化函数v
	void init();
public:
	~Network();
	static Network* getInstance();

	//输入昵称后用这个函数把昵称发给服务器
	void sendNikename(char nikename[]);

	//如果在大厅，返回各个房间的信息
	//例如"3#1guolinghao##2yg##5chl#"表示共三个房间，各个房间的房主是#...#，房间序号为#后的数字

	//如果在房间内，返回房间内的信息
	//"2#guolinghao##yg#"第一个字符表示房间内有多少人，#..#中间是房间内玩家昵称
	char* refresh();

	//创建房间，如果成功返回创建房间的序号，失败返回-1
	int buildRoom();

	//进入index房间，如果成功返回true，失败返回false
	bool intoRoom(int index);

	//非房主成员在进入房间后调用该函数等待开始
	void wait();

	//房主开始游戏调用该函数
	void startGame();

	//返回下一局可能对战谁
	char* getNextEnemy();

	//传入自己的棋盘，返回对手的棋盘
	char* getFightBoard(const char myBoard[]);

	//传入是否死亡，0代表没死，1代表死亡，每次战斗结束都要调用一下这个函数
	void fightOver(int die);

	//返回商城给的卡
	char* getCard(int level);

	//将卡退回去
	void cardBack(char ch, int num);
};