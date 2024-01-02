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
* AI��ս��1��1  ʤ���ж� yg
��Դ�ļ�����˻���ս������ʵ��

��ս�׶Σ� 
���Ϸ� ��ʾ������������ʣ��ʱ�䣬���Լ������ֵı仯��ʾ���Աߣ�  ���õ���ʱ  ����ʱ�������л�ս������

1�������Ƭ���ڱ�ս����ʾ��ӦӢ�ۣ�Ȼ��Ƭ��ʧ
2�������϶���ս����Ӣ�۵���ָ����Ԫ�񣨵���ƶ���ͻ������̸���ʾ���ɿ������ʧ��
�ƶ���ָ��λ��ʱ���б�ɫ��Ԫ����ʾ��λ�÷Ƿ���Ҫ���أ�  �����Ҫ�����������̵�Ӣ�۲���������Ϣ��

������ť  ������������Ҫ��ʱ���ص�������Ч��
ˢ�£��ص�������ɣ���ȡ����ˢ�º�Ŀ�����Ϣ�� �Ƴ���ǰ�̳��е�����ʣ�࿨Ƭ������̬��ʾ�¼���Ŀ�Ƭ�����Ҳ൮�������Ƶ���Ӧ��λ�ã�  
������ ���ص�������ɣ����¾��飬��Ҽ��٣���Ҫ��ʾ���ֱ仯������ȡ�ȼ����������仯��Ҫ��ʾ��

�̳�������ʼ��ȡ������Ϣ�����ֱ����ʾ���ſ��ƣ������Ӧλ�õĿ��ƣ�ֻ�е���ǰ��������ڸÿ��ƵĽ����ʱ���ص�������Ч����Ӧλ�ÿ�����ʧ����Ҽ��ٲ���ʾ���ٺ�Ľ������

װ������//��ʼΪ�գ�����Ұ�ֽ�����װ����ССӢ�۴������ƶ���װ����  zzy/yg
    Ŀǰ��д���������ɣ��϶���ָ��λ�ú���ʧ��Ȼ������ֵ�仯��

��Ҫ��¼�ҵ����Ӻ�������Ϣ��������ν�����Ϣ�� yg

ս���׶Σ�
�������Ծ�ʱ����ʾ����������Ϣ��ֻ��ССӢ�۴�������ʾ�ǳƼ��ɣ��Ծ��� ��ʾʧ�ܣ�����˳�������һ������  �Ծ�Ӯ��ʾ������һ�֣����ջ�ʤ��ʾʤ�����ж�ʤ������
 װ����Ӣ���϶���Ч����������Ч
 �������Ĳ��ּ���


 ����׶�


��λ�ô�����ĳһ������ꣿ

*/

/*
��Դ�ļ������Ϸ��ս�����Ĺ���ʵ�� :
��Ϸ����ʵ�֣��˻���ս �� ������ս�ֿ�д


Ԥ���������������ת��������
����
��սϯ�ǵ�0�� Ȼ�����������Ǵ�1��ʼ
��սϯ��һ�����(0,1)
��սϯ���Ǵ�(1,1)->(4,7)

�̳ǣ�
ˢ���̳ǰ�ť  ������ť������������ӣ��������Զ���������ʾ��ǰ�ȼ���
�̳���ʾ�����ĳλ������������Ƭ��ʧ��������ע���жϵ�ǰ����Ƿ��㹻���򣩣�
Ȼ��սϯ����һ������Ҽ��٣����ˢ��ʱ���ſ�Ƭ������ʾ������ˢ��ʱ�Ķ�������
�����ʾ������仯��

װ������
Ԥ����������
����ڱ�ս�������ӡ��Լ�װ���ķ��ã�ʵ��������϶����Ϸ�λ��ʱ����ʾ�벻�Ϸ�λ�õķ���
�϶�ʱ�������̸����ʾ��

��ս�����������

��Ϣ��ʾ����
�Ϸ�������ʱ��ʾ��������ʾ�������� ��ǰս��ʤ�����
�Ҳࣺ�����Ϣ���ǳƣ�Ѫ������һ�ֶ�ս���֣�

һ����ս���ڣ�
1.��׿Դ�� ����Ұ�֣�Ȼ�����ˮ��   ССӢ�۴�����װ��������ʾȻ���ƶ���װ������״̬���£�
2.�̳��� Ԥ�������� װ����

����ս�����ڣ�
��ս������

�������㻷�ڣ�
���ս���

 ��ȸ��ã�
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

    vector<cocos2d::Sprite*> Equipment;//��������װ��
    vector<cocos2d::Sprite*> Card;//���浱ǰ�ĸ�����
    vector<cocos2d::Sprite*> Chess;//���汸ս������

    EventDispatcher* eventDispatcher;
    //������
    vector<EventListenerTouchOneByOne*> touchlistenerCard;
    vector<EventListenerTouchOneByOne*> touchlistenerChess;
    vector<EventListenerTouchOneByOne*> touchlistenerEquipment;


    Vec2 EuipRetPos;
    Vec2 ChessRetPos;


    std::string  cardInfo;  //��Player�����ȡ  marketRefresh();
    std::string  equipmentInfo;  

    std::string boardInfo; //���浱ǰ���̵������Լ���Ӧ��λ�����꣨��Ҫ�������ӵȼ���Ϣ��װ����
   
    

    int coins=4;  //�����  ��ȡ�� getCoin����  ��ʼʱΪ4�����г��ۣ����ӣ������򣨼��٣�������Ұ��/��ս������ʤ ���ú���+4
    int level=1;  //�ȼ�   ��ȡ: Player��  int getLevel() { return level; }
    //int preStartPos = 0;


public:
     //void startCountdown(float duration);
     //void updateCountdown(float dt);

    virtual bool init();
    static Scene* scene();

    void onMouseDown(cocos2d::EventMouse* event);

    bool touchBegan(cocos2d::Touch* touch, cocos2d::Event* event);//������ʼ
    void touchMoved(cocos2d::Touch* touch, cocos2d::Event* event);//������
    void touchEnded(cocos2d::Touch* touch, cocos2d::Event* event);//��������

    bool c_touchBegan(cocos2d::Touch* touch, cocos2d::Event* event);//��Ƭ������ʼ

    void onRefBtnClicked(Ref* sender);

    void onUpBtnClicked(Ref* sender);

    Vec2 Tranverse(Vec2 v);

    // ��������ʱ��ʼ��ʱ
    virtual void onEnter() override {
        Layer::onEnter();
        // �ڽ��볡��ʱ��ʼ��ʱ
        unschedule(CC_SCHEDULE_SELECTOR(AIGameScene::updateTimer));  // ��ֹͣ��ʱ���������ظ�����
        schedule(CC_SCHEDULE_SELECTOR(AIGameScene::updateTimer), 1.0f, 30, 0.0f);
    }

    //// �����˳�ʱֹͣ��ʱ
    //virtual void onExit() override {
    //    Layer::onExit();
    //    // ���˳�����ʱֹͣ��ʱ
    //    unschedule(CC_SCHEDULE_SELECTOR(AIGameScene::updateTimer));
    //}

    // �����л���Ϻ��������ü�ʱ���ĳ�ʼֵ
    virtual void onEnterTransitionDidFinish() override {
        Layer::onEnterTransitionDidFinish();
        // �ڳ����л���Ϻ��������ü�ʱ���ĳ�ʼֵ
        remainingTime = 30;  // ���ó�ʼ����ʱʱ��
        countdownTimer->setPercentage(100);
    }

    // ���¼�ʱ��
    void updateTimer(float dt) {
        remainingTime -= 1.0f;
        // ���µ���ʱ�ı�
        countdownLabel->setString(StringUtils::format("Countdown: %.0f", remainingTime));
        // ���½�����
        float percentage = (remainingTime / total_time) * 100;
        // ֱ�Ӹ��½������İٷֱ�
        countdownTimer->setPercentage(percentage);
        if (remainingTime <= 0) {
            unschedule(CC_SCHEDULE_SELECTOR(AIGameScene::updateTimer));
            //�л����������ĳ���
            //Director::getInstance()->popScene();
            Director::getInstance()->pushScene(this->scene());
            Director::getInstance()->replaceScene(TransitionFade::create(1.0, Chessboard::createScene()));
        }
    }
    

    CREATE_FUNC(AIGameScene);// implement the "static create()" method manually
};

#endif 