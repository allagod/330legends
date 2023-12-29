#pragma once
class GlobalRes
{
private:
	GlobalRes() {}
	GlobalRes(const GlobalRes&) = delete;
	GlobalRes& operator =(const GlobalRes&) = delete;

	int players; //存储玩家数量
	bool isbgm = 1;
	int alivePeople; //存储目前存活的玩家数量
	int isAlive[5] = { 0 }; //存储i号玩家是否存活
	bool isOnline = 0; //是否是联机模式，如果是联机模式，isOnline为1
	int period = 0;
public:
	static GlobalRes* getInstance();

	//设置玩家数量
	void setPlayers(int num);

	//第num号玩家死亡，如果死亡后只剩一个人，返回true，否则返回false
	bool died(int num);

	//设置是否播放BGM
	void setBGM(bool my_set);

	//设置当前是联机模式
	void setOnline();

	//返接口
	int getPlayers() { return players; }
	bool isBGM() { return isbgm; }
	int getAlivePeople() { return alivePeople; }
	bool IsOnline() { return isOnline; }
	void addPeriod();
};
