#pragma once
#ifndef __INIT_SCENE_H__
#define __INIT_SCENE_H__
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "cocos2d.h"
using namespace cocos2d; //不要忘了这个！
/*
本源文件主要完成从进入游戏到切换到HomeScene主菜单场景的系列操作
*/

class InitScene : public Layer {
private:
    Size winSize;
public:
    bool init(); //完成主要场景的创建工作
    static Scene* scene(); //写法固定
    //在这里添加回调函数以及update函数的重写

    void onStartBtnClicked(Ref* sender);

    CREATE_FUNC(InitScene);  //必须放最后！封装好的从构造（创建）函数 create（）
    //除了以上三个必须实现外，可以补充增加回调函数、重写update函数 
    /*关于回调函数CC_CALLBACK_0、CC_CALLBACK_1 和 CC_CALLBACK_2 是在Cocos2d-x中用于创建回调函数的宏，
    用于绑定成员函数或静态函数。后面不同的数字代表该回调函数需要的参数个数*/
};

#endif
