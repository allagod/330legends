#pragma once
class GlobalRes
{
private:
	GlobalRes() {}
	GlobalRes(const GlobalRes&) = delete;
	GlobalRes& operator =(const GlobalRes&) = delete;

	int players; //�洢�������
	bool isbgm = 1;
	int alivePeople; //�洢Ŀǰ�����������
	int isAlive[5] = { 0 }; //�洢i������Ƿ���
	bool isOnline = 0; //�Ƿ�������ģʽ�����������ģʽ��isOnlineΪ1
	int period = 0;
public:
	static GlobalRes* getInstance();

	//�����������
	void setPlayers(int num);

	//��num��������������������ֻʣһ���ˣ�����true�����򷵻�false
	bool died(int num);

	//�����Ƿ񲥷�BGM
	void setBGM(bool my_set);

	//���õ�ǰ������ģʽ
	void setOnline();

	//���ӿ�
	int getPlayers() { return players; }
	bool isBGM() { return isbgm; }
	int getAlivePeople() { return alivePeople; }
	bool IsOnline() { return isOnline; }
	void addPeriod();
};
