#include"cocos2d.h"

using namespace cocos2d;

class HealthBar : public DrawNode {

private:
    float totalHealth;
    
    float barWidth;//�����Ŀ��,������ΪѪ���Ķ���

    float barHeight;//�����ĸ߶�

    int  Pos_x;

    int  Pos_y;

    Vec2 barPosition;

    DrawNode* hpBar;

    int numSeparators;//�ָ���

public:
	HealthBar (int a,int b){
		totoalHealth=a;
		barWidth=a;		
		numSeparators=b;
	}

    bool init(int x, int y);//���øú���������x,y����󾭹��仯��ɲ���Ѫ��
  
    // ����Ѫ��,����amount���۳�ָ����Ѫ��
    void decreaseHealth(float amount);

};
