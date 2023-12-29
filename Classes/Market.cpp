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
	//ѭ��4�εõ�4����

	string cards;

	random_device rd;						     //���õȼ������������
	mt19937 gen(rd());
	int i = 0;
	while (i != 4) {
		discrete_distribution<> distribution(begin(rate[Population_level]), end(rate[Population_level]));
		int cardIdex = distribution(gen);		//�õ������һ�ſ��ĵȼ�
		int whichcard = 1;						//�õȼ��µ���һ��Ӣ��

		switch (cardIdex)
		{
		case 0: {
			discrete_distribution<int> distribution({ 0,1,2,3 });
			whichcard = distribution(gen);		//�õ�Ӣ��
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

		if (card_pool_EmptyorNot() == 1) {		//���п���������
			break;
		}
		if (Card_Pool[whichcard] == 0) {			//��ǰ����������
			continue;
		}
		mall.push_back(whichcard);
		Card_Pool[whichcard]--;
		++i;
	}
	sort(mall.begin(), mall.end());				//�Գ������Ӣ������
	int CardSize = mall.size();
	i = 0;									    //�Կ���ת��string����
	while (i <= CardSize - 1) {
		cards += CHAR(mall[i]);
		++i;
	}
	int j = 0;
	while (j < 4 - CardSize) {				//ȱ�ٿ�����0����
		cards += '0';
		++j;
	}
	mall.clear();
	return cards;
}