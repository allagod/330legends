//���ڴ洢��ҵ�ССӢ�ۣ�Ѫ�����ȼ�����Ϣ
#include <string>
#include "Market.h"
using namespace std;

class Player
{
private:
#define NONE_CHESS 0
#define MAX_PREPARE 5
	//����Ĭ�Ϲ��캯��
	Player();
	Player(const Player&) = delete;
	Player& operator =(const Player&) = delete;

	int health = 100, level = 1, coins = 0, experience = 0;
	const int needExperience[10] = { 0, 2, 2, 4, 8, 10, 36, 48, 72 };
	struct ChessIfo
	{
		char type = NONE_CHESS;
		int level = 0;
		int chessCoins = 0;
		int equipment[3] = { 0 };
	};
	ChessIfo BoardChessIfo[6][12];
	void intoBoardCode(char* tmp, ChessIfo now);
	vector<char> buying;
public:
	static Player* getInstance();
	// ���ص�ǰ���̵ı���
	char* getBoardCode();

	// �������з�����
	void intoChess(char now_type, int now_level, int a, int b, int c, int x, int y, int coinn);

	// ��(x, y) �����������
	void clearChess(int x, int y);

	//��(x,y)����������
	void sold(int x, int y);

	//��(old_x,old_y)���������ƶ���(new_x,new_y)�������λ�������ӣ�����true������޷��ƶ�������false
	bool move(int old_x, int old_y, int new_x, int new_y);

	//ˢ���̳�
	string marketRefresh();

	//���̳��е�x�ſ�Ƭ
	string marketBuy(int x);

	//����index��װ��װ��(x,y)��������
	bool equip(int index, int x, int y);

	//��������ť���������������һ�μ�4�㾭��ֵ������ֵ���˾ͻ�����
	void experienceUp();

	// �ӿں���
	int getHealth() { return health; }
	int getLevel() { return level; }
	void healthDown(int down) { health -= down; }
	void healthUp(int up) { health += up; }
};

Player* Player::getInstance()
{
	static Player local_instance;
	return &local_instance;
}