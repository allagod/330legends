#pragma once
#ifndef __HOME_SCENE_H__
#define __HOME_SCENE_H__
#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"
using namespace cocos2d;

/*
本源文件完成模式选择界面的功能实现 最终切换进入游戏场景
主要包含 AI对战模式  联机模式  设置  三个按钮
点击AI对战模式后直接进入游戏场景
点击联机模式后进入 请求用户输入昵称 创建房间 显示当前在线房间  点击加入可用房间 人满后点击开始进入游戏场景
点击设置后 settingsScene界面包含背景音乐与音效的开关按钮  音量调节按钮  返回按钮
*/

class HomeScene : public cocos2d::Layer {
public:
    static bool isSoundOn;//默认音效为打开状态
    static int audioId;//音效的音频ID 
    static int backgroundMusicId; // 背景音乐的音频ID
    virtual bool init();
    static Scene* scene();

    void onAIBtnClicked(Ref* sender);
    void onOnlineBtnClicked(Ref* sender);
    void onsettingsBtnClicked(Ref* sender);

    CREATE_FUNC(HomeScene);// implement the "static create()" method manually
};

#endif 