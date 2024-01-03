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
��Դ�ļ������Ϸ��ս�����Ĺ���ʵ�� :

Ԥ���������������ת��������

��ս�׶Σ�
���Ϸ� ��ʾ������������ʣ��ʱ�䣬���Լ������ֵı仯��ʾ���Աߣ�  ���õ���ʱ  ����ʱ�������л�ս������

1�������Ƭ���ڱ�ս����ʾ��ӦӢ�ۣ�Ȼ��Ƭ��ʧ
2�������϶���ս����Ӣ�۵���ָ����Ԫ�񣨵���ƶ���ͻ������̸���ʾ���ɿ������ʧ��
�ƶ���ָ��λ��ʱ���б�ɫ��Ԫ����ʾ��λ�÷Ƿ���Ҫ���أ�  �����Ҫ�����������̵�Ӣ�۲���������Ϣ��

������ť  ������������Ҫ��ʱ���ص�������Ч��
ˢ�£��ص�������ɣ���ȡ����ˢ�º�Ŀ�����Ϣ�� �Ƴ���ǰ�̳��е�����ʣ�࿨Ƭ������̬��ʾ�¼���Ŀ�Ƭ�����Ҳ൮�������Ƶ���Ӧ��λ�ã�
������ ���ص�������ɣ����¾��飬��Ҽ��٣���Ҫ��ʾ���ֱ仯������ȡ�ȼ����������仯��Ҫ��ʾ��

�̳ǣ�
ˢ���̳ǰ�ť  ������ť������������ӣ��������Զ���������ʾ��ǰ�ȼ���
�̳���ʾ����ʼ��ȡ������Ϣ�����ֱ����ʾ���ſ��ƣ����ĳλ������������Ƭ��ʧ��������ע���жϵ�ǰ����Ƿ��㹻���򣩣�
Ȼ��սϯ����һ������Ҽ��٣����ˢ��ʱ���ſ�Ƭ������ʾ������ˢ��ʱ�Ķ�������
�����ʾ������仯��

װ������
Ԥ����������
����ڱ�ս�������ӡ��Լ�װ���ķ��ã�ʵ��������϶����Ϸ�λ��ʱ����ʾ�벻�Ϸ�λ�õķ���
�϶�ʱ�������̸����ʾ��

ս���׶Σ�
 װ����Ӣ���϶���Ч����������Ч
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
    //std::string boardInfo; //���浱ǰ���̵������Լ���Ӧ��λ�����꣨��Ҫ�������ӵȼ���Ϣ��װ����
   
public:

    virtual bool init();
    static Scene* scene();

    bool touchBegan(cocos2d::Touch* touch, cocos2d::Event* event);//������ʼ
    void touchMoved(cocos2d::Touch* touch, cocos2d::Event* event);//������
    void touchEnded(cocos2d::Touch* touch, cocos2d::Event* event);//��������
    bool c_touchBegan(cocos2d::Touch* touch, cocos2d::Event* event);//��Ƭ������ʼ

    void onRefBtnClicked(Ref* sender);
    void onUpBtnClicked(Ref* sender);

    Vec2 Tranverse(Vec2 v);

    // ��������ʱ��ʼ��ʱ
    virtual void onEnter();

    // �����л���Ϻ��������ü�ʱ���ĳ�ʼֵ
    virtual void onEnterTransitionDidFinish();

    // ���¼�ʱ��
    void updateTimer(float dt);
    CREATE_FUNC(AIGameScene);// implement the "static create()" method manually
};

#endif 