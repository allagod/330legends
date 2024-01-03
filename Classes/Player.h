#pragma once
#include <string>
#include "Market.h"
#include "SelectEnemy.h"
using namespace std;

class Player
{
private:
#define NONE_CHESS 0
#define MAX_PREPARE 5
	//���캯��˽�л�
	Player() {}
	Player(const Player&) = delete;
	Player& operator =(const Player&) = delete;

	int health = 100, level = 1, coins = 4, experience = 0, numChess = 0;
	const int needExperience[10] = { 0, 2, 2, 4, 8, 10, 36, 48, 72 };
	struct ChessIfo
	{
		char type = NONE_CHESS;
		int level = 0;
		int chessCoins = 0;
		int equipment[3] = { 0 };
		int numOfEquipments = 0;
	};
	ChessIfo BoardChessIfo[6][12];
	vector<char> buying;
	vector<int> equipments{ 1,2,3 };
	string intoBoardCode(ChessIfo now);
	void intoChess(char now_type, int now_level, int a, int b, int c, int x, int y, int coinn);
	void clearChess(int x, int y);
public:
	//��õ�ǰ����
	static Player* getInstance();

	// ���ص�ǰ���̵ı���
	string getBoardCode();

	//��(x,y)����������
	void sold(int x, int y);

	//��(old_x,old_y)���������ƶ���(new_x,new_y)�������λ�������ӣ�����true������޷��ƶ�������false
	bool move(int old_x, int old_y, int new_x, int new_y);

	//ˢ���̳ǣ�����һ���ַ�����"abcc"��ʾ�̳�ˢ�³��Ŀ���abcc��������
	//��ˢ�°�ť�����������
	string marketRefresh();

	//���̳��е�x�ſ�Ƭ
	/*����һ���ַ���
	"*"��ʾ��ǰ��սϯ�������޷�����
	"04"��ʾ��ǰ������ͬ�������Ӳ���3�ţ���Ҫɾ��"0"�����飬�����������ӷ��ڱ�սϯ���ĺ�λ
	"201231"��ʾ��Ҫɾ��"2"�����飬��Ҫɾ���ľ���λ����"(0,1)""(2,3)"������(0,1)��ʾ��սϯ�ĵ�һ��λ�ã���սϯ��λ�ô�һ�ſ�ʼ����BoardChessIfo[0][0]���ã�
			��ɾ��������������¹���ľ�����ڱ�սϯ�ĵ�"1"��λ��
	"401022654"��ʾ��Ҫɾ��4�����飬����8λ����Ҫɾ�������λ�ã����һλ���¹�������õ�λ�ã���սϯ���ĸ�
	���ڹ��������ֻ��ɾ��2��4�������������������ɾ��2�����¹������Ϊ2�ǣ�ɾ��4�����¹������Ϊ3��
	*/
	string marketBuy(int x);

	//����index��װ��װ��(x,y)��������
	bool equip(int index, int x, int y);

	//��������ť���������������һ�μ�4�㾭��ֵ������ֵ���˾ͻ�����
	//���������֮���������򷵻�true�����û���������򷵻�false
	//ע��ȼ����������ϳ�������������������ʱ1�����Ѿ�����һ�����ӣ���Ҫ���϶������ϳ�ʱ����Ҫ�ȵ���move�����жϣ������ƶ���Ӧ�õ���move�жϣ������move����false˵���������������������ϳ�
	bool experienceUp();

	//ս���������Ѫ�ĺ���������������᷵��true
	bool healthDown(int down);

	//�����»غϿ��ܶ�ս�Ķ��֣�����ʱ�����Զ��1�ţ�����ֻ����һ����ҿ��ܵĶ���
	string getNextProbableEnemy();

	//��Ұ�ֺ���ã�������λ�����ַ�������"112"��ʾ�õ�������1��װ����һ��2��װ��
	string getEquipments();

	//���ʼ���ߴ���ص�ǰ����ʱ���ã����ڽ���һ���µĻغϣ����������4���
	void intoNextTurn();

	//����õ�һ�����˵ı���
	string getEnemyCode();

	// �ӿں���
	int getHealth() { return health; }
	int getLevel() { return level; }
	int getCoins() { return coins; }
};