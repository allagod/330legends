#pragma once
#include<iostream>
#include<vector>
#include<random>
#include<cstring>
#include<algorithm>
#include "GlobalRes.h"
using namespace std;

//�̳���˵��
// 1.mount[3][4]��ά����  3��ʾ2��3��4����������ʱ���Ƶ�����  4��ʾ���ֲ�ͬ�ȼ��Ƶĵ�����
//	Ŀǰֻ�Ǹ���һ����ŵ����֣����Ը���
// 2.
// 3.��Ҫˢ�£����Σ�string�����ݵȼ�����4�ſ�����aabc  aa00
// 4.��û�������˵ȣ��Żؿ��أ����͡�����char int
// 5.��ս����
// 

//�̳���
class Market
{
private:
#define  CHAR(i) char(i+'a') //���ڽ�ԭ��������ת��Сд����ĸ
#define  INT(j) int(j-'a')	 //���ڽ���������ĸת��������
#define Shooter1  0
#define Warrior1  1
#define Tank1     2
#define Master1   3
	//1���������֣�
#define Shooter2  4
#define Tank2     5
#define Warrior   6
//2���������֣�
#define Tank3     7
#define Master3   8
//3������һ�֣�
#define Shooter4  9
	int Card_Pool[10] = {};				//��������
	int mount[3][4] = {					//��ͬ��ս������Ӧ�����ֵȼ��Ŀ�������
		{6,5,4,3},		//2�� 0������6�ţ�1������5��...
		{8,7,6,5},		//3��
		{12,11,10,9},	//4��
	};
	//��ͬ�ȼ���Ӧ��ˢ�¸���
	double rate[6][4] = {
	{1,0,0,0} ,
	{0.75,0.25,0,0},
	{0.5,0.3,0.2,0} ,
	{0.25,0.4,0.3,0.05},
	{0.2,0.3,0.4,0.1},
	{0.15,0.25,0.45,015} };
	int GamePopulation;					//��ս����
	vector<int> mall;					//ÿ��ˢ�µ����ſ�
	void Init_CardPool();	//���صĳ�ʼ��
	bool card_pool_EmptyorNot();	//�жϿ����Ƿ�Ϊ��


	Market();
	Market(const Market&) = delete;
	Market& operator =(const Market&) = delete;
	void init(int num);
public:
	static Market* getInstance();

	//�ӿڣ������ڿ�����ɾ�������˵Ŀ�
	void take(char ch, int num) { Card_Pool[INT(ch)] -= num; }
	//�ӿڣ����ؿ�������
	void back(char ch, int num) { Card_Pool[INT(ch)] += num; }

	//���ݵȼ�����һ�����������
	//��һ��������жϿ��Ƶȼ����ڶ���������ж�Ӣ��
	//����Population_level�������������˿ڵȼ����ж�Ӧ�ĸ���ˢ��
	//���ص������ſ��Ƶ�string�ַ���,����"aabf"������ƿ�ȱ�����ʾ0
	string GetCard(int Population_level);
};
