#include <iostream>
#include <winsock2.h>
#include <windows.h>
#include <thread>
#include <atomic>
#include <vector>
#include <mutex>
#include "Select_Enemy.h"
#include "Market.h"
#pragma comment(lib, "ws2_32.lib")
using namespace std;

#define MAX_CLIENT  1024
#define MAX_NIKELEN 64
#define MAX_ROOM 8
#define BUFF_LEN 64
#define MAX_ROOMMEMBER 4

#define NONE_ROOM -1

static atomic<int> nextRoom; //存放所有空房间中序号最小的一个
static atomic<int> roomMember[MAX_ROOM][MAX_ROOMMEMBER]; //存放房间内玩家的序号
static atomic<int> roomCondition[MAX_ROOM]; //房间未使用，为0，如果在使用，则为房间内人数
static atomic<bool> isStart[MAX_ROOM];
static atomic<int> numofClient;
static atomic<int> numofRoom;
static atomic<int> livePeople[MAX_ROOM];
mutex myLock;

struct Client
{
	SOCKET clientSocket;
	char nikename[MAX_NIKELEN];
	int cliCondition = -1; //表示该客户的状态，-1表示已断开连接，-2表示在大厅，其他数字表示所在房间号
	int isAlive = 0;
	char myBoard[128] = { 0 };
	int fightCondition = 0; //1表示正在战斗，0表示正在购买
};

class Room
{
private:
	char buff[BUFF_LEN * 16] = { 0 };
	char roomersName[MAX_ROOM][MAX_ROOMMEMBER][MAX_NIKELEN]; //存放房间内玩家的名字
public:
	Room();
	char* getRooms();
	char* getNowRoom(int index);
	bool buildRoom(int index);
	bool intoRoom(int who, int roomNum);
	void startGame(int index);
};

Room::Room()
{
	nextRoom = 0;
	memset(roomMember, 0, sizeof(roomMember));
	memset(roomCondition, 0, sizeof(roomCondition));
	memset(isStart, 0, sizeof(isStart));
	memset(roomersName, 0, sizeof(roomersName));
	memset(livePeople, 0, sizeof(livePeople));
}

Client client[MAX_CLIENT];
Room* rooms = new Room;
vector<jthread> CliThreads;

bool Room::buildRoom(int index)
{
	if (nextRoom == NONE_ROOM)
		return false;
	numofRoom++;
	strcpy_s(roomersName[nextRoom][0], client[index].nikename);
	roomMember[nextRoom][0] = index;
	roomCondition[nextRoom] = 1;
	client[index].cliCondition = nextRoom;

	for (int i = 0; i < MAX_ROOM; i++)
		if (roomCondition[i] == 0)
		{
			nextRoom = i;
			break;
			if (i == MAX_ROOM - 1)
				nextRoom = NONE_ROOM;
		}
	return true;
}

bool Room::intoRoom(int who, int roomNum)
{
	if (roomCondition[roomNum] == MAX_ROOMMEMBER)
		return false;
	strcpy_s(roomersName[roomNum][roomCondition[roomNum]], client[who].nikename);
	roomMember[roomNum][roomCondition[roomNum]] = who;
	roomCondition[roomNum]++;
	client[who].cliCondition = roomNum;
	return true;
}

char* Room::getRooms()
{
	memset(buff, 0, sizeof(buff));
	int cnt = 0;
	for(int i = 0; i < MAX_ROOM; i++)
		if (roomCondition[i] && (!isStart[i]))
			cnt++;
	buff[0] = cnt + '0'; //第一个字符表示有多少个房间正在使用
	buff[1] = 0;
	for (int i = 0; i < MAX_ROOM; i++)
		if (roomCondition[i] && (!isStart[i]))
		{
			char tmp[MAX_NIKELEN * 2] = { 0 };
			tmp[0] = '#'; tmp[1] = i + '0';
			strcat(tmp, roomersName[i][0]);
			strcat(buff, tmp);
			strcat(buff, "#");
		}
	cout << buff << endl;
	return buff; //例如"3#1guolinghao##2yg##5chl#"表示共三个房间，各个房间的房主是#...#，房间序号为#后的数字
}

// 返回当前房间内的玩家信息
char* Room::getNowRoom(int index)
{
	memset(buff, 0, sizeof(buff));
	buff[0] = roomCondition[index] + '0'; //第一个字符表示房间内有多少人
	buff[1] = 0;
	for (int i = 0; i < roomCondition[index]; i++)
	{
		char tmp[MAX_NIKELEN * 2] = { 0 };
		tmp[0] = '#'; tmp[1] = 0;
		strcat(tmp, roomersName[index][i]);
		strcat(buff, tmp);
		strcat(buff, "#");
	}
	cout << "发送给" << index << "号客户端房间内信息：" << buff << endl;
	return buff; // "2#guolinghao##yg#"
}

vector<select_enemy> selects;
vector<Market> markets;

void Room::startGame(int index)
{
	int tmp = roomCondition[index];
	cout << "当前房间人数为：" << tmp << endl;
	livePeople[index] = tmp;
	isStart[index] = 1;
	selects[index].init(roomCondition[index]);
	markets[index].init(roomCondition[index]);
	for (int i = 0; i < roomCondition[index]; i++)
	{
		send(client[roomMember[index][i]].clientSocket, "GameStart", strlen("GameStart"), NULL);
		cout << "发送给" << i << "开始游戏" << endl;
		client[roomMember[index][i]].isAlive = 1;
	}
}

void ReceiveNikename(int index)
{
	int r;
	char buff[MAX_NIKELEN - 1];
	r = recv(client[index].clientSocket, buff, MAX_NIKELEN - 2, NULL);
	if (r > 0)
	{
		buff[r] = 0;//添加结束符号
		if (buff[0] == '*')
		{
			strcpy_s(client[index].nikename, buff + 1);
			cout << index << "号客户端的昵称为：" << buff << endl;
		}
	}
}

void refresh(int index)
{
	if (client[index].cliCondition == -2)
	{
		cout << "发送给" << index << "号客户端大厅信息：";
		send(client[index].clientSocket, rooms->getRooms(), strlen(rooms->getRooms()), NULL);
	}
	else
	{
		send(client[index].clientSocket, rooms->getNowRoom(client[index].cliCondition), strlen(rooms->getNowRoom(client[index].cliCondition)), NULL);
	}
}

void nextProbableEnemy(int gameRoom)
{
	string s = selects[gameRoom].nextProbableEnemy();
	cout << "原始下一轮对手为：" << s << endl;
	string probableEnemy[5];
	for (int i = 0; i < livePeople[gameRoom]; i++)
	{
		for (int j = i * (livePeople[gameRoom] - 1); j < s.size() && j < i * (livePeople[gameRoom] - 1) + livePeople[gameRoom] - 1; j++)
			probableEnemy[i] += s[j];
		cout << "probableEnemy[" << i << "]" << probableEnemy[i] << endl;
	}
	for (int i = 0; i < roomCondition[gameRoom]; i++)
	{
		if (client[roomMember[gameRoom][i]].isAlive == 0)
			continue;
		char tmp[BUFF_LEN] = { 0 };
		strcpy_s(tmp, probableEnemy[i].c_str());
		cout << "发送给第" << roomMember[gameRoom][i] << "号下一轮对手：" << tmp << endl;
		send(client[roomMember[gameRoom][i]].clientSocket, tmp, strlen(tmp), NULL);
	}
}

void readyToFight(int gameRoom)
{
	for(int i = 0; i < roomCondition[gameRoom]; i++)
		if(client[roomMember[gameRoom][i]].isAlive)
			send(client[roomMember[gameRoom][i]].clientSocket, "readyToFight", strlen("readyToFight"), NULL);
}

void sendBackBoard(int gameRoom)
{
	while (1)
	{
		bool tag = 0;
		for (int i = 0; i < roomCondition[gameRoom]; i++)
			if (client[roomMember[gameRoom][i]].isAlive)
				if (client[roomMember[gameRoom][i]].fightCondition == 0)
					tag = 1;
		if (tag)
			continue;
		break;
	}
	string s = selects[gameRoom].battle();
	cout << "真实对战顺序为：" << s << endl;
	int a = s[0] - '0' - 1, b = s[1] - '0' - 1;
	send(client[roomMember[gameRoom][a]].clientSocket, client[roomMember[gameRoom][b]].myBoard, strlen(client[roomMember[gameRoom][b]].myBoard), NULL);
	

	cout << "给第" << roomMember[gameRoom][a] << "号客户端发送：" << client[roomMember[gameRoom][b]].myBoard << endl;
	
	
	send(client[roomMember[gameRoom][b]].clientSocket, client[roomMember[gameRoom][a]].myBoard, strlen(client[roomMember[gameRoom][a]].myBoard), NULL);
	if (livePeople[gameRoom] == 2)
	{
		memset(client[roomMember[gameRoom][a]].myBoard, 0, sizeof(client[roomMember[gameRoom][a]].myBoard));
		memset(client[roomMember[gameRoom][b]].myBoard, 0, sizeof(client[roomMember[gameRoom][b]].myBoard));
		return;
	}
	memset(client[roomMember[gameRoom][a]].myBoard, 0, sizeof(client[roomMember[gameRoom][a]].myBoard));
	memset(client[roomMember[gameRoom][b]].myBoard, 0, sizeof(client[roomMember[gameRoom][b]].myBoard));
	a = s[2] - '0' - 1; b = s[3] - '0' - 1;
	send(client[roomMember[gameRoom][a]].clientSocket, client[roomMember[gameRoom][b]].myBoard, strlen(client[roomMember[gameRoom][b]].myBoard), NULL);
	if (livePeople[gameRoom] == 3)
	{
		memset(client[roomMember[gameRoom][a]].myBoard, 0, sizeof(client[roomMember[gameRoom][a]].myBoard));
		return;
	}
	send(client[roomMember[gameRoom][b]].clientSocket, client[roomMember[gameRoom][a]].myBoard, strlen(client[roomMember[gameRoom][a]].myBoard), NULL);
	memset(client[roomMember[gameRoom][b]].myBoard, 0, sizeof(client[roomMember[gameRoom][b]].myBoard));
}

void imDead(int index)
{
	livePeople[client[index].cliCondition]--;
	client[index].isAlive = 0;

	//商城的
}

void fightOver(int index)
{
	myLock.lock();
	client[index].fightCondition = 0;
	myLock.unlock();
	if (roomMember[client[index].cliCondition][0] == index) //如果这个人是房主
	{
		while (1)
		{
			bool tag = 0;
			for (int i = 0; i < roomCondition[client[index].cliCondition]; i++)
				if (client[roomMember[client[index].cliCondition][i]].isAlive)
					if (client[roomMember[client[index].cliCondition][i]].fightCondition == 1)
						tag = 1;
			if (tag)
				continue;
			break;
		}
		for(int i = 0; i < roomCondition[client[index].cliCondition]; i++)
			if (client[roomMember[client[index].cliCondition][i]].isAlive)
				send(client[roomMember[client[index].cliCondition][i]].clientSocket, "goBuy", strlen("goBuy"), NULL);
	}
}

void getMarket(int i, int level)
{
	myLock.lock();
	string s = markets[client[i].cliCondition].GetCard(level);
	myLock.unlock();
	send(client[i].clientSocket, s.c_str(), strlen(s.c_str()), NULL);
}

void cardBack(int i, char ch, int num)
{
	myLock.lock();
	markets[client[i].cliCondition].back(ch, num);
	myLock.unlock();
}

/*
* 输入昵称
# 请求刷新
@ 创建房间
! 加入房间
& 开始游戏
= 获得可能对战信息
+ 获得对战信息
$ 房主发送完自己的棋盘后发送该信号，让服务器进行分发
- 放回卡片
% 棋盘中的信息
d 表示死了
m 获取商城
*/
void listenClient(int i)
{
	while (1)
	{
		if (client[i].cliCondition == -1)
			continue;
		int r;
		char buff[BUFF_LEN];
		r = recv(client[i].clientSocket, buff, BUFF_LEN - 1, NULL);
		if (r > 0)
		{
			buff[r] = 0;//添加结束符号
			cout << "收到" << i << "号客户端";

			if (buff[0] == '#') //刷新请求
			{
				cout << "刷新请求\n";
				refresh(i);
			}
			//建房请求
			else if (buff[0] == '@')
			{
				cout << "建房请求\n";
				char tmp[10] = { 0 };
				if (rooms->buildRoom(i))
				{
					tmp[0] = 'y'; tmp[1] = client[i].cliCondition + '0'; tmp[2] = 0;
					send(client[i].clientSocket, tmp, strlen(tmp), NULL);
				}
				else
				{
					tmp[0] = 'n'; tmp[1] = 0;
					send(client[i].clientSocket, tmp, strlen(tmp), NULL);
				}
			}
			//进房请求
			else if (buff[0] == '!')
			{
				cout << "进房请求\n";
				int target = buff[1] - '0';
				if (rooms->intoRoom(i, target))
					send(client[i].clientSocket, "y", strlen("y"), NULL);
				else
					send(client[i].clientSocket, "n", strlen("n"), NULL);
			}
			//开始游戏
			else if (buff[0] == '&')
			{
				cout << "开始游戏请求\n";
				if (buff[1] == '&')
					rooms->startGame(client[i].cliCondition);
			}
			//房主发送，会将可能对战的信息发送给各玩家
			else if (buff[0] == '=')
			{
				cout << "对战信息发送\n";
				nextProbableEnemy(client[i].cliCondition);
			}
			//房主发送，所有玩家都收到readyToFight之后再发送棋盘信息
			else if (buff[0] == '+')
			{
				cout << "棋盘信息发送\n";
				readyToFight(client[i].cliCondition);
				client[i].fightCondition = 1;
			}
			//每个玩家发送棋盘信息
			else if (buff[0] == '%')
			{
				cout << "玩家发送棋盘信息\n";
				myLock.lock();
				memset(client[i].myBoard, 0, sizeof(client[i].myBoard));
				client[i].fightCondition = 1;
				strcpy(client[i].myBoard, buff + 1);
				//Sleep(1000);
				//cout << "收到" << i << "号玩家的棋盘信息，为" << client[i].myBoard << endl;
				myLock.unlock();
			}
			//将自己对手的棋盘发送回来
			else if (buff[0] == '$')
			{
				cout << "接收棋盘信息\n";
				sendBackBoard(client[i].cliCondition);
			}
			//战斗结束之后发送，收到goBuy之后再进入下一回合的购买
			else if (buff[0] == 'd') //一个d表示一回合结束，两个d表示自己结束并且死亡
			{
				cout << "回合结束\n";
				if (buff[1] == 'd')
					imDead(i);
				fightOver(i);
			}
			//刷新商城
			else if (buff[0] == 'm')
			{
				cout << "刷新商城\n";
				int level = buff[1] - '0';
				getMarket(i, level);
			}
			//送回卡片
			else if (buff[0] == '-')
			{
				cout << "送回卡片\n";
				cardBack(i, buff[1], buff[2] - '0');
			}
		}
	}
}

void test();

int main() 
{
	for (int i = 0; i < 8; i++)
	{
		selects.push_back(select_enemy());
		markets.push_back(Market());
	}
	//1. 确定网络协议版本		
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData))//载入失败
	{
		//输出出错信息
		cout << "载入socket库失败\n";
		system("pause");
		return -1;
	}
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) 
	{
		printf("确定网络协议版本失败：%d\n",
			GetLastError());
		system("pause");
		return -1;
	}
	printf("确定网络协议版本成功\n");

	//2. 创建socket		
	SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSocket == SOCKET_ERROR) 
	{
		printf("创建socket失败：%d\n",
			GetLastError());
		//清理协议版本信息
		WSACleanup();
		system("pause");
		return -1;
	}
	printf("创建socket成功\n");
	//3. 确定服务器协议地址簇	
	SOCKADDR_IN  addr = { 0 };
	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr = inet_addr("100.81.181.249");
	addr.sin_port = htons(13526);

	//4. 绑定
	if (bind(serverSocket, (sockaddr*)&addr, sizeof addr) == SOCKET_ERROR) 
	{
		printf("绑定失败：%d\n",
			GetLastError());
		//关闭socket		
		closesocket(serverSocket);
		//清理协议版本信息
		WSACleanup();
		system("pause");
		return -1;
	}
	printf("绑定成功\n");
	//thread testThread(test);
	//5. 监听
	if (listen(serverSocket, 100) == SOCKET_ERROR) 
	{
		printf("监听失败：%d\n",
			GetLastError());
		//关闭socket		
		closesocket(serverSocket);
		//清理协议版本信息
		WSACleanup();
		system("pause");
		return -1;
	}
	printf("监听成功\n");
	//6. 接受客户端连接	
	for (int i = 1; i < MAX_CLIENT; i++) 
	{
		client[i].clientSocket = accept(serverSocket, (sockaddr*)NULL, NULL);
		if (client[i].clientSocket == SOCKET_ERROR)
		{
			printf("服务器崩溃：%d\n",
				GetLastError());
			//关闭socket		
			closesocket(serverSocket);
			//清理协议版本信息
			WSACleanup();
			system("pause");
			return -1;
		}
		cout << i << "号客户端已连接\n";
		numofClient++;
		client[i].cliCondition = -2;
		ReceiveNikename(i);
		//send(client[i].clientSocket, rooms->getRooms(), strlen(rooms->getRooms()), NULL);
		CliThreads.push_back(jthread(listenClient, i));
	}

	//8. 关闭socket		
	closesocket(serverSocket);
	//9. 清理协议版本信息
	WSACleanup();
	delete rooms;

	while (1);
	return 0;
}

void test()
{
	while (1)
	{
		cout << rooms->getRooms() << endl;
		cout << rooms->getNowRoom(0) << endl;
		cout << endl;
		Sleep(5000);
	}
}