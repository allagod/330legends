#pragma once
#ifndef __ROOM_SCENE_H__
#define __ROOM_SCENE_H__

#include "cocos2d.h"
//#include"cocos"
#include "ui/CocosGUI.h"
#include <vector>
using namespace cocos2d;

//!未测试  先测试文本框输入功能


/*
本源文件主要完成点击联机模式后，显示创建房间画面, 最后到切换到GameScene
*/

class RoomScene : public Layer{
public:
    bool init();
    bool isHomeowner = false;
    bool isCreateRoom = false;
    bool isconfirm = false; 
    bool isjoinRoom = false;
    int num;
    //const char* nickname;
    std::vector<cocos2d::ui::Button*> labelButton;
    ui::EditBox* editBox;

    static Scene* scene();

    void  refreshCallback(cocos2d::Ref* sender);
    void createRoomCallback(cocos2d::Ref* sender);
    void startGameCallback(cocos2d::Ref* sender);
    void LableButtonClickCallback(Ref* sender);
 
    void ConfirmBtnCallback(cocos2d::Ref* sender);

    CREATE_FUNC(RoomScene);


};




#endif