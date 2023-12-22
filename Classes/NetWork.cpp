#include "NetWork"

NetWork::NetWork()
{
	//1. ȷ������Э��汾		
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData))//����ʧ��
		//���������Ϣ
		cout << "����socket��ʧ��\n";
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		printf("ȷ������Э��汾ʧ�ܣ�%d\n",
			GetLastError());
	}
	printf("ȷ������Э��汾�ɹ�\n");

	//2. ����socket		
	serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSocket == SOCKET_ERROR)
	{
		printf("����socketʧ�ܣ�%d\n",
			GetLastError());
		//����Э��汾��Ϣ
		WSACleanup();
	}
	printf("����socket�ɹ�\n");
	//3. ȷ��������Э���ַ��	
	SOCKADDR_IN  addr = { 0 };
	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr = inet_addr("100.79.182.197");
	addr.sin_port = htons(13526);

	//4. ���ӷ�����
	if (connect(serverSocket, (sockaddr*)&addr, sizeof addr) == SOCKET_ERROR)
	{
		printf("���ӷ�����ʧ�ܣ�%d\n",
			GetLastError());
		//�ر�socket		
		closesocket(serverSocket);
		//����Э��汾��Ϣ
		WSACleanup();
	}
	printf("���ӷ������ɹ�\n");
}

NetWork::~NetWork()
{
	//8. �ر�socket		
	closesocket(serverSocket);
	//9. ����Э��汾��Ϣ
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
		buff[r] = 0;//��ӽ�������
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
		buff[r] = 0;//��ӽ�������
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
		buff[r] = 0;//��ӽ�������
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
