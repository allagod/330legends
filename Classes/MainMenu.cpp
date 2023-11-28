#include "MainMenu.h"
#include "Resource.h"
#include "audio/include/AudioEngine.h"
#include "GameLayer.h"
#include "AboutLayer.h"
#include "SettingsLayer.h"

using namespace cocos2d;

Scene* MainMenu::scene()
{
    Scene* scene = Scene::create();
    MainMenu* layer = MainMenu::create();
    scene->addChild(layer);

    return scene;
}

// on "init" you need to initialize your instance
bool MainMenu::init()
{

    if (!Layer::init())
    {
        return false;
    }
    //预载资源
    


    //////////////////

    winSize = Director::getInstance()->getWinSize();
    //添加背景


    //添加一些贴图



    //添加按钮



    return true;
}
