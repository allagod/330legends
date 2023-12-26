#pragma once
#include<string>
#include<vector>
#include "GlobalRes.h"


class SelectEnemy
{
public:
	static SelectEnemy* getInstance();
	std::string nextProbableEnemy();//返回index号玩家可能对战敌人的编号 234 134.. 230

	void init(int n);//传入当前玩家的数量

	void died(int index);//传入死人的玩家编号

	std::string battle();//进行对战的函数

private:
	SelectEnemy() {
		number = 0;
		count = 0;
		death.clear();
		init(GlobalRes::getInstance()->getPlayers());
	}
	SelectEnemy(const SelectEnemy&) = delete;
	SelectEnemy& operator =(const SelectEnemy&) = delete;
	int number;//玩家目前的数量

	std::vector<int>player;

	std::vector<int> death;//记录死亡玩家的编号

	int count;//进行计数的变量

	std::vector<std::string> probableEnemy;//每个玩家可能对战的敌人

	std::vector<std::vector<std::string>>information;//对局信息

};

SelectEnemy* SelectEnemy::getInstance()
{
	static SelectEnemy local_instance;
	return &local_instance;
}