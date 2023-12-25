//用于存储玩家的小小英雄，血量，等级等信息
#include <string>
#include "Market.h"
using namespace std;

class Player
{
private:
#define NONE_CHESS 0
#define MAX_PREPARE 5
	//禁用默认构造函数
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
	// 返回当前棋盘的编码
	char* getBoardCode();

	// 往棋盘中放棋子
	void intoChess(char now_type, int now_level, int a, int b, int c, int x, int y, int coinn);

	// 将(x, y) 处的棋子清空
	void clearChess(int x, int y);

	//将(x,y)处棋子卖掉
	void sold(int x, int y);

	//将(old_x,old_y)处的棋子移动到(new_x,new_y)，如果新位置无棋子，返回true，如果无法移动，返回false
	bool move(int old_x, int old_y, int new_x, int new_y);

	//刷新商城
	string marketRefresh();

	//买商城中第x张卡片
	string marketBuy(int x);

	//将第index个装备装到(x,y)处棋子上
	bool equip(int index, int x, int y);

	//按升级按钮调用这个函数，点一次加4点经验值，经验值够了就会升级
	void experienceUp();

	// 接口函数
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