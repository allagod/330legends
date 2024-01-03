#ifndef __CHESSHERO_H__
#define __CHESSHERO_H__

#include "cocos2d.h"
#include "audio/include/AudioEngine.h"
#include"bar.h"
#include <mutex>
#include<thread>
#include <vector>


USING_NS_CC;

class ChessHero : public Sprite {
private:
    /*
    * 1.技能使用倒计时
    * 2.棋盘update（）扣血的时候
    * 3.<0的时候的一些判定跳出
    */
    int m_MP=0;

    float health_bar_scale;

    float health_bar_x;

    float health_bar_y;

    /*
     * 1.星级
     * 2.这个主要是属性的增幅作用
     */
    int m_grade=0;//星级

    /*
     * 1.初始设定，代表棋盘英雄的攻击动作帧有几张
     * 2.这个主要是用于生成周期定时器
     */
    int m_action_num = 0;
    int m_action_num_down = 0;

    float firing_range = 0;


public:

    /*
    * 1.初始化setScale()的时候的数值
    * 2.upgrade()升级以后要数字要变大
    */
    float m_SpriteTypeValue = 0;//精灵大小数值

    /*
    * 1.用于判断游戏结束，只要一方全部是1，就可以算作一个结束标志，然后时间局内时间结束是另一个结束标志，两个结束标志同时满足，就可以结束了
    * 2.棋盘英雄在死亡动画动画播放完毕时，这个设为1
    * 3.棋盘英雄在创建时，这个设为0
    */
    bool animation_end = 0;

    float m_defence = 1;

    /*
     * 1.用于棋盘update（）每一帧扣血
     */
    float m_SpritePowerValue=0;//精灵攻击力大小

    /*
    * 1.用于控制棋盘英雄的攻击动画播放速度
    * 2.用于控制远程英雄的子弹发射速度
    */
    float m_SpriteAttackSpeed=0;//精灵攻击速度

    /*
    * 1.用于在英雄的init（）函数里的schedule（）函数定时扣血
    */
    float skill_injury=0;

    /*
    * 1.由于棋盘的update（）在创始就开始执行，但是不能时刻都扣血，所以要给棋盘英雄设定一个攻击状态
    * 2.在自己的对手非空，自己还活着，对手正在自己的攻击范围内，就可以开启攻击状态
    * 3.在对手死亡或者自己死亡或者对手不在自己的攻击范围，就可以取消攻击状态
    */
    bool attack_status=0;

    /*
    * 1.用于在英雄的init（）函数里的schedule（）函数定时赋值为1，当值为1时，就可以扣技能值大小的血了
    */
    bool skill_status=0;

    /*
    * 1.创建血条
    * 2.棋盘update（）扣血的时候
    * 3.<0的时候的一些周期的计时器跳出
    */
    float m_HP = 0;

    bool position_status;

    bool end_flag = 0;
public:
    ChessHero();
    ~ChessHero();

    HealthBar* healthBar;

    /*
     * 1.由棋盘update在每一帧为每一个活着的人分配一个对手
     * 2.对手会决定棋盘英雄接下来的行为
     */
    ChessHero* enemy=nullptr;

    /*
     * 1.用于棋盘英雄在schedule决定接下来开移动的动画还是攻击的动画
     * 2.或许可以影响棋盘英雄在放上棋盘的时候产生的高亮的效果
     */
    float m_AttackRange;

    /*
     * 1.用于英雄所装装备的可视化
     */
    int weapon[3];

    void skill_make1(float dt);
    void skill_make2(float dt);
    void skill_make3(float dt);

    //发动攻击（对于射手就是shoot，对于近战就是chop，boxing,控制精灵发出的伤害，同样，需要考虑暴击、法术强度、攻击强度、暴击率）
    void makeAttack();

    void ChessHero_upgrade(const int &upgrade);

    void wear_weapon();

    //用于生成开火
    void fireProjectile();

    cocos2d::Vector<cocos2d::SpriteFrame*> getAnimation(const char* format, int count);

    //控制精灵动态更新的函数
    virtual void update(float dt);

    //控制精灵静态部分初始化
    virtual bool init(const std::string& filename, const int& k,const int &upgrade);

    static ChessHero* create(const std::string& filename, const int& k,const int &upgrade) {
        ChessHero* chessHero = new ChessHero();
        if (chessHero && chessHero->init(filename,k,upgrade)) {
            chessHero->autorelease();
            return chessHero;
        }
        else {
            delete chessHero;
            return nullptr;
        }
    }

    //控制棋盘英雄决定时移动还是攻击
    void attack1(float dt);

    void attack2(float dt);

    //
    virtual void  destroy(float dt);

};
#endif  //__CHESSHERO_H__
