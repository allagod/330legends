#include "Player.h"



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