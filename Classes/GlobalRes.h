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

	//�����������
	void setPlayers(int num);

	//��num��������������������ֻʣһ���ˣ�����true�����򷵻�false
	bool died(int num);

	//�����Ƿ񲥷�BGM
	void setBGM(bool my_set);

	//���ӿ�
	int getPlayers() { return players; }
	bool isBGM() { return isbgm; }
	int getAlivePeople() { return alivePeople; }
};
