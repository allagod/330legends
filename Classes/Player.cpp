#include "Player.h"

Player::Player()
{
}

void Player::intoBoardCode(char* tmp, ChessIfo now)
{
	if (now.type == NONE_CHESS)
	{
		for (int i = 0; i < 5; i++)
			tmp[i] = '0';
		return;
	}
	tmp[0] = now.type; tmp[1] = now.level + '0';
	for (int i = 2; i < 5; i++)
	{
		tmp[i] = now.equipment[i - 2] + '0';
	}
}

// 该函数返回要参与战斗的棋子的位置信息
// %开头，用五位字符表示一个位置的棋子
// 第一位是棋子的类型，第二位是棋子的星级，第三到五位表示棋子所携带的装备
// "%a212000000c1000"表示(1,1)位置棋子类型是a，二星，携带编号为1和2的两个装备，(1,2)位置无棋子，(1,3)位置棋子类型是c，一星，无装备
char* Player::getBoardCode()
{
	char tmp[150];
	tmp[0] = '%';
	for (int i = 1; i <= 4; i++)
		for (int j = 1; j <= 7; j++)
			intoBoardCode(tmp + 1 + (i * 7 + j - 8) * 5, BoardChessIfo[i][j]);
	tmp[1 + (4 * 7 + 8 - 8) * 5] = '0';
	return tmp;
}

// 往棋盘中放棋子
void Player::intoChess(char now_type, int now_level, int a, int b, int c, int x, int y, int coinn)
{
	BoardChessIfo[x][y].type = now_type;
	BoardChessIfo[x][y].level = now_level;
	BoardChessIfo[x][y].equipment[0] = a;
	BoardChessIfo[x][y].equipment[1] = b;
	BoardChessIfo[x][y].equipment[2] = c;
	BoardChessIfo[x][y].chessCoins = coinn;
	return;
}

// 将(x, y) 处的棋子清空
void Player::clearChess(int x, int y)
{
	BoardChessIfo[x][y].type = NONE_CHESS;
	BoardChessIfo[x][y].level = 0;
	BoardChessIfo[x][y].equipment[0] = 0;
	BoardChessIfo[x][y].equipment[1] = 0;
	BoardChessIfo[x][y].equipment[2] = 0;
	return;
}

void Player::sold(int x, int y)
{
	coins += BoardChessIfo[x][y].chessCoins;
	//记得放回卡池
	clearChess(x, y);
}

bool Player::move(int old_x, int old_y, int new_x, int new_y)
{
	if (BoardChessIfo[new_x][new_y].type != NONE_CHESS)
		return false;
	intoChess(BoardChessIfo[old_x][old_y].type, BoardChessIfo[old_x][old_y].level, BoardChessIfo[old_x][old_y].equipment[0]
		, BoardChessIfo[old_x][old_y].equipment[1], BoardChessIfo[old_x][old_y].equipment[2], new_x, new_y, BoardChessIfo[old_x][old_y].chessCoins);
	clearChess(old_x, old_y);
	return true;
}

//刷新商城
string Player::marketRefresh()
{
	if (buying.size())
		for (auto i : buying)
			Market::getInstance()->back(i, 1);
	return Market::getInstance()->GetCard(level);
}

//买商城中第x张卡片
string Player::marketBuy(int x)
{
	char nowType = buying[x];
	bool tag = 0;
	int inLocation;
	for (inLocation = 1; inLocation <= MAX_PREPARE; inLocation++)
		if (BoardChessIfo[0][inLocation].type == NONE_CHESS)
		{
			tag = 1;
			break;
		}
	if (!tag)
		return "0";
	//遍历棋盘，寻找是否可以升星
	int tot = 0, nowx[10], nowy[10];
	for (int i = 0; i <= 4 && tag; i++)
		for (int j = 1; j <= MAX_PREPARE; j++)
		{
			if (BoardChessIfo[i][j].type == nowType && BoardChessIfo[i][j].level == 1)
			{
				nowx[++tot] = i; nowy[tot] = j;
			}
			if (tot == 3)
			{
				tag = 0;
				break;
			}
		}
	tag = 1;
	for (int i = 0; i <= 4 && tag; i++)
		for (int j = 1; j <= MAX_PREPARE; j++)
		{
			if (BoardChessIfo[i][j].type == nowType && BoardChessIfo[i][j].level == 1)
			{
				nowx[++tot] = i; nowy[tot] = j;
			}
			if (tot == 3)
			{
				tag = 0;
				break;
			}
		}
}

//将第index个装备装到(x,y)处棋子上
bool Player::equip(int index, int x, int y)
{

}

//按升级按钮调用这个函数，点一次加4点经验值，经验值够了就会升级
void Player::experienceUp()
{

}