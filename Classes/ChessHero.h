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
    * 1.����ʹ�õ���ʱ
    * 2.����update������Ѫ��ʱ��
    * 3.<0��ʱ���һЩ�ж�����
    */
    int m_MP=0;

    float health_bar_scale;

    float health_bar_x;

    float health_bar_y;

    /*
     * 1.�Ǽ�
     * 2.�����Ҫ�����Ե���������
     */
    int m_grade=0;//�Ǽ�

    /*
     * 1.��ʼ�趨����������Ӣ�۵Ĺ�������֡�м���
     * 2.�����Ҫ�������������ڶ�ʱ��
     */
    int m_action_num = 0;
    int m_action_num_down = 0;

    float firing_range = 0;


public:

    /*
    * 1.��ʼ��setScale()��ʱ�����ֵ
    * 2.upgrade()�����Ժ�Ҫ����Ҫ���
    */
    float m_SpriteTypeValue = 0;//�����С��ֵ

    /*
    * 1.�����ж���Ϸ������ֻҪһ��ȫ����1���Ϳ�������һ��������־��Ȼ��ʱ�����ʱ���������һ��������־������������־ͬʱ���㣬�Ϳ��Խ�����
    * 2.����Ӣ�����������������������ʱ�������Ϊ1
    * 3.����Ӣ���ڴ���ʱ�������Ϊ0
    */
    bool animation_end = 0;

    float m_defence = 1;

    /*
     * 1.��������update����ÿһ֡��Ѫ
     */
    float m_SpritePowerValue=0;//���鹥������С

    /*
    * 1.���ڿ�������Ӣ�۵Ĺ������������ٶ�
    * 2.���ڿ���Զ��Ӣ�۵��ӵ������ٶ�
    */
    float m_SpriteAttackSpeed=0;//���鹥���ٶ�

    /*
    * 1.������Ӣ�۵�init�����������schedule����������ʱ��Ѫ
    */
    float skill_injury=0;

    /*
    * 1.�������̵�update�����ڴ�ʼ�Ϳ�ʼִ�У����ǲ���ʱ�̶���Ѫ������Ҫ������Ӣ���趨һ������״̬
    * 2.���Լ��Ķ��ַǿգ��Լ������ţ����������Լ��Ĺ�����Χ�ڣ��Ϳ��Կ�������״̬
    * 3.�ڶ������������Լ��������߶��ֲ����Լ��Ĺ�����Χ���Ϳ���ȡ������״̬
    */
    bool attack_status=0;

    /*
    * 1.������Ӣ�۵�init�����������schedule����������ʱ��ֵΪ1����ֵΪ1ʱ���Ϳ��Կۼ���ֵ��С��Ѫ��
    */
    bool skill_status=0;

    /*
    * 1.����Ѫ��
    * 2.����update������Ѫ��ʱ��
    * 3.<0��ʱ���һЩ���ڵļ�ʱ������
    */
    float m_HP = 0;

    bool position_status;

    bool end_flag = 0;
public:
    ChessHero();
    ~ChessHero();

    HealthBar* healthBar;

    /*
     * 1.������update��ÿһ֡Ϊÿһ�����ŵ��˷���һ������
     * 2.���ֻ��������Ӣ�۽���������Ϊ
     */
    ChessHero* enemy=nullptr;

    /*
     * 1.��������Ӣ����schedule�������������ƶ��Ķ������ǹ����Ķ���
     * 2.�������Ӱ������Ӣ���ڷ������̵�ʱ������ĸ�����Ч��
     */
    float m_AttackRange;

    /*
     * 1.����Ӣ����װװ���Ŀ��ӻ�
     */
    int weapon[3];

    void skill_make1(float dt);
    void skill_make2(float dt);
    void skill_make3(float dt);

    //�����������������־���shoot�����ڽ�ս����chop��boxing,���ƾ��鷢�����˺���ͬ������Ҫ���Ǳ���������ǿ�ȡ�����ǿ�ȡ������ʣ�
    void makeAttack();

    void ChessHero_upgrade(const int &upgrade);

    void wear_weapon();

    //�������ɿ���
    void fireProjectile();

    cocos2d::Vector<cocos2d::SpriteFrame*> getAnimation(const char* format, int count);

    //���ƾ��鶯̬���µĺ���
    virtual void update(float dt);

    //���ƾ��龲̬���ֳ�ʼ��
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

    //��������Ӣ�۾���ʱ�ƶ����ǹ���
    void attack1(float dt);

    void attack2(float dt);

    //
    virtual void  destroy(float dt);

};
#endif  //__CHESSHERO_H__
