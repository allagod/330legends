#include"SelectEnemy.h"
#include<random>
#include<algorithm>
#include<iostream>

using namespace std;

const int numPlayer = 4;//�޶����ֻ���ĸ����

void SelectEnemy::init(int n) {
	number = n;//��number���г�ʼ��
}

void SelectEnemy::died(int index)
{
	death.push_back(index);
	count = 0;//���¿�ʼ��һ��ս����������0
}

string SelectEnemy::nextProbableEnemy() {

	if (count % (number - 1) == 0) {
		probableEnemy.clear();
		for (int i = 0; i < numPlayer; i++) {
			string str;
			auto it = find(death.begin(), death.end(), i + 1);
			if (it != death.end()) {
				for (int k = 0; k < number - 1; k++) {
					str += to_string(0);//�����ǰ���Ϊ������ң������"00.."�ַ���
				}
				probableEnemy.push_back(str);
				continue;
			}
			for (int j = 1; j < numPlayer + 1; j++) {
				if (j != i + 1) {
					auto it = find(death.begin(), death.end(), j);
					if (it != death.end()) {
						continue;
					}//�����������ұ��
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
	//�������ã�cout << "��" << count << ":";
	if (count % (number - 1) != 0) {//�жϵ�ǰ�����Ƿ���Ҫ������һ����Ϣ�Ĳ���
		string str;
		int j = 0;
		for (; j < number / 2; j++) {
			str += information[count % (number - 1)][j];
			size_t found = probableEnemy[information[count % (number - 1)][j][0] - '0' - 1].find(information[count % (number - 1)][j][1]);
			if (found != string::npos) {
				// ɾ���ҵ����ַ�
				probableEnemy[information[count % (number - 1)][j][0] - '0' - 1].erase(found, 1);
				probableEnemy[information[count % (number - 1)][j][0] - '0' - 1].push_back('0');//���0
			}
			else {
				cout << "û�ҵ����ַ�";//����������ʾ
			}
			found = probableEnemy[information[count % (number - 1)][j][1] - '0' - 1].find(information[count % (number - 1)][j][0]);
			if (found != string::npos) {
				// ɾ���ҵ����ַ�
				probableEnemy[information[count % (number - 1)][j][1] - '0' - 1].erase(found, 1);
				probableEnemy[information[count % (number - 1)][j][1] - '0' - 1].push_back('0');
			}
			else {
				cout << "û�ҵ����ַ�";
			}
		}
		if (number % 2) {
			str += information[count % (number - 1)][j];
		}//��������
		count++;
		return str;
	}
	player.clear();
	information.clear();
	for (int i = 0; i < numPlayer; i++) {
		auto it = find(death.begin(), death.end(), i + 1);
		if (it == death.end()) {
			player.push_back(i + 1);//��������������Ҫ��ӱ��
		}
	}
	for (int i = 0; i < number - 1; i++) {
		std::vector<std::string> enemy;//ÿ����ÿ����Ҷ�ս�ĵ���
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
		}  //�������о�����,��b����ʾ�����ƣ�ż������Ҫ
		int temp = player[number - 1];
		for (int j = number - 1; j > 1; j--) {
			player[j] = player[j - 1];
		}
		player[1] = temp;
		information.push_back(enemy);
	}
	std::random_device rd;
	std::mt19937 g(rd());

	// ʹ�� std::shuffle ����ϴ��
	std::shuffle(information.begin(), information.end(), g);
	int j = 0;
	string str;
	for (; j < number / 2; j++) {
		str += information[count % (number - 1)][j];
		size_t found = probableEnemy[information[count % (number - 1)][j][0] - '0' - 1].find(information[count % (number - 1)][j][1]);
		if (found != string::npos) {
			// ɾ���ҵ����ַ�
			probableEnemy[information[count % (number - 1)][j][0] - '0' - 1].erase(found, 1);
			probableEnemy[information[count % (number - 1)][j][0] - '0' - 1].push_back('0');
		}
		else {
			cout << "û�ҵ����ַ�";
		}
		found = probableEnemy[information[count % (number - 1)][j][1] - '0' - 1].find(information[count % (number - 1)][j][0]);
		if (found != string::npos) {
			// ɾ���ҵ����ַ�
			probableEnemy[information[count % (number - 1)][j][1] - '0' - 1].erase(found, 1);
			probableEnemy[information[count % (number - 1)][j][1] - '0' - 1].push_back('0');
		}
		else {
			cout << "û�ҵ����ַ�";
		}

	}
	if (number % 2) {
		str += information[count % (number - 1)][j];
	}
	count++;
	return  str;
}

//��������
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
