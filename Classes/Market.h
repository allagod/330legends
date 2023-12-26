#pragma once
#include<iostream>
#include<vector>
#include<random>
#include<cstring>
#include<algorithm>
#include "GlobalRes.h"
using namespace std;

//商城类说明
// 1.mount[3][4]二维数组  3表示2、3、4三种人数的时候牌的数量  4表示四种不同等级牌的的数量
//	目前只是给了一个大概的数字，可以更改
// 2.
// 3.需要刷新：传参（string）根据等级返回4张卡类似aabc  aa00
// 4.（没有买、死人等）放回卡池：类型、张数char int
// 5.对战人数
// 

//商城类
class Market
{
private:
#define  CHAR(i) char(i+'a') //用于将原本的数字转成小写的字母
#define  INT(j) int(j-'a')	 //用于将传来的字母转换成数字
#define Shooter1  0
#define Warrior1  1
#define Tank1     2
#define Master1   3
	//1级卡（三种）
#define Shooter2  4
#define Tank2     5
#define Warrior   6
//2级卡（两种）
#define Tank3     7
#define Master3   8
//3级卡（一种）
#define Shooter4  9
	int Card_Pool[10] = {};				//卡池数组
	int mount[3][4] = {					//不同对战人数对应的四种等级的卡牌数量
		{6,5,4,3},		//2人 0级牌有6张，1级牌有5张...
		{8,7,6,5},		//3人
		{12,11,10,9},	//4人
	};
	//不同等级对应的刷新概率
	double rate[6][4] = {
	{1,0,0,0} ,
	{0.75,0.25,0,0},
	{0.5,0.3,0.2,0} ,
	{0.25,0.4,0.3,0.05},
	{0.2,0.3,0.4,0.1},
	{0.15,0.25,0.45,015} };
	int GamePopulation;					//对战人数
	vector<int> mall;					//每次刷新的四张卡
	void Init_CardPool();	//卡池的初始化
	bool card_pool_EmptyorNot();	//判断卡池是否为空


	Market();
	Market(const Market&) = delete;
	Market& operator =(const Market&) = delete;
	void init(int num);
public:
	static Market* getInstance();

	//接口：用于在卡池中删除购买了的卡
	void take(char ch, int num) { Card_Pool[INT(ch)] -= num; }
	//接口：返回卡到卡池
	void back(char ch, int num) { Card_Pool[INT(ch)] += num; }

	//根据等级按照一定概率输出牌
	//第一个随机数判断卡牌等级；第二个随机数判断英雄
	//传参Population_level，本函数根据人口等级进行对应的概率刷新
	//返回的是四张卡牌的string字符串,例如"aabf"，如果牌库缺卡则表示0
	string GetCard(int Population_level);
};
