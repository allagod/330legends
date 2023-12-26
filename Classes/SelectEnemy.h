#pragma once
#include<string>
#include<vector>
#include "GlobalRes.h"


class SelectEnemy
{
public:
	static SelectEnemy* getInstance();
	std::string nextProbableEnemy();//����index����ҿ��ܶ�ս���˵ı�� 234 134.. 230

	void init(int n);//���뵱ǰ��ҵ�����

	void died(int index);//�������˵���ұ��

	std::string battle();//���ж�ս�ĺ���

private:
	SelectEnemy() {
		number = 0;
		count = 0;
		death.clear();
		init(GlobalRes::getInstance()->getPlayers());
	}
	SelectEnemy(const SelectEnemy&) = delete;
	SelectEnemy& operator =(const SelectEnemy&) = delete;
	int number;//���Ŀǰ������

	std::vector<int>player;

	std::vector<int> death;//��¼������ҵı��

	int count;//���м����ı���

	std::vector<std::string> probableEnemy;//ÿ����ҿ��ܶ�ս�ĵ���

	std::vector<std::vector<std::string>>information;//�Ծ���Ϣ

};

SelectEnemy* SelectEnemy::getInstance()
{
	static SelectEnemy local_instance;
	return &local_instance;
}