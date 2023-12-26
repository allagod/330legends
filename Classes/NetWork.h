#include <iostream>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

class Network
{
private:
#define MAX_NIKELEN 64
#define BUFF_LEN 64
#define MAX_NIKELEN 64
	Network();
	Network(const Network&) = delete;
	Network& operator =(const Network&) = delete;


	char nikename[MAX_NIKELEN]; //�洢����ǳ�
	SOCKET serverSocket; //���ù�
	char buff[BUFF_LEN];//���ù�
	int isHost = 0; //�Ƿ��Ƿ���
public:
	static Network* getInstance();
	//��ʼ������v
	void init();

	//�����ǳƺ�������������ǳƷ���������
	void sendNikename(char nikename[]);

	//����ڴ��������ظ����������Ϣ
	//����"3#1guolinghao##2yg##5chl#"��ʾ���������䣬��������ķ�����#...#���������Ϊ#�������

	//����ڷ����ڣ����ط����ڵ���Ϣ
	//"2#guolinghao##yg#"��һ���ַ���ʾ�������ж����ˣ�#..#�м��Ƿ���������ǳ�
	char* refresh();

	//�������䣬����ɹ����ش����������ţ�ʧ�ܷ���-1
	int buildRoom();

	//����index���䣬����ɹ�����true��ʧ�ܷ���false
	bool intoRoom(int index);

	//�Ƿ�����Ա�ڽ��뷿�����øú����ȴ���ʼ
	void wait();

	//������ʼ��Ϸ���øú���
	void startGame();

	//������һ�ֿ��ܶ�ս˭
	char* getNextEnemy();

	//�����Լ������̣����ض��ֵ�����
	char* getFightBoard(const char myBoard[]);

	//�����Ƿ�������0����û����1��������
	void fightOver(int die);

	//�����̳Ǹ��Ŀ�
	char* getCard(int level);

	//��û��Ŀ��˻�ȥ
	void cardBack(char ch, int num);
};