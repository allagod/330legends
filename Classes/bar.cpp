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

    // 定义血条的位置
    barPosition(x, y);

    // 绘制整个血条

   hpBar = DrawNode::create();

    hpBar->drawSolidRect(barPosition, Vec2(barPosition.x + barWidth, barPosition.y - barHeight), Color4F(0, 1, 0, 1)); // 绿色血条
    this->addChild(hpBar);

    Pos_x=barPosition.x;
    Pos_y=barPosition.y;

    // 绘制黑色分隔线
    numSeparators = 3; // 三条分割线,即四格血

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

    hpBar->clear(); // 清除原来的血条
    hpBar->drawSolidRect(barPosition, Vec2(Pos_x + barWidth, Pos_y - barHeight), Color4F(0, 1, 0, 1)); // 重新绘制血条

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

    // 使用当前时间作为随机数种子
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // 产生100-300范围内的随机数
    int health = std::rand() % 201 + 100; // 0到200之间的随机数再加上100

    std::string healthString = std::to_string(health); // 将整数转换为字符串

    auto label = Label::createWithTTF(healthString, "fonts/arial.ttf", 24); // 创建一个艺术字 Label
    label->setPosition(Vec2(x,y)); // 设置文本的位置为 (x, y)
    this->addChild(label); // 添加到场景中
            
    label->setTextColor(Color4B(255, 0, 0, 255)); // 设置文本颜色为红色

    label->setScale(2.0f); // 设置初始大小为2倍

    // 创建一个 ScaleTo 动作，从当前大小（2倍）缩放到最终大小（0.5倍），并在2秒内完成
    auto scaleAction = ScaleTo::create(2.0f, 0.5f);

    // 让 label 执行这个动作
    label->runAction(scaleAction);

    auto fadeOutAction = FadeOut::create(1.0f);

    // 执行 FadeOut 动作
    label->runAction(fadeOutAction);
}

void generateInformation(float x,float y,std::string &a,std::string &b,std::string&c)
{
    auto listener = EventListenerMouse::create();


    /*std::string health = "Health: 100\n";
    std::string name = "Name: Player\n";
    std::string defense = "Defense: 50\n";*/

    std::string allInfo = a + b + c;

    // 创建标签并设置内容
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
                hideSpriteInfo(); // 隐藏精灵信息
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