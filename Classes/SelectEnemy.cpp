#include"SelectEnemy.h"
#include<random>
#include<algorithm>
#include<iostream>

using namespace std;

const int numPlayer = 4;//限定最多只有四个玩家

void SelectEnemy::init(int n) {
	number = n;//对number进行初始化
}

void SelectEnemy::died(int index)
{
	death.push_back(index);
	count = 0;//重新开始新一轮战斗，计数归0
}

string SelectEnemy::nextProbableEnemy() {

	if (count % (number - 1) == 0) {
		probableEnemy.clear();
		for (int i = 0; i < numPlayer; i++) {
			string str;
			auto it = find(death.begin(), death.end(), i + 1);
			if (it != death.end()) {
				for (int k = 0; k < number - 1; k++) {
					str += to_string(0);//如果当前编号为已死玩家，则加入"00.."字符串
				}
				probableEnemy.push_back(str);
				continue;
			}
			for (int j = 1; j < numPlayer + 1; j++) {
				if (j != i + 1) {
					auto it = find(death.begin(), death.end(), j);
					if (it != death.end()) {
						continue;
					}//不添加已死玩家编号
					str += to_string(j);
				}
			}
			probableEnemy.push_back(str);
		}
	}
	string str;
	for (int i = 0; i < numPlayer; i++) {
		str += probableEnemy[i];
	}
	return str;
}


string  SelectEnemy::battle() {
	//测试所用：cout << "第" << count << ":";
	if (count % (number - 1) != 0) {//判断当前计数是否需要进行新一轮信息的产生
		string str;
		int j = 0;
		for (; j < number / 2; j++) {
			str += information[count % (number - 1)][j];
			size_t found = probableEnemy[information[count % (number - 1)][j][0] - '0' - 1].find(information[count % (number - 1)][j][1]);
			if (found != string::npos) {
				// 删除找到的字符
				probableEnemy[information[count % (number - 1)][j][0] - '0' - 1].erase(found, 1);
				probableEnemy[information[count % (number - 1)][j][0] - '0' - 1].push_back('0');//添加0
			}
			else {
				cout << "没找到该字符";//给出错误提示
			}
			found = probableEnemy[information[count % (number - 1)][j][1] - '0' - 1].find(information[count % (number - 1)][j][0]);
			if (found != string::npos) {
				// 删除找到的字符
				probableEnemy[information[count % (number - 1)][j][1] - '0' - 1].erase(found, 1);
				probableEnemy[information[count % (number - 1)][j][1] - '0' - 1].push_back('0');
			}
			else {
				cout << "没找到该字符";
			}
		}
		if (number % 2) {
			str += information[count % (number - 1)][j];
		}//奇数特判
		count++;
		return str;
	}
	player.clear();
	information.clear();
	for (int i = 0; i < numPlayer; i++) {
		auto it = find(death.begin(), death.end(), i + 1);
		if (it == death.end()) {
			player.push_back(i + 1);//如果玩家已死则不需要添加编号
		}
	}
	for (int i = 0; i < number - 1; i++) {
		std::vector<std::string> enemy;//每轮中每个玩家对战的敌人
		for (int j = 0; j < number / 2; j++) {
			std::string str;
			str += std::to_string(player[j]);
			str += std::to_string(player[number - 1 - j]);
			enemy.push_back(str);
		}
		if (number % 2) {
			std::string str;
			str += std::to_string(player[number / 2]);
			str += std::to_string(player[number - 1]);
			str += 'b';
			enemy.push_back(str);
		}  //奇数进行镜像复制,用b来表示镜像复制，偶数则不需要
		int temp = player[number - 1];
		for (int j = number - 1; j > 1; j--) {
			player[j] = player[j - 1];
		}
		player[1] = temp;
		information.push_back(enemy);
	}
	std::random_device rd;
	std::mt19937 g(rd());

	// 使用 std::shuffle 进行洗牌
	std::shuffle(information.begin(), information.end(), g);
	int j = 0;
	string str;
	for (; j < number / 2; j++) {
		str += information[count % (number - 1)][j];
		size_t found = probableEnemy[information[count % (number - 1)][j][0] - '0' - 1].find(information[count % (number - 1)][j][1]);
		if (found != string::npos) {
			// 删除找到的字符
			probableEnemy[information[count % (number - 1)][j][0] - '0' - 1].erase(found, 1);
			probableEnemy[information[count % (number - 1)][j][0] - '0' - 1].push_back('0');
		}
		else {
			cout << "没找到该字符";
		}
		found = probableEnemy[information[count % (number - 1)][j][1] - '0' - 1].find(information[count % (number - 1)][j][0]);
		if (found != string::npos) {
			// 删除找到的字符
			probableEnemy[information[count % (number - 1)][j][1] - '0' - 1].erase(found, 1);
			probableEnemy[information[count % (number - 1)][j][1] - '0' - 1].push_back('0');
		}
		else {
			cout << "没找到该字符";
		}

	}
	if (number % 2) {
		str += information[count % (number - 1)][j];
	}
	count++;
	return  str;
}

//测试样例
/*
int main()
{
	SelectEnemy  b;
	for (int i = 0; i < 7; i++) {
		b.init(4);
		string a = b.nextProbableEnemy();
		string c = b.battle();
		for (int j = 0; j < c.length(); j++) {
			if (j == c.length() - 1 && c.length() % 2) {
				cout << c[j];
				break;
			}
			cout << c[j] - '0';
		}
		cout << "    ";
		for (int j = 0; j < a.length(); j++) {
			cout << a[j] - '0';
		}
		cout << endl;
	}
	b.died(1);
	for (int i = 0; i < 5; i++) {
		b.init(3);
		string a = b.nextProbableEnemy();
		string c = b.battle();
		for (int j = 0; j < c.length(); j++) {
			if (j == c.length() - 1 && c.length() % 2) {
				cout << c[j];
				break;
			}
			cout << c[j] - '0';
		}
		cout << "    ";
		for (int j = 0; j < a.length(); j++) {
			cout << a[j] - '0';
		}
		cout << endl;
	}
	b.died(3);
	for (int i = 0; i < 1; i++) {
		b.init(2);
		string a = b.nextProbableEnemy();
		string c = b.battle();
		for (int j = 0; j < c.length(); j++) {
			if (j == c.length() - 1 && c.length() % 2) {
				cout << c[j];
				break;
			}
			cout << c[j] - '0';
		}
		cout << "    ";
		for (int j = 0; j < a.length(); j++) {
			cout << a[j] - '0';
		}
		cout << endl;
	}
}*/
