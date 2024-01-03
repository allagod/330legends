#ifndef __BAR_H__
#define __BAR_H__
#include"cocos2d.h"

using namespace cocos2d;

class HealthBar : public DrawNode {

private:
    float totalHealth;

    float barWidth;//条带的宽度,可以认为血量的多少

    float barHeight;//条带的高度

    int  Pos_x;

    int  Pos_y;


    DrawNode* hpBar;

    int numSeparators;//分割线

public:
    //a代表血量，如200，b=(a/50-1)代表黑线
    HealthBar(float a) {
        totalHealth = a;
        barWidth = a;
        numSeparators = (a / 50 - 1);
    }

    bool init(float x, float y,const int &k);//调用该函数，传入x,y坐标后经过变化后可产生血条

    // 减少血量,传入amount，扣除指定的血量
    void decreaseHealth(float amount, const int& k);

};
#endif // __BAR_H__
