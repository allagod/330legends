#include <iostream>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#define MAX_NIKELEN 64
#define BUFF_LEN 64
#define MAX_NIKELEN 64

class NetWork
{
private:
	char nikename[MAX_NIKELEN]; //�洢����ǳ�
	SOCKET serverSocket; //���ù�
	char buff[BUFF_LEN];//���ù�
public:
	NetWork();
	~NetWork();
	void sendNikename(char nikename[]); //�����ǳƺ�������������ǳƷ���������

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
};