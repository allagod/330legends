#include"cocos2d.h"

using namespace cocos2d;

class HealthBar : public DrawNode {

private:
    float totalHealth;
    
    float barWidth;//条带的宽度,可以认为血量的多少

    float barHeight;//条带的高度

    int  Pos_x;

    int  Pos_y;

    Vec2 barPosition;

    DrawNode* hpBar;

    int numSeparators;//分割线

public:
	HealthBar (int a,int b){
		totoalHealth=a;
		barWidth=a;		
		numSeparators=b;
	}

    bool init(int x, int y);//调用该函数，传入x,y坐标后经过变化后可产生血条
  
    // 减少血量,传入amount，扣除指定的血量
    void decreaseHealth(float amount);

};
