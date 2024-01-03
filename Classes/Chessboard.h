#ifndef __CHESSBOARD_H__
#define __CHESSBOARD_H__

#include <vector>
#include "cocos2d.h"
#include "ChessHero.h"

class Chessboard : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    bool init();

    void update(float dt);

    //用于接收AI（）与服务器（）传入的字符串指令
    //棋子的输入，敌人的输入
    void MessagePutIn();

    //用于接受服务器创造战斗棋子的信息
    void ChessHero_create();

    //用于展示棋子战斗过程中暴击伤害的数值
    void generateValue(float dt);

    //在战斗中触摸后提供棋子的信息
    void generateInformation(std::string& a, std::string& b, std::string& c);

    //信息菜单的隐藏
    void hideSpriteInfo(int i);

    //信息菜单的显示
    void showSpriteInfo(int i);

    //切换战斗层与备战层的接口
    void  exitLayer();

    //开始倒计时
    void startCountdown(float duration);

    //每秒更新倒计时
    void updateCountdown(float dt);

    //为战斗层上每一个棋子分配敌人
    void BattleMatch();
 
    // implement the "static create()" method manually
    CREATE_FUNC(Chessboard);

private:
    std::vector<ChessHero*> player;//战斗层上我方棋子英雄数组

    std::vector<ChessHero*> enemy;//战斗层上敌方棋子英雄数组

    int player_num = 0;//我方棋盘英雄人数
    int enemy_num = 0;//敌方棋盘英雄人数

    int end_status = 0;//战斗层终止、切换标志

    float remainingTime = 30.0;//战斗页面倒计时计时器

    cocos2d::Label* countdownLabel;//倒计时菜单

    std::vector<cocos2d::Label*> chesshero_label;//棋子英雄的信息菜单

    cocos2d::ProgressTimer* countdownTimer;//倒计时器
};

#endif // __HELLOWORLD_SCENE_H__
