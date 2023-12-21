//���ڴ洢��ҵ�ССӢ�ۣ�Ѫ�����ȼ�����Ϣ
#include <iostream>
using namespace std;

class Player
{
private:
#define NONE_CHESS 0
	int health = 0, level = 0;
	struct ChessIfo
	{
		char type = NONE_CHESS;
		int level = 0;
		int equipment[3] = { 0 };
	};
	ChessIfo BoardChessIfo[6][12];
	void intoBoardCode();
public:
	char* getBoardCode();
	void intoChess(char now_type, int now_level, int a, int b, int c, int x, int y); // �������з�����
	void clearChess(int x, int y); // ��(x, y) �����������

	// �ӿں���
	int getHealth() { return health; }
	int getLevel() { return level; }
	void levelUp() { level++; }
	void healthDown(int down) { health -= down; }
	void healthUp(int up) { health += up; }
};