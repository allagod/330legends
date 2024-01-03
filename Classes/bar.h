#ifndef __BAR_H__
#define __BAR_H__
#include"cocos2d.h"

using namespace cocos2d;

class HealthBar : public DrawNode {

private:
    float totalHealth;

    float barWidth;//�����Ŀ��,������ΪѪ���Ķ���

    float barHeight;//�����ĸ߶�

    int  Pos_x;

    int  Pos_y;


    DrawNode* hpBar;

    int numSeparators;//�ָ���

public:
    //a����Ѫ������200��b=(a/50-1)�������
    HealthBar(float a) {
        totalHealth = a;
        barWidth = a;
        numSeparators = (a / 50 - 1);
    }

    bool init(float x, float y,const int &k);//���øú���������x,y����󾭹��仯��ɲ���Ѫ��

    // ����Ѫ��,����amount���۳�ָ����Ѫ��
    void decreaseHealth(float amount, const int& k);

};
#endif // __BAR_H__
