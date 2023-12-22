//用于存储玩家的小小英雄，血量，等级等信息
#include <iostream>
using namespace std;

class Player
{
private:
#define NONE_CHESS 0
	int health = 100, level = 1, coins = 0;
	struct ChessIfo
	{
		char type = NONE_CHESS;
		int level = 0;
		int chessCoins = 0;
		int equipment[3] = { 0 };
	};
	ChessIfo BoardChessIfo[6][12];
	void intoBoardCode();
public:
	char* getBoardCode();
	void intoChess(char now_type, int now_level, int a, int b, int c, int x, int y, int coinn); // 往棋盘中放棋子
	void clearChess(int x, int y); // 将(x, y) 处的棋子清空
	void sold(int x, int y); //将(x,y)处棋子卖掉
	bool move(int old_x, int old_y, int new_x, int new_y);//将(old_x,old_y)处的棋子移动到(new_x,new_y)，如果新位置无棋子，返回true，如果无法移动，返回false

	// 接口函数
	int getHealth() { return health; }
	int getLevel() { return level; }
	void levelUp() { level++; }
	void healthDown(int down) { health -= down; }
	void healthUp(int up) { health += up; }
};