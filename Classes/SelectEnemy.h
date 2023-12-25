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

	std::string nextProbableEnemy();//����index����ҿ��ܶ�ս���˵ı�� 234 134.. 230

	void init(int n);//���뵱ǰ��ҵ�����

	void died(int index);//�������˵���ұ��

	std::string battle();//���ж�ս�ĺ���

private:
	int number;//���Ŀǰ������

	std::vector<int>player;

	std::vector<int> death;//��¼������ҵı��

	int count;//���м����ı���

	std::vector<std::string> probableEnemy;//ÿ����ҿ��ܶ�ս�ĵ���

	std::vector<std::vector<std::string>>information;//�Ծ���Ϣ

};

#endif // !__Select_Enemy_H__
