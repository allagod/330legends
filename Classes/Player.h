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
	//构造函数私有化
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
	//获得当前单例
	static Player* getInstance();

	// 返回当前棋盘的编码
	string getBoardCode();

	//将(x,y)处棋子卖掉
	void sold(int x, int y);

	//将(old_x,old_y)处的棋子移动到(new_x,new_y)，如果新位置无棋子，返回true，如果无法移动，返回false
	bool move(int old_x, int old_y, int new_x, int new_y);

	//刷新商城，返回一个字符串，"abcc"表示商城刷新出的卡是abcc四种类型
	//按刷新按钮调用这个函数
	string marketRefresh();

	//买商城中第x张卡片
	/*返回一个字符串
	"*"表示当前备战席已满，无法购买
	"04"表示当前棋盘中同类型棋子不满3张，需要删除"0"个精灵，被购买后的棋子放在备战席的四号位
	"201231"表示需要删除"2"个精灵，需要删除的精灵位置是"(0,1)""(2,3)"，其中(0,1)表示备战席的第一个位置（备战席的位置从一号开始，即BoardChessIfo[0][0]弃用）
			在删除这两个精灵后，新购买的精灵放在备战席的第"1"个位置
	"401022654"表示需要删除4个精灵，后续8位是需要删除精灵的位置，最后一位是新购买精灵放置的位置，备战席第四个
	由于购买后生星只有删除2和4个精灵的情况，所以如果删除2个，新购买精灵变为2星，删除4个，新购买精灵变为3星
	*/
	string marketBuy(int x);

	//将第index个装备装到(x,y)处棋子上
	bool equip(int index, int x, int y);

	//按升级按钮调用这个函数，点一次加4点经验值，经验值够了就会升级
	//如果调用了之后升级，则返回true，如果没有升级，则返回false
	//注意等级数就是能上场的最大棋子数，如果此时1级，已经上了一个棋子，还要再拖动棋子上场时，需要先调用move函数判断（所有移动都应该调用move判断），如果move返回false说明场上棋子已满，不能上场
	bool experienceUp();

	//战斗结束后扣血的函数，如果死亡，会返回true
	bool healthDown(int down);

	//返回下回合可能对战的对手，单机时玩家永远是1号，所以只返回一号玩家可能的对手
	string getNextProbableEnemy();

	//打野怪后调用，返回三位长的字符串，如"112"表示得到了两件1号装备，一件2号装备
	string getEquipments();

	//在最开始或者打完回当前场景时调用，用于进入一个新的回合，给玩家增加4金币
	void intoNextTurn();

	//随机得到一串敌人的编码
	string getEnemyCode();

	// 接口函数
	int getHealth() { return health; }
	int getLevel() { return level; }
	int getCoins() { return coins; }
};