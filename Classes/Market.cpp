#include "Market.h"

Market::Market()
{
	init(GlobalRes::getInstance()->getPlayers());
}

Market* Market::getInstance()
{
	static Market local_instance;
	return &local_instance;
}

int Market::getPrice(char ch)
{
	return fei[ch - 'a'];
}

void Market::init(int num)
{
	GamePopulation = num;
	Init_CardPool();
}

void Market::Init_CardPool()
{
	int i = 0;
	for (; i <= 9; ++i)
	{
		if (i <= 3)
			Card_Pool[i] = mount[GamePopulation - 2][0];
		else if (i <= 6)
			Card_Pool[i] = mount[GamePopulation - 2][1];
		else if (i <= 8)
			Card_Pool[i] = mount[GamePopulation - 2][2];
		else if (i <= 9)
			Card_Pool[i] = mount[GamePopulation - 2][3];
	}
}

bool Market::card_pool_EmptyorNot()
{
	if (Card_Pool[0] == 0 &&
		Card_Pool[1] == 0 &&
		Card_Pool[2] == 0 &&
		Card_Pool[3] == 0 &&
		Card_Pool[4] == 0 &&
		Card_Pool[5] == 0 &&
		Card_Pool[6] == 0 &&
		Card_Pool[7] == 0 &&
		Card_Pool[8] == 0 &&
		Card_Pool[9] == 0)
	{
		return 1;
	}
}

string Market::GetCard(int Population_level)
{
	//循环4次得到4张牌

	string cards;

	random_device rd;						     //设置等级的随机数种子
	mt19937 gen(rd());
	int i = 0;
	while (i != 4) {
		discrete_distribution<> distribution(begin(rate[Population_level]), end(rate[Population_level]));
		int cardIdex = distribution(gen);		//得到抽出来一张卡的等级
		int whichcard = 1;						//该等级下的哪一种英雄

		switch (cardIdex)
		{
		case 0: {
			discrete_distribution<int> distribution({ 0,1,2,3 });
			whichcard = distribution(gen);		//得到英雄
		}
			  break;
		case 1: {
			discrete_distribution<int> distribution({ 4,5,6 });
			whichcard = distribution(gen) + 4;
		}
			  break;
		case 2: {
			discrete_distribution<int> distribution({ 7, 8 });
			whichcard = distribution(gen) + 7;
		}
			  break;
		case 3:
			whichcard = 9;
			break;
		}

		if (card_pool_EmptyorNot() == 1) {		//所有卡被抽完了
			break;
		}
		if (Card_Pool[whichcard] == 0) {			//当前卡被抽完了
			continue;
		}
		mall.push_back(whichcard);
		Card_Pool[whichcard]--;
		++i;
	}
	sort(mall.begin(), mall.end());				//对抽出来的英雄排序
	int CardSize = mall.size();
	i = 0;									    //对卡组转成string类型
	while (i <= CardSize - 1) {
		cards += CHAR(mall[i]);
		++i;
	}
	int j = 0;
	while (j < 4 - CardSize) {				//缺少卡就用0补上
		cards += '0';
		++j;
	}
	mall.clear();
	return cards;
}