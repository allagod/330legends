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

static atomic<int> nextRoom; //������пշ����������С��һ��
static atomic<int> roomMember[MAX_ROOM][MAX_ROOMMEMBER]; //��ŷ�������ҵ����
static atomic<int> roomCondition[MAX_ROOM]; //����δʹ�ã�Ϊ0�������ʹ�ã���Ϊ����������
static atomic<bool> isStart[MAX_ROOM];
static atomic<int> numofClient;
static atomic<int> numofRoom;
static atomic<int> livePeople[MAX_ROOM];
mutex myLock;

struct Client
{
	SOCKET clientSocket;
	char nikename[MAX_NIKELEN];
	int cliCondition = -1; //��ʾ�ÿͻ���״̬��-1��ʾ�ѶϿ����ӣ�-2��ʾ�ڴ������������ֱ�ʾ���ڷ����
	int isAlive = 0;
	char myBoard[128] = { 0 };
	int fightCondition = 0; //1��ʾ����ս����0��ʾ���ڹ���
};

class Room
{
private:
	char buff[BUFF_LEN * 16] = { 0 };
	char roomersName[MAX_ROOM][MAX_ROOMMEMBER][MAX_NIKELEN]; //��ŷ�������ҵ�����
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
	buff[0] = cnt + '0'; //��һ���ַ���ʾ�ж��ٸ���������ʹ��
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
	return buff; //����"3#1guolinghao##2yg##5chl#"��ʾ���������䣬��������ķ�����#...#���������Ϊ#�������
}

// ���ص�ǰ�����ڵ������Ϣ
char* Room::getNowRoom(int index)
{
	memset(buff, 0, sizeof(buff));
	buff[0] = roomCondition[index] + '0'; //��һ���ַ���ʾ�������ж�����
	buff[1] = 0;
	for (int i = 0; i < roomCondition[index]; i++)
	{
		char tmp[MAX_NIKELEN * 2] = { 0 };
		tmp[0] = '#'; tmp[1] = 0;
		strcat(tmp, roomersName[index][i]);
		strcat(buff, tmp);
		strcat(buff, "#");
	}
	cout << "���͸�" << index << "�ſͻ��˷�������Ϣ��" << buff << endl;
	return buff; // "2#guolinghao##yg#"
}

vector<select_enemy> selects;
vector<Market> markets;

void Room::startGame(int index)
{
	int tmp = roomCondition[index];
	cout << "��ǰ��������Ϊ��" << tmp << endl;
	livePeople[index] = tmp;
	isStart[index] = 1;
	selects[index].init(roomCondition[index]);
	markets[index].init(roomCondition[index]);
	for (int i = 0; i < roomCondition[index]; i++)
	{
		send(client[roomMember[index][i]].clientSocket, "GameStart", strlen("GameStart"), NULL);
		cout << "���͸�" << i << "��ʼ��Ϸ" << endl;
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
		buff[r] = 0;//��ӽ�������
		if (buff[0] == '*')
		{
			strcpy_s(client[index].nikename, buff + 1);
			cout << index << "�ſͻ��˵��ǳ�Ϊ��" << buff << endl;
		}
	}
}

void refresh(int index)
{
	if (client[index].cliCondition == -2)
	{
		cout << "���͸�" << index << "�ſͻ��˴�����Ϣ��";
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
	cout << "ԭʼ��һ�ֶ���Ϊ��" << s << endl;
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
		cout << "���͸���" << roomMember[gameRoom][i] << "����һ�ֶ��֣�" << tmp << endl;
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
	cout << "��ʵ��ս˳��Ϊ��" << s << endl;
	int a = s[0] - '0' - 1, b = s[1] - '0' - 1;
	send(client[roomMember[gameRoom][a]].clientSocket, client[roomMember[gameRoom][b]].myBoard, strlen(client[roomMember[gameRoom][b]].myBoard), NULL);
	

	cout << "����" << roomMember[gameRoom][a] << "�ſͻ��˷��ͣ�" << client[roomMember[gameRoom][b]].myBoard << endl;
	
	
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

	//�̳ǵ�
}

void fightOver(int index)
{
	myLock.lock();
	client[index].fightCondition = 0;
	myLock.unlock();
	if (roomMember[client[index].cliCondition][0] == index) //���������Ƿ���
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
* �����ǳ�
# ����ˢ��
@ ��������
! ���뷿��
& ��ʼ��Ϸ
= ��ÿ��ܶ�ս��Ϣ
+ ��ö�ս��Ϣ
$ �����������Լ������̺��͸��źţ��÷��������зַ�
- �Żؿ�Ƭ
% �����е���Ϣ
d ��ʾ����
m ��ȡ�̳�
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
			buff[r] = 0;//��ӽ�������
			cout << "�յ�" << i << "�ſͻ���";

			if (buff[0] == '#') //ˢ������
			{
				cout << "ˢ������\n";
				refresh(i);
			}
			//��������
			else if (buff[0] == '@')
			{
				cout << "��������\n";
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
			//��������
			else if (buff[0] == '!')
			{
				cout << "��������\n";
				int target = buff[1] - '0';
				if (rooms->intoRoom(i, target))
					send(client[i].clientSocket, "y", strlen("y"), NULL);
				else
					send(client[i].clientSocket, "n", strlen("n"), NULL);
			}
			//��ʼ��Ϸ
			else if (buff[0] == '&')
			{
				cout << "��ʼ��Ϸ����\n";
				if (buff[1] == '&')
					rooms->startGame(client[i].cliCondition);
			}
			//�������ͣ��Ὣ���ܶ�ս����Ϣ���͸������
			else if (buff[0] == '=')
			{
				cout << "��ս��Ϣ����\n";
				nextProbableEnemy(client[i].cliCondition);
			}
			//�������ͣ�������Ҷ��յ�readyToFight֮���ٷ���������Ϣ
			else if (buff[0] == '+')
			{
				cout << "������Ϣ����\n";
				readyToFight(client[i].cliCondition);
				client[i].fightCondition = 1;
			}
			//ÿ����ҷ���������Ϣ
			else if (buff[0] == '%')
			{
				cout << "��ҷ���������Ϣ\n";
				myLock.lock();
				memset(client[i].myBoard, 0, sizeof(client[i].myBoard));
				client[i].fightCondition = 1;
				strcpy(client[i].myBoard, buff + 1);
				//Sleep(1000);
				//cout << "�յ�" << i << "����ҵ�������Ϣ��Ϊ" << client[i].myBoard << endl;
				myLock.unlock();
			}
			//���Լ����ֵ����̷��ͻ���
			else if (buff[0] == '$')
			{
				cout << "����������Ϣ\n";
				sendBackBoard(client[i].cliCondition);
			}
			//ս������֮���ͣ��յ�goBuy֮���ٽ�����һ�غϵĹ���
			else if (buff[0] == 'd') //һ��d��ʾһ�غϽ���������d��ʾ�Լ�������������
			{
				cout << "�غϽ���\n";
				if (buff[1] == 'd')
					imDead(i);
				fightOver(i);
			}
			//ˢ���̳�
			else if (buff[0] == 'm')
			{
				cout << "ˢ���̳�\n";
				int level = buff[1] - '0';
				getMarket(i, level);
			}
			//�ͻؿ�Ƭ
			else if (buff[0] == '-')
			{
				cout << "�ͻؿ�Ƭ\n";
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
	//1. ȷ������Э��汾		
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData))//����ʧ��
	{
		//���������Ϣ
		cout << "����socket��ʧ��\n";
		system("pause");
		return -1;
	}
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) 
	{
		printf("ȷ������Э��汾ʧ�ܣ�%d\n",
			GetLastError());
		system("pause");
		return -1;
	}
	printf("ȷ������Э��汾�ɹ�\n");

	//2. ����socket		
	SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSocket == SOCKET_ERROR) 
	{
		printf("����socketʧ�ܣ�%d\n",
			GetLastError());
		//����Э��汾��Ϣ
		WSACleanup();
		system("pause");
		return -1;
	}
	printf("����socket�ɹ�\n");
	//3. ȷ��������Э���ַ��	
	SOCKADDR_IN  addr = { 0 };
	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr = inet_addr("100.81.181.249");
	addr.sin_port = htons(13526);

	//4. ��
	if (bind(serverSocket, (sockaddr*)&addr, sizeof addr) == SOCKET_ERROR) 
	{
		printf("��ʧ�ܣ�%d\n",
			GetLastError());
		//�ر�socket		
		closesocket(serverSocket);
		//����Э��汾��Ϣ
		WSACleanup();
		system("pause");
		return -1;
	}
	printf("�󶨳ɹ�\n");
	//thread testThread(test);
	//5. ����
	if (listen(serverSocket, 100) == SOCKET_ERROR) 
	{
		printf("����ʧ�ܣ�%d\n",
			GetLastError());
		//�ر�socket		
		closesocket(serverSocket);
		//����Э��汾��Ϣ
		WSACleanup();
		system("pause");
		return -1;
	}
	printf("�����ɹ�\n");
	//6. ���ܿͻ�������	
	for (int i = 1; i < MAX_CLIENT; i++) 
	{
		client[i].clientSocket = accept(serverSocket, (sockaddr*)NULL, NULL);
		if (client[i].clientSocket == SOCKET_ERROR)
		{
			printf("������������%d\n",
				GetLastError());
			//�ر�socket		
			closesocket(serverSocket);
			//����Э��汾��Ϣ
			WSACleanup();
			system("pause");
			return -1;
		}
		cout << i << "�ſͻ���������\n";
		numofClient++;
		client[i].cliCondition = -2;
		ReceiveNikename(i);
		//send(client[i].clientSocket, rooms->getRooms(), strlen(rooms->getRooms()), NULL);
		CliThreads.push_back(jthread(listenClient, i));
	}

	//8. �ر�socket		
	closesocket(serverSocket);
	//9. ����Э��汾��Ϣ
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