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

//��Դ�ļ�����ʧ�ܵ�debug��Ϣ�������
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

    // ������������Ч�� Ĭ��Ϊ��״̬
    //"background_music.mp3" �������ļ���·��
    //audioId = AudioEngine::play2d("audio_music.mp3", true);
    

    //backgroundMusicId = AudioEngine::play2d("bgMusic.mp3", true);

    //��ӱ���
    auto background = Sprite::create("Starbg.png");
    background->setPosition(visibleSize / 2);
    // ��������ͼƬ�����ű�����ʹ���������
    background->setScaleX(visibleSize.width / background->getContentSize().width);
    background->setScaleY(visibleSize.height / background->getContentSize().height);
    this->addChild(background);

    //���AI��ս��ť
    auto aiBtn = MenuItemImage::create("AI.jpg", "AI_selected.jpg");
    aiBtn->setPosition(Vec2(200 , origin.y + visibleSize.height - 150));
    aiBtn->setCallback(CC_CALLBACK_1(HomeScene::onAIBtnClicked, this));

    //���������ս��ť 
    auto onlineBtn = MenuItemImage::create("lianji.jpg", "lianji_selected.jpg");
    onlineBtn->setPosition(Vec2(200 , origin.y + visibleSize.height - 350));
    onlineBtn->setCallback(CC_CALLBACK_1(HomeScene::onOnlineBtnClicked, this));

    // ������ð�ť
    auto settingsButton = MenuItemImage::create("setting.jpg", "setting_selected.jpg");
    settingsButton->setPosition(Vec2(200, origin.y + visibleSize.height - 550));
    settingsButton->setCallback(CC_CALLBACK_1(HomeScene::onsettingsBtnClicked, this));

    // �����˵�����Ӱ�ť
    auto menu = Menu::create(aiBtn, onlineBtn, settingsButton, nullptr);
    menu->setPosition(Vec2(visibleSize.width - menu->getContentSize().width, 0));    // ���ò˵���λ��Ϊ��Ļ�Ҳ�
    this->addChild(menu, 1);

    //Director::getInstance()->pushScene(this->scene());//�� HomeScene���ڵĳ�������ջ��
    return true;

}

//�����ֱ�ӽ�����Ϸ����
void HomeScene::onAIBtnClicked(Ref* sender) {
    /*Director::getInstance()->replaceScene(TransitionFade::create(1.0, AIGameScene::scene()));*/
    Director::getInstance()->replaceScene(TransitionFade::create(1.0, New3DScene::createScene()));
    return;
}

// �����ť����ʾ���ý���
void HomeScene::onsettingsBtnClicked(Ref* sender) {
    //auto* settingsScene = Scene::create();
    //settingsScene->addChild(SettingsScene::create());
    Director::getInstance()->replaceScene(TransitionFade::create(1.0, SettingsScene::scene()));
    return;
}

// ������ʾ����ü������������ǳƣ����ü��̣�������
void HomeScene::onOnlineBtnClicked(Ref* sender) {
    Director::getInstance()->replaceScene(TransitionFade::create(1.0, RoomScene::scene()));
    return;
}


