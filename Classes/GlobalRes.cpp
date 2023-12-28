#include "GlobalRes.h"

GlobalRes* GlobalRes::getInstance()
{
	static GlobalRes local_instance;
	return &local_instance;
}

void GlobalRes::setPlayers(int num)
{
	players = num;
	alivePeople = num;
	for (int i = 1; i <= num; i++)
		isAlive[i] = 1;
}

void GlobalRes::setBGM(bool my_set)
{
	isbgm = my_set;
}

bool GlobalRes::died(int num)
{
	alivePeople--;
	isAlive[num] = 0;

	if (alivePeople == 1)
		return true;
	return false;
}

void GlobalRes::setOnline()
{
	isOnline = 1;
}

void GlobalRes::addPeriod()
{
	period++;
}