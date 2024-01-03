#pragma once
#ifndef _SETTINGS_SCENE_H_
#define _SETTINGS_SCENE_H_
#include "cocos2d.h"
#include "HomeScene.h"
#include "ui/UISlider.h" // 包含 UISlider 的头文件
#include "ui/CocosGUI.h"
#include "ui/UIWidget.h"
using namespace cocos2d;
using namespace cocos2d::ui;

/*
本源文件完成设置界面的功能实现：主要包含背景音乐的开关与音量的调节
*/
class SettingsScene : public cocos2d::Layer {
public:

    bool init();
    bool isSoundOn;
    int audioId;//音效的音频ID 
    int backgroundMusicId; // 背景音乐的音频ID

    static Scene* scene();
    CREATE_FUNC(SettingsScene);

private:
    void toggleMusic(Ref* sender);// 处理背景音乐开关的回调函数
    void onVolumeChanged(Ref* sender, Slider::EventType type);// 拖动条数值变化时的回调函数
};

#endif