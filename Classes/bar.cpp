#include"bar.h"
#include"cocos2d.h"

using namespace cocos2d;

bool HealthBar::init(int x,int y){
    if (!DrawNode::init()) {
        return false;
    }

    totalHealth = 200.0f;

    barWidth = 200.0f;

    barHeight = 20.0f;

    // ����Ѫ����λ��
    barPosition(x, y);

    // ��������Ѫ��

   hpBar = DrawNode::create();

    hpBar->drawSolidRect(barPosition, Vec2(barPosition.x + barWidth, barPosition.y - barHeight), Color4F(0, 1, 0, 1)); // ��ɫѪ��
    this->addChild(hpBar);

    Pos_x=barPosition.x;
    Pos_y=barPosition.y;

    // ���ƺ�ɫ�ָ���
    numSeparators = 3; // �����ָ���,���ĸ�Ѫ

    for (int i = 1; i <= numSeparators; ++i) {
        DrawNode* separator = DrawNode::create();
        float separatorPositionX = barPosition.x + i * (barWidth / (numSeparators + 1));
        separator->drawLine(Vec2(separatorPositionX, barPosition.y),Vec2(separatorPositionX, barPosition.y - barHeight),Color4F::BLACK);
        this->addChild(separator);
    }

    return true;
}


void HealthBar::decreaseHealth(float amount){
    barWidth -= amount;
    if (barWidth < 0.0f) {
        barWidth = 0.0f;
    }
    
    numSeparators = barWidth / 50.0f;

    hpBar->clear(); // ���ԭ����Ѫ��
    hpBar->drawSolidRect(barPosition, Vec2(Pos_x + barWidth, Pos_y - barHeight), Color4F(0, 1, 0, 1)); // ���»���Ѫ��

    for (int i = 1; i <= numSeparators; ++i) {
        DrawNode* separator = DrawNode::create();
        float separatorPositionX = barPosition.x + i * (barWidth / (numSeparators + 1));
        separator->drawLine(Vec2(separatorPositionX, barPosition.y), Vec2(separatorPositionX, barPosition.y - barHeight), Color4F::BLACK);
        this->addChild(separator);
    }

}

#include <cstdlib>
#include <ctime>

void  generateValue(float x,float y){

    // ʹ�õ�ǰʱ����Ϊ���������
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // ����100-300��Χ�ڵ������
    int health = std::rand() % 201 + 100; // 0��200֮���������ټ���100

    std::string healthString = std::to_string(health); // ������ת��Ϊ�ַ���

    auto label = Label::createWithTTF(healthString, "fonts/arial.ttf", 24); // ����һ�������� Label
    label->setPosition(Vec2(x,y)); // �����ı���λ��Ϊ (x, y)
    this->addChild(label); // ��ӵ�������
            
    label->setTextColor(Color4B(255, 0, 0, 255)); // �����ı���ɫΪ��ɫ

    label->setScale(2.0f); // ���ó�ʼ��СΪ2��

    // ����һ�� ScaleTo �������ӵ�ǰ��С��2�������ŵ����մ�С��0.5����������2�������
    auto scaleAction = ScaleTo::create(2.0f, 0.5f);

    // �� label ִ���������
    label->runAction(scaleAction);

    auto fadeOutAction = FadeOut::create(1.0f);

    // ִ�� FadeOut ����
    label->runAction(fadeOutAction);
}

void generateInformation(float x,float y,std::string &a,std::string &b,std::string&c)
{
    auto listener = EventListenerMouse::create();


    /*std::string health = "Health: 100\n";
    std::string name = "Name: Player\n";
    std::string defense = "Defense: 50\n";*/

    std::string allInfo = a + b + c;

    // ������ǩ����������
    label = Label::createWithTTF(allInfo, "fonts/arial.ttf", 24);
    label->setPosition(Vec2(100, 100));
    this->addChild(label);
    label->setVisible(false);
      

    listener->onMouseMove = [=](Event* event) {
        EventMouse* e = dynamic_cast<EventMouse*>(event);
        if (e) {
            Vec2 mousePos = Vec2(e->getCursorX(), e->getCursorY());
            Vec2 mouseSpritePos = convertToNodeSpace(mousePos);
            if (sprite->getBoundingBox().containsPoint(mouseSpritePos)) {
                showSpriteInfo();
            }
            else {
                hideSpriteInfo(); // ���ؾ�����Ϣ
            }
        }
    };
}

void HelloWorld::hideSpriteInfo()
{
    label->setVisible(false);


}

void HelloWorld::showSpriteInfo()
{
    label->setVisible(true);
}