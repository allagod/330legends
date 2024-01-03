#include"bar.h"
#include"cocos2d.h"

using namespace cocos2d;

bool HealthBar::init(float x, float y,const int &k) {
    if (!DrawNode::init()) {
        return false;
    }

    barHeight = 20.0f;

    // 定义血条的位置
    Pos_x = x;
    Pos_y = y;

    // 绘制整个血条

    hpBar = DrawNode::create();
    if(k==1)
        hpBar->drawSolidRect(Vec2(Pos_x,Pos_y), Vec2(Pos_x + barWidth, Pos_y - barHeight), Color4F(0, 1, 0, 1)); // 绿色血条
    else
        hpBar->drawSolidRect(Vec2(Pos_x, Pos_y), Vec2(Pos_x + barWidth, Pos_y - barHeight), Color4F(1, 0, 0, 1)); // 红色血条

    this->addChild(hpBar);


    for (int i = 1; i <= numSeparators; i++) {
        auto separatorLine = DrawNode::create();
        separatorLine->drawLine(Vec2(Pos_x + i * 50, Pos_y), Vec2(Pos_x + i * 50, Pos_y - barHeight), Color4F::BLACK);
        addChild(separatorLine);
    }

    return true;
}


void HealthBar::decreaseHealth(float amount,const int &k) {
    barWidth -= amount;

    if (barWidth < 0.0f) {
        barWidth = 0.0f;
    }

    numSeparators = barWidth / 50.0f;

    hpBar->clear();

    if (k == 1)
        hpBar->drawSolidRect(Vec2(Pos_x, Pos_y), Vec2(Pos_x + barWidth, Pos_y - barHeight), Color4F(0, 1, 0, 1)); // 重新绘制血条
    else
        hpBar->drawSolidRect(Vec2(Pos_x, Pos_y), Vec2(Pos_x + barWidth, Pos_y - barHeight), Color4F(1, 0, 0, 1)); // 重新绘制血条
}
