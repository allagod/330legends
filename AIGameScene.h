#pragma once
#ifndef __AI_GAME_SCENE_H__
#define __AI_GAME_SCENE_H__
#include "cocos2d.h"
//#include "Player.h"
#include"Chessboard.h"
#include <vector>

using namespace cocos2d;
using namespace std;
/*
* AI对战：1打1  胜负判断 yg
本源文件完成人机对战场景的实现

备战阶段： 
最上方 显示进度条（代表剩余时间，可以加上数字的变化显示在旁边）  设置倒计时  倒计时结束就切换战斗场景

1。点击卡片会在备战区显示对应英雄，然后卡片消失
2。可以拖动备战区的英雄到达指定单元格（点击移动后就会有棋盘格显示，松开则会消失，
移动到指定位置时会有变色单元格显示；位置非法需要返回）  最后需要返回整个棋盘的英雄布局坐标信息；

两个按钮  （都是满足金币要求时，回调函数有效）
刷新（回调函数完成，获取本次刷新后的卡牌信息， 移除当前商城中的所有剩余卡片，并动态显示新加入的卡片，从右侧诞生快速移到对应的位置）  
购买经验 （回调函数完成，更新经验，金币减少（需要显示数字变化），获取等级情况：如果变化需要显示）

商城区：初始获取卡牌信息情况，直接显示五张卡牌，点击对应位置的卡牌（只有当当前金币数大于该卡牌的金币数时，回调函数有效：对应位置卡牌消失，金币减少并显示减少后的金币数）

装备区：//初始为空，击打野怪结束后爆装备，小小英雄触碰后，移动到装备区  zzy/yg
    目前先写死就三个吧（拖动到指定位置后消失，然后属性值变化）

需要记录我的棋子和坐标信息，还有如何交换信息？ yg

战斗阶段：
（联机对局时不显示其他队友信息，只在小小英雄处跟随显示昵称即可；对局输 显示失败，点击退出返回上一个界面  对局赢显示进入下一局，最终获胜显示胜利，判断胜负？）
 装备与英雄拖动无效，其他均有效
 加上杨光的部分即可


 结算阶段


如何获得窗口上某一点的坐标？

*/

/*
本源文件完成游戏对战场景的功能实现 :
游戏场景实现：人机对战 与 联机对战分开写


预备函数：鼠标坐标转棋盘坐标
、、
备战席是第0行 然后其他坐标是从1开始
备战席第一格就是(0,1)
对战席就是从(1,1)->(4,7)

商城：
刷新商城按钮  升级按钮（点击后经验增加，满经验自动升级，显示当前等级）
商城显示（点击某位置所处的区域卡片消失，并购买（注意判断当前金币是否足够购买），
然后备战席增加一个，金币减少，点击刷新时四张卡片更新显示，含有刷新时的动画）；
金币显示（跟随变化）

装备区：
预备棋子区：
完成在备战环节棋子、以及装备的放置，实现鼠标点击拖动到合法位置时的显示与不合法位置的返回
拖动时会有棋盘格的显示；

对战棋盘区：杨光

信息显示区：
上方：倒计时显示（或者显示进度条） 当前战局胜负情况
右侧：玩家信息（昵称，血条，下一局对战对手）

一、备战环节：
1.章卓源： 击打野怪，然后出现水晶   小小英雄触碰后爆装备，并显示然后移动到装备区（状态更新）
2.商城区 预备棋子区 装备区

二、战斗环节：
对战棋盘区

三、结算环节：
最终结算

 羁绊（先搁置）
*/

struct Position {
    int x;
    int y;
};

class AIGameScene : public cocos2d::Layer {
private:
    cocos2d::Label* label;

    cocos2d::Label* Coinslabel;
    cocos2d::Label* Levellabel;

    cocos2d::Label* countdownLabel;
    cocos2d::ProgressTimer* countdownTimer;
    float remainingTime=30;
    float total_time=30;

    cocos2d::Sprite* Board_selected;
    cocos2d::Sprite* Boardbackground;
    cocos2d::Sprite* clearLogo;

    vector<cocos2d::Sprite*> Equipment;//储存三个装备
    vector<cocos2d::Sprite*> Card;//储存当前四个卡牌
    vector<cocos2d::Sprite*> Chess;//储存备战区棋子

    EventDispatcher* eventDispatcher;
    //监听器
    vector<EventListenerTouchOneByOne*> touchlistenerCard;
    vector<EventListenerTouchOneByOne*> touchlistenerChess;
    vector<EventListenerTouchOneByOne*> touchlistenerEquipment;


    Vec2 EuipRetPos;
    Vec2 ChessRetPos;


    std::string  cardInfo;  //从Player中类获取  marketRefresh();
    std::string  equipmentInfo;  

    std::string boardInfo; //储存当前棋盘的棋子以及对应的位置坐标（需要包含棋子等级信息、装备吗）
   
    

    int coins=4;  //金币数  获取： getCoin（）  初始时为4，当有出售（增加）、购买（减少）、打完野怪/对战结束获胜 调用函数+4
    int level=1;  //等级   获取: Player类  int getLevel() { return level; }
    //int preStartPos = 0;


public:
     //void startCountdown(float duration);
     //void updateCountdown(float dt);

    virtual bool init();
    static Scene* scene();

    void onMouseDown(cocos2d::EventMouse* event);

    bool touchBegan(cocos2d::Touch* touch, cocos2d::Event* event);//触摸开始
    void touchMoved(cocos2d::Touch* touch, cocos2d::Event* event);//触摸中
    void touchEnded(cocos2d::Touch* touch, cocos2d::Event* event);//触摸结束

    bool c_touchBegan(cocos2d::Touch* touch, cocos2d::Event* event);//卡片触摸开始

    void onRefBtnClicked(Ref* sender);

    void onUpBtnClicked(Ref* sender);

    Vec2 Tranverse(Vec2 v);

    // 场景进入时开始计时
    virtual void onEnter() override {
        Layer::onEnter();
        // 在进入场景时开始计时
        unschedule(CC_SCHEDULE_SELECTOR(AIGameScene::updateTimer));  // 先停止计时器，避免重复启动
        schedule(CC_SCHEDULE_SELECTOR(AIGameScene::updateTimer), 1.0f, 30, 0.0f);
    }

    //// 场景退出时停止计时
    //virtual void onExit() override {
    //    Layer::onExit();
    //    // 在退出场景时停止计时
    //    unschedule(CC_SCHEDULE_SELECTOR(AIGameScene::updateTimer));
    //}

    // 场景切换完毕后重新设置计时器的初始值
    virtual void onEnterTransitionDidFinish() override {
        Layer::onEnterTransitionDidFinish();
        // 在场景切换完毕后重新设置计时器的初始值
        remainingTime = 30;  // 设置初始倒计时时间
        countdownTimer->setPercentage(100);
    }

    // 更新计时器
    void updateTimer(float dt) {
        remainingTime -= 1.0f;
        // 更新倒计时文本
        countdownLabel->setString(StringUtils::format("Countdown: %.0f", remainingTime));
        // 更新进度条
        float percentage = (remainingTime / total_time) * 100;
        // 直接更新进度条的百分比
        countdownTimer->setPercentage(percentage);
        if (remainingTime <= 0) {
            unschedule(CC_SCHEDULE_SELECTOR(AIGameScene::updateTimer));
            //切换到杨光那里的场景
            //Director::getInstance()->popScene();
            Director::getInstance()->pushScene(this->scene());
            Director::getInstance()->replaceScene(TransitionFade::create(1.0, Chessboard::createScene()));
        }
    }
    

    CREATE_FUNC(AIGameScene);// implement the "static create()" method manually
};

#endif 