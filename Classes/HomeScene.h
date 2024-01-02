#pragma once
#ifndef __HOME_SCENE_H__
#define __HOME_SCENE_H__
#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"
using namespace cocos2d;

/*
��Դ�ļ����ģʽѡ�����Ĺ���ʵ�� �����л�������Ϸ����
��Ҫ���� AI��սģʽ  ����ģʽ  ����  ������ť
���AI��սģʽ��ֱ�ӽ�����Ϸ����
�������ģʽ����� �����û������ǳ� �������� ��ʾ��ǰ���߷���  ���������÷��� ����������ʼ������Ϸ����
������ú� settingsScene�������������������Ч�Ŀ��ذ�ť  �������ڰ�ť  ���ذ�ť
*/

class HomeScene : public cocos2d::Layer {
public:
    static bool isSoundOn;//Ĭ����ЧΪ��״̬
    static int audioId;//��Ч����ƵID 
    static int backgroundMusicId; // �������ֵ���ƵID
    virtual bool init();
    static Scene* scene();

    void onAIBtnClicked(Ref* sender);
    void onOnlineBtnClicked(Ref* sender);
    void onsettingsBtnClicked(Ref* sender);

    CREATE_FUNC(HomeScene);// implement the "static create()" method manually
};

#endif 