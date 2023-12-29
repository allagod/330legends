#include <random>
#include "Player.h"
#include "Network.h"

Player* Player::getInstance()
{
	static Player local_instance;
	return &local_instance;
}

string Player::intoBoardCode(ChessIfo now)
{
	string tmp;
	if (now.type == NONE_CHESS)
	{
		for (int i = 0; i < 5; i++)
			tmp += '0';
		return tmp;
	}
	tmp += now.type; tmp += now.level + '0';
	for (int i = 0; i < 3; i++)
		tmp += now.equipment[i] + '0';
	return tmp;
}

// �ú�������Ҫ����ս�������ӵ�λ����Ϣ
// %��ͷ������λ�ַ���ʾһ��λ�õ�����
// ��һλ�����ӵ����ͣ��ڶ�λ�����ӵ��Ǽ�����������λ��ʾ������Я����װ��
// "%a212000000c1000"��ʾ(1,1)λ������������a�����ǣ�Я�����Ϊ1��2������װ����(1,2)λ�������ӣ�(1,3)λ������������c��һ�ǣ���װ��
string Player::getBoardCode()
{
	string tmp;
	tmp += '%';
	for (int i = 1; i <= 4; i++)
		for (int j = 1; j <= 7; j++)
			tmp += intoBoardCode(BoardChessIfo[i][j]);
	tmp += '0';
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
	BoardChessIfo[x][y].chessCoins = 0;
	return;
}

void Player::sold(int x, int y)
{
	coins += BoardChessIfo[x][y].chessCoins;
	if (GlobalRes::getInstance()->IsOnline())
	{
		Network::getInstance()->cardBack(BoardChessIfo[x][y].type, BoardChessIfo[x][y].level * pow(3, level - 1));
	}
	else
	{
		Market::getInstance()->back(BoardChessIfo[x][y].type, BoardChessIfo[x][y].level * pow(3, level - 1));
	}
	if (x != 0)
		numChess--;
	clearChess(x, y);
}

bool Player::move(int old_x, int old_y, int new_x, int new_y)
{
	if (BoardChessIfo[new_x][new_y].type != NONE_CHESS)
		return false;
	if (old_x == 0 && new_x != 0 && numChess == level)
		return false;
	if (old_x == 0 && new_x != 0)
		numChess++;
	if (old_x != 0 && new_x == 0)
		numChess--;
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
		{
			if (GlobalRes::getInstance()->IsOnline())
				Network::getInstance()->cardBack(i, 1);
			else
				Market::getInstance()->back(i, 1);
		}
	buying.clear();
	string cards;
	if (GlobalRes::getInstance()->IsOnline())
		cards = Network::getInstance()->getCard(level);
	else
		cards = Market::getInstance()->GetCard(level);
	for (int i = 0; i < 4; i++)
		buying.push_back(cards[i]);
	coins -= 2;
	return cards;
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
		return "*";
	//�������̣�Ѱ���Ƿ��������
	string returnString;
	bool ifToLevel2 = 0;
	int tot = 0, nowx[10], nowy[10];
	for (int i = 0; i <= 4 && (!ifToLevel2); i++)
		for (int j = 1; j <= 8; j++)
		{
			if (BoardChessIfo[i][j].type == nowType && BoardChessIfo[i][j].level == 1)
			{
				nowx[++tot] = i; nowy[tot] = j;
			}
			if (tot == 2)
			{
				ifToLevel2 = 1;
				break;
			}
		}
	if (!ifToLevel2)
	{
		returnString += '0';
		returnString += inLocation + '0';
		BoardChessIfo[0][inLocation].type = nowType;
		BoardChessIfo[0][inLocation].level = 1;
		BoardChessIfo[0][inLocation].chessCoins = Market::getInstance()->getPrice(nowType);
		coins -= BoardChessIfo[0][inLocation].chessCoins;
		return returnString;
	}
	bool ifToLevel3 = 0;
	for (int i = 0; i <= 4 && (!ifToLevel3); i++)
		for (int j = 1; j <= 8; j++)
		{
			if (BoardChessIfo[i][j].type == nowType && BoardChessIfo[i][j].level == 2)
			{
				nowx[++tot] = i; nowy[tot] = j;
			}
			if (tot == 4)
			{
				ifToLevel3 = 1;
				break;
			}
		}
	if (!ifToLevel3)
	{
		returnString += '2';
		for (int i = 1; i <= 2; i++)
		{
			returnString += nowx[i] + '0'; returnString += nowy[i] + '0';
		}
		returnString += inLocation + '0';
		BoardChessIfo[0][inLocation].type = nowType;
		BoardChessIfo[0][inLocation].level = 1;
		BoardChessIfo[0][inLocation].chessCoins = Market::getInstance()->getPrice(nowType);
		coins -= BoardChessIfo[0][inLocation].chessCoins;
		return returnString;
	}
	returnString += '4';
	for (int i = 1; i <= 4; i++)
	{
		returnString += nowx[i] + '0'; returnString += nowy[i] + '0';
	}
	returnString += inLocation + '0';
	BoardChessIfo[0][inLocation].type = nowType;
	BoardChessIfo[0][inLocation].level = 1;
	BoardChessIfo[0][inLocation].chessCoins = Market::getInstance()->getPrice(nowType);
	coins -= BoardChessIfo[0][inLocation].chessCoins;
	return returnString;
}

//����index��װ��װ��(x,y)��������
bool Player::equip(int index, int x, int y)
{
	if (BoardChessIfo[x][y].type == NONE_CHESS || BoardChessIfo[x][y].numOfEquipments == 3)
		return false;
	BoardChessIfo[x][y].equipment[BoardChessIfo[x][y].numOfEquipments++] = equipments[index];
	equipments.erase(equipments.begin() + index);
	return true;
}

//��������ť���������������һ�μ�4�㾭��ֵ������ֵ���˾ͻ�����
bool Player::experienceUp()
{
	experience += 4;
	coins -= 4;
	if (experience >= needExperience[level])
	{
		level++;
		experience -= needExperience[level];
		return true;
	}
	return false;
}

bool Player::healthDown(int down)
{
	health -= down;
	if (health <= 0)
		return true;
	return false;
}

string Player::getNextProbableEnemy()
{
	string tmp = SelectEnemy::getInstance()->nextProbableEnemy();
	string returnString;
	for (int i = 0; i < GlobalRes::getInstance()->getAlivePeople() - 1; i++)
		returnString += tmp[i];
	return returnString;
}

string Player::getEquipments()
{
	string s;
	random_device rd;
	// ʹ�� Mersenne Twister ����
	mt19937 gen(rd());
	// ����ֲ���Χ
	uniform_int_distribution<> distribution(1, 6);
	for (int i = 1; i <= 3; i++)
	{
		int nowRandom = distribution(gen);
		s += nowRandom + '0';
		equipments.push_back(nowRandom);
	}
	return s;
}

void Player::intoNextTurn()
{
	coins += 4;
	GlobalRes::getInstance()->addPeriod();
}