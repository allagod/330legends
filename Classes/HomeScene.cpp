#include "HomeScene.h"
#include "SettingsScene.h"
#include "RoomScene.h"
#include "AIGameScene.h"
#include"try.h"
#include "audio/include/AudioEngine.h"

USING_NS_CC;

bool HomeScene::isSoundOn = true;
int HomeScene::audioId = 0;
int HomeScene::backgroundMusicId = 0;
//std::string RoomScene::mode = "default";

//资源文件加载失败的debug信息输出函数
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HomeSceneScene.cpp\n");
}

Scene* HomeScene::scene() {
    // 'scene' is an autorelease object
    Scene* scene = Scene::create();
    // 'layer' is an autorelease object
    HomeScene* layer = HomeScene::create();
    // add layer as a child to scene
    scene->addChild(layer);
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HomeScene::init()
{
    if (!Layer::init())
        return false;

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 背景音乐与音效打开 默认为打开状态
    //"background_music.mp3" 是音乐文件的路径
    //audioId = AudioEngine::play2d("audio_music.mp3", true);
    

    //backgroundMusicId = AudioEngine::play2d("bgMusic.mp3", true);

    //添加背景
    auto background = Sprite::create("Starbg.png");
    background->setPosition(visibleSize / 2);
    // 调整背景图片的缩放比例，使其充满窗口
    background->setScaleX(visibleSize.width / background->getContentSize().width);
    background->setScaleY(visibleSize.height / background->getContentSize().height);
    this->addChild(background);

    //添加AI对战按钮
    auto aiBtn = MenuItemImage::create("AI.jpg", "AI_selected.jpg");
    aiBtn->setPosition(Vec2(200 , origin.y + visibleSize.height - 150));
    aiBtn->setCallback(CC_CALLBACK_1(HomeScene::onAIBtnClicked, this));

    //添加联机对战按钮 
    auto onlineBtn = MenuItemImage::create("lianji.jpg", "lianji_selected.jpg");
    onlineBtn->setPosition(Vec2(200 , origin.y + visibleSize.height - 350));
    onlineBtn->setCallback(CC_CALLBACK_1(HomeScene::onOnlineBtnClicked, this));

    // 添加设置按钮
    auto settingsButton = MenuItemImage::create("setting.jpg", "setting_selected.jpg");
    settingsButton->setPosition(Vec2(200, origin.y + visibleSize.height - 550));
    settingsButton->setCallback(CC_CALLBACK_1(HomeScene::onsettingsBtnClicked, this));

    // 创建菜单并添加按钮
    auto menu = Menu::create(aiBtn, onlineBtn, settingsButton, nullptr);
    menu->setPosition(Vec2(visibleSize.width - menu->getContentSize().width, 0));    // 设置菜单的位置为屏幕右侧
    this->addChild(menu, 1);

    //Director::getInstance()->pushScene(this->scene());//将 HomeScene所在的场景推入栈中
    return true;

}

//点击后直接进入游戏场景
void HomeScene::onAIBtnClicked(Ref* sender) {
    /*Director::getInstance()->replaceScene(TransitionFade::create(1.0, AIGameScene::scene()));*/
    Director::getInstance()->replaceScene(TransitionFade::create(1.0, New3DScene::createScene()));
    return;
}

// 点击按钮后显示设置界面
void HomeScene::onsettingsBtnClicked(Ref* sender) {
    //auto* settingsScene = Scene::create();
    //settingsScene->addChild(SettingsScene::create());
    Director::getInstance()->replaceScene(TransitionFade::create(1.0, SettingsScene::scene()));
    return;
}

// 弹出提示框调用键盘请求输入昵称（调用键盘）并保存
void HomeScene::onOnlineBtnClicked(Ref* sender) {
    Director::getInstance()->replaceScene(TransitionFade::create(1.0, RoomScene::scene()));
    return;
}


