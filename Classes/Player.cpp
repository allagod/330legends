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

// �ú�������Ҫ����ս�������ӵ�λ����Ϣ
// %��ͷ������λ�ַ���ʾһ��λ�õ�����
// ��һλ�����ӵ����ͣ��ڶ�λ�����ӵ��Ǽ�����������λ��ʾ������Я����װ��
// "%a212000000c1000"��ʾ(1,1)λ������������a�����ǣ�Я�����Ϊ1��2������װ����(1,2)λ�������ӣ�(1,3)λ������������c��һ�ǣ���װ��
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

// �������з�����
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

// ��(x, y) �����������
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
	//�ǵ÷Żؿ���
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

//ˢ���̳�
string Player::marketRefresh()
{
	if (buying.size())
		for (auto i : buying)
			Market::getInstance()->back(i, 1);
	return Market::getInstance()->GetCard(level);
}

//���̳��е�x�ſ�Ƭ
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
	//�������̣�Ѱ���Ƿ��������
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

//����index��װ��װ��(x,y)��������
bool Player::equip(int index, int x, int y)
{

}

//��������ť���������������һ�μ�4�㾭��ֵ������ֵ���˾ͻ�����
void Player::experienceUp()
{

}