#include "NetWork"

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
	strcpy_s(nikename, a);
	char tmp[MAX_NIKELEN * 4] = "*";
	strcat(tmp, a);
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

int NetWork::buildRoom()
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

bool NetWork::intoRoom(int index)
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
