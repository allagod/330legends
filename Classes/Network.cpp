#include "Network.h"
#include <thread>
#include <cstring>
using namespace std;

Network::Network()
{
	init();
}

Network* Network::getInstance()
{
	static Network local_instance;
	return &local_instance;
}

void Network::init()
{
	//1. 确定网络协议版本		
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData))//载入失败
		//输出出错信息
		cout << "载入socket库失败\n";
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		printf("确定网络协议版本失败：%d\n",
			GetLastError());
	}
	printf("确定网络协议版本成功\n");

	//2. 创建socket		
	serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSocket == SOCKET_ERROR)
	{
		printf("创建socket失败：%d\n",
			GetLastError());
		//清理协议版本信息
		WSACleanup();
	}
	printf("创建socket成功\n");
	//3. 确定服务器协议地址簇	
	SOCKADDR_IN  addr = { 0 };
	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr = inet_addr("100.81.181.249");
	addr.sin_port = htons(13526);

	//4. 连接服务器
	if (connect(serverSocket, (sockaddr*)&addr, sizeof addr) == SOCKET_ERROR)
	{
		printf("连接服务器失败：%d\n",
			GetLastError());
		//关闭socket		
		closesocket(serverSocket);
		//清理协议版本信息
		WSACleanup();
	}
	printf("连接服务器成功\n");
}

Network::~Network()
{
	//8. 关闭socket		
	closesocket(serverSocket);
	//9. 清理协议版本信息
	WSACleanup();
}

void Network::sendNikename(const char a[])
{
	strcpy_s(nikename, a);
	char tmp[MAX_NIKELEN * 4] = "*";
	strcat_s(tmp, a);
	send(serverSocket, tmp, strlen(tmp), NULL);
}

char* Network::refresh()
{
	send(serverSocket, "#", strlen("#"), NULL);
	int r;
	memset(buff, 0, sizeof(buff));
	r = recv(serverSocket, buff, BUFF_LEN - 1, NULL);
	if (r > 0)
	{
		buff[r] = 0;//添加结束符号
		return buff;
	}
}

int Network::buildRoom()
{
	send(serverSocket, "@", strlen("@"), NULL);
	int r;
	memset(buff, 0, sizeof(buff));
	r = recv(serverSocket, buff, BUFF_LEN - 1, NULL);
	if (r > 0)
	{
		buff[r] = 0;//添加结束符号
		if (buff[0] == 'n')
			return -1;
		if (buff[0] == 'y')
			return buff[1] - '0';
	}
}

bool Network::intoRoom(int index)
{
	char tmp[5];
	tmp[0] = '!'; tmp[1] = index + '0'; tmp[2] = 0;
	send(serverSocket, tmp, strlen(tmp), NULL);
	int r;
	memset(buff, 0, sizeof(buff));
	r = recv(serverSocket, buff, BUFF_LEN - 1, NULL);
	if (r > 0)
	{
		buff[r] = 0;//添加结束符号
		if (buff[0] == 'y')
			return true;
		return false;
	}

}

void Network::wait()
{
	int r;
	memset(buff, 0, sizeof(buff));
	char gameStart[] = "GameStart";
	while (1)
	{
		r = recv(serverSocket, buff, BUFF_LEN - 1, NULL);
		if (r > 0)
			if (strcmp(gameStart, buff) == 0)
				break;
	}
	send(serverSocket, "&", strlen("&"), NULL);
}

void Network::startGame()
{
	isHost = 1;
	send(serverSocket, "&&", strlen("&&"), NULL);
	int r;
	while (1)
	{
		memset(buff, 0, sizeof(buff));
		r = recv(serverSocket, buff, BUFF_LEN - 1, NULL);
		if (r > 0)
		{
			//cout << "startGameBuff:" << buff << endl;
			if (strcmp(buff, "GameStart") == 0)
				break;
		}
	}
}

char* Network::getNextEnemy()
{
	//cout << "successingetNextEnemy\n";
	if (isHost)
	{
		//cout << "getNextEnemySend:" << "=" << endl;
		send(serverSocket, "=", strlen("="), NULL);
	}
	memset(buff, 0, sizeof(buff));
	int r;
	while (1)
	{
		r = recv(serverSocket, buff, BUFF_LEN - 1, NULL);
		if (r > 0)
			return buff;
	}
}

char* Network::getFightBoard(const char myBoard[])
{
	if (isHost)
		send(serverSocket, "+", strlen("+"), NULL);
	int r;
	while (1)
	{
		memset(buff, 0, sizeof(buff));
		r = recv(serverSocket, buff, BUFF_LEN - 1, NULL);
		if (r > 0)
			if (strcmp(buff, "readyToFight") == 0)
				break;
	}
	send(serverSocket, myBoard, strlen(myBoard), NULL);
	if (isHost)
		send(serverSocket, "$", strlen("$"), NULL);
	while (1)
	{
		memset(buff, 0, sizeof(buff));
		r = recv(serverSocket, buff, BUFF_LEN - 1, NULL);
		if (r > 0)
			break;
	}
	cout << "getFightBoard中的返回buff为：" << buff << endl;
	return buff;
}

void Network::fightOver(int tag)
{
	memset(buff, 0, sizeof(buff));
	buff[0] = 'd';
	if (tag)
		buff[1] = 'd';
	send(serverSocket, buff, strlen(buff), NULL);
	int r;
	while (1)
	{
		memset(buff, 0, sizeof(buff));
		r = recv(serverSocket, buff, BUFF_LEN - 1, NULL);
		if (r > 0)
			if (strcmp(buff, "goBuy") == 0)
				break;
	}
}

char* Network::getCard(int level)
{
	memset(buff, 0, sizeof(buff));
	buff[0] = 'm'; buff[1] = level + '0'; buff[2] = 0;
	send(serverSocket, buff, strlen(buff), NULL);
	int r;
	while (1)
	{
		memset(buff, 0, sizeof(buff));
		r = recv(serverSocket, buff, BUFF_LEN - 1, NULL);
		if (r > 0)
			break;
	}
	return buff;
}

void Network::cardBack(char ch, int num)
{
	memset(buff, 0, sizeof(buff));
	buff[0] = '-'; buff[1] = ch; buff[2] = num + '0'; buff[3] = 0;
	send(serverSocket, buff, strlen(buff), NULL);
}