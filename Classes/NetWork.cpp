#include <stdio.h>
#include <iostream>
#include <winsock2.h>
#include <thread>
#pragma comment(lib, "ws2_32.lib")

using namespace std;
#define MAX_NIKELEN 64
#define BUFF_LEN 64
#define MAX_NIKELEN 64

class NetWork
{
private:
	char nikename[MAX_NIKELEN];
	SOCKET serverSocket;
	char buff[BUFF_LEN];
public:
	NetWork();
	~NetWork();
	void sendNikename(char nikename[]);
	char* refresh();
	char* buildRoom();
	char* intoRoom(int index);
	void wait();
	void startGame();
};

NetWork::NetWork()
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
	addr.sin_addr.S_un.S_addr = inet_addr("100.79.182.197");
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

NetWork::~NetWork()
{
	//8. 关闭socket		
	closesocket(serverSocket);
	//9. 清理协议版本信息
	WSACleanup();
}

void NetWork::sendNikename(char a[])
{
	cout << "a:" << a << endl;
	strcpy_s(nikename, a);
	char tmp[MAX_NIKELEN + 1] = "*";
	strcat(tmp, a);
	cout << "send:" << tmp << endl;
	send(serverSocket, tmp, strlen(tmp), NULL);
}

char* NetWork::refresh()
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

char* NetWork::buildRoom()
{
	send(serverSocket, "@", strlen("@"), NULL);
	int r;
	memset(buff, 0, sizeof(buff));
	r = recv(serverSocket, buff, BUFF_LEN - 1, NULL);
	if (r > 0)
	{
		buff[r] = 0;//添加结束符号
		return buff;
	}
}

char* NetWork::intoRoom(int index)
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
		return buff;
	}
}

void NetWork::wait()
{
	int r;
	char buff[BUFF_LEN], gameStart[] = "GameStart";
	while (1)
	{
		r = recv(serverSocket, buff, BUFF_LEN - 1, NULL);
		if (r > 0)
			if (strcmp(gameStart, buff) == 0)
				break;
	}
	send(serverSocket, "&", strlen("&"), NULL);
}

void NetWork::startGame()
{
	send(serverSocket, "&&", strlen("&&"), NULL);
}

NetWork test;//初始化


void update()
{
	while (1)
	{
		char buff[BUFF_LEN];
		strcpy_s(buff, test.refresh());
		cout << buff << endl;
		Sleep(1000);
	}
}

int main()
{
	char tmpNikename[MAX_NIKELEN];
	char buff[BUFF_LEN];
	cout << "请输入昵称\n";
	cin >> tmpNikename;



	test.sendNikename(tmpNikename);//发送昵称



	thread newthread(update);
	int n;
	cin >> n;
	if (n == 1)//如果要创建房间
	{
		strcpy_s(buff, test.buildRoom());//创建


		cout << buff << endl;
		int m;
		cin >> m;



		test.startGame();//开始游戏
	}
	else //如果要假如房间
	{
		int kk; // 加入的房间号
		cin >> kk;


		strcpy_s(buff, test.intoRoom(kk));//加入


		cout << buff << endl;



		test.wait();//等待开始


	}
	return 0;
}