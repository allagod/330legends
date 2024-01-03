#pragma once
#ifndef _SETTINGS_SCENE_H_
#define _SETTINGS_SCENE_H_
#include "cocos2d.h"
#include "HomeScene.h"
#include "ui/UISlider.h" // ���� UISlider ��ͷ�ļ�
#include "ui/CocosGUI.h"
#include "ui/UIWidget.h"
using namespace cocos2d;
using namespace cocos2d::ui;

/*
��Դ�ļ�������ý���Ĺ���ʵ�֣���Ҫ�����������ֵĿ����������ĵ���
*/
class SettingsScene : public cocos2d::Layer {
public:

    bool init();
    bool isSoundOn;
    int audioId;//��Ч����ƵID 
    int backgroundMusicId; // �������ֵ���ƵID

    static Scene* scene();
    CREATE_FUNC(SettingsScene);

private:
    void toggleMusic(Ref* sender);// ���������ֿ��صĻص�����
    void onVolumeChanged(Ref* sender, Slider::EventType type);// �϶�����ֵ�仯ʱ�Ļص�����
};

#endif