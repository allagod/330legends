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
		buff[r] = 0;//��ӽ�������
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
		buff[r] = 0;//��ӽ�������
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
		buff[r] = 0;//��ӽ�������
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

NetWork test;//��ʼ��


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
	cout << "�������ǳ�\n";
	cin >> tmpNikename;



	test.sendNikename(tmpNikename);//�����ǳ�



	thread newthread(update);
	int n;
	cin >> n;
	if (n == 1)//���Ҫ��������
	{
		strcpy_s(buff, test.buildRoom());//����


		cout << buff << endl;
		int m;
		cin >> m;



		test.startGame();//��ʼ��Ϸ
	}
	else //���Ҫ���緿��
	{
		int kk; // ����ķ����
		cin >> kk;


		strcpy_s(buff, test.intoRoom(kk));//����


		cout << buff << endl;



		test.wait();//�ȴ���ʼ


	}
	return 0;
}