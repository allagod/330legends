#pragma once

#ifndef __Select_Enemy__
#define __Select_Enemy__
#include<string>
#include<vector>


class select_enemy {
public:
	select_enemy() {
		number = 0;
		count = 0;
		death.clear();
	}

	std::string nextProbableEnemy();//返回index号玩家可能对战敌人的编号 234 134.. 230

	void init(int n);//传入当前玩家的数量

	void died(int index);//传入死人的玩家编号

	std::string battle();//进行对战的函数

private:
	int number;//玩家目前的数量

	std::vector<int>player;

	std::vector<int> death;//记录死亡玩家的编号

	int count;//进行计数的变量

	std::vector<std::string> probableEnemy;//每个玩家可能对战的敌人

	std::vector<std::vector<std::string>>information;//对局信息

};

#endif // !__Select_Enemy_H__
