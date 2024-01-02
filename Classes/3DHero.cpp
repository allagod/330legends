#include"bar.h"
#include"cocos2d.h"

using namespace cocos2d;

bool HealthBar::init(float x, float y) {
    if (!DrawNode::init()) {
        return false;
    }
    //测试样例 
    //totalHealth = 200.0f;

    //barWidth = 200.0f;

    barHeight = 20.0f;

    // 定义血条的位置
    Pos_x = x;
    Pos_y = y;

    // 绘制整个血条

    hpBar = DrawNode::create();

    hpBar->drawSolidRect(Vec2(Pos_x,Pos_y), Vec2(Pos_x + barWidth, Pos_y - barHeight), Color4F(0, 1, 0, 1)); // 绿色血条
    this->addChild(hpBar);

   

    // 绘制黑色分隔线
    //numSeparators = 3; // 三条分割线,即四格血

    //for (int i = 1; i <= numSeparators; ++i) {
        //DrawNode* separator = DrawNode::create();
      //  float separatorPositionX = barPosition.x + i * (barWidth / (numSeparators + 1));
       // separator->drawLine(Vec2(separatorPositionX, barPosition.y),Vec2(separatorPositionX, barPosition.y - barHeight),Color4F::BLACK);
     //   this->addChild(separator);
   // }

    for (int i = 1; i <= numSeparators; i++) {
        auto separatorLine = DrawNode::create();
        separatorLine->drawLine(Vec2(Pos_x + i * 50, Pos_y), Vec2(Pos_x + i * 50, Pos_y - barHeight), Color4F::BLACK);
        addChild(separatorLine);
    }

    return true;
}


void HealthBar::decreaseHealth(float amount) {
    //amount = 20;

    barWidth -= amount;
    if (barWidth < 0.0f) {
        barWidth = 0.0f;
    }

    numSeparators = barWidth / 50.0f;

    hpBar->clear(); // 清除原来的血条
    // 移除之前的黑线 
   
    hpBar->drawSolidRect(Vec2(Pos_x, Pos_y), Vec2(Pos_x + barWidth, Pos_y - barHeight), Color4F(0, 1, 0, 1)); // 重新绘制血条

    for (int i = 1; i <= numSeparators; ++i) {
        auto separatorLine = DrawNode::create();
        separatorLine->drawLine(Vec2(Pos_x + i * 50, Pos_y), Vec2(Pos_x + i * 50, Pos_y- barHeight), Color4F::BLACK);
        addChild(separatorLine);
    }

}
