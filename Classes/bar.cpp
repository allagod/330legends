#include"bar.h"
#include"cocos2d.h"

using namespace cocos2d;

bool HealthBar::init(float x, float y) {
    if (!DrawNode::init()) {
        return false;
    }
    //�������� 
    //totalHealth = 200.0f;

    //barWidth = 200.0f;

    barHeight = 20.0f;

    // ����Ѫ����λ��
    Pos_x = x;
    Pos_y = y;

    // ��������Ѫ��

    hpBar = DrawNode::create();

    hpBar->drawSolidRect(Vec2(Pos_x,Pos_y), Vec2(Pos_x + barWidth, Pos_y - barHeight), Color4F(0, 1, 0, 1)); // ��ɫѪ��
    this->addChild(hpBar);

   

    // ���ƺ�ɫ�ָ���
    //numSeparators = 3; // �����ָ���,���ĸ�Ѫ

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

    hpBar->clear(); // ���ԭ����Ѫ��
    // �Ƴ�֮ǰ�ĺ��� 
   
    hpBar->drawSolidRect(Vec2(Pos_x, Pos_y), Vec2(Pos_x + barWidth, Pos_y - barHeight), Color4F(0, 1, 0, 1)); // ���»���Ѫ��

    for (int i = 1; i <= numSeparators; ++i) {
        auto separatorLine = DrawNode::create();
        separatorLine->drawLine(Vec2(Pos_x + i * 50, Pos_y), Vec2(Pos_x + i * 50, Pos_y- barHeight), Color4F::BLACK);
        addChild(separatorLine);
    }

}
