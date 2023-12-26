#pragma once
class GlobalRes
{
private:
	GlobalRes();
	GlobalRes(const GlobalRes&) = delete;
	GlobalRes& operator =(const GlobalRes&) = delete;

	int players;
	bool isbgm = 1;
	int alivePeople;
	int isAlive[5] = { 0 };
public:
	static GlobalRes* getInstance();

	//设置玩家数量
	void setPlayers(int num);

	//第num号玩家死亡，如果死亡后只剩一个人，返回true，否则返回false
	bool died(int num);

	//设置是否播放BGM
	void setBGM(bool my_set);

	//返接口
	int getPlayers() { return players; }
	bool isBGM() { return isbgm; }
	int getAlivePeople() { return alivePeople; }
};
