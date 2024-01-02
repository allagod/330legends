#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "InitScene.h"
#include "HomeScene.h"
//#include "HelloWorldScene.h"
#include "audio/include/AudioEngine.h"

#include "cocos/ui/UILoadingBar.h"
#include <cocostudio/SimpleAudioEngine.h>

using namespace cocos2d;

USING_NS_CC;

Scene* InitScene::scene()
{
    // 'scene' is an autorelease object
    Scene* scene = Scene::create();
    // 'layer' is an autorelease object
    InitScene* layer = InitScene::create();
    // add layer as a child to scene
    scene->addChild(layer);
    // return the scene
    return scene;
}

bool InitScene::init()
{
    winSize = Director::getInstance()->getVisibleSize();

    //��ӱ��� �����ڴ˻������Ǹ����������̬Ч��
    auto background = Sprite::create("begin.jpg");
    background->setPosition(winSize / 2);
    // ��������ͼƬ�����ű�����ʹ���������
    background->setScaleX(winSize.width / background->getContentSize().width);
    background->setScaleY(winSize.height / background->getContentSize().height);
    this->addChild(background);
    //���330LEGEND��ǩ��������ʾ
    auto label1 = Label::createWithTTF("330LEGEND", "fonts/arial.ttf",80);
    label1->setPosition(winSize / 2);
    this->addChild(label1);
    //��ӿ�ʼ��Ϸ��ť
    auto startButton1 = MenuItemImage::create("start_2.png","start_2_selected.png" );

    startButton1->setCallback(CC_CALLBACK_1(InitScene::onStartBtnClicked, this));
    startButton1->setPosition(Vec2(winSize.width / 2, winSize.height / 4));

    auto menu = Menu::create(startButton1, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu);
    return true;
}


void InitScene::onStartBtnClicked(Ref* sender) {
    auto winSize = Director::getInstance()->getVisibleSize();

    // �����µĳ���
    auto* scene = Scene::create();

    // ��ӱ���
    auto background = Sprite::create("start_backgroud.png");
    background->setPosition(winSize / 2);
    // ��������ͼƬ�����ű�����ʹ���������
    background->setScaleX(winSize.width / background->getContentSize().width);
    background->setScaleY(winSize.height / background->getContentSize().height);
    scene->addChild(background);

    // �������Ĵ���
    auto loadingBar = ui::LoadingBar::create("loadingbar.jpg"); 

    /*loadingBar->loadTexture("loadingbar.jpg");  */
    loadingBar->setPosition(Vec2(winSize.width / 2, winSize.height / 4));
    loadingBar->setPercent(0);
    loadingBar->setContentSize(Size(800, 200));
    loadingBar->setLocalZOrder(10);  // ���ò�Σ�ȷ�������� UI Ԫ���ϲ���ʾ
    loadingBar->setDirection(ui::LoadingBar::Direction::LEFT);
    scene->addChild(loadingBar);

    // ������Լ�����������Ԥ����������Դ
    loadingBar->runAction(Sequence::create(
        // 3���ڼ��ص�100%
        ProgressTo::create(3.0, 100),
        CallFunc::create([scene]() {
            // ����������100%�󣬴����ص�����:�������˵�����HomeScene
    // �����֡����
    // ��������
            SpriteFrameCache::getInstance()->addSpriteFramesWithFile("magic_player.plist");
            SpriteFrameCache::getInstance()->addSpriteFramesWithFile("magic_bingnv.plist");
            SpriteFrameCache::getInstance()->addSpriteFramesWithFile("bingnv_attack.plist");
            SpriteFrameCache::getInstance()->addSpriteFramesWithFile("bobi_attack.plist");
            SpriteFrameCache::getInstance()->addSpriteFramesWithFile("gewen_attack.plist");
            SpriteFrameCache::getInstance()->addSpriteFramesWithFile("kaerma_attack.plist");
            SpriteFrameCache::getInstance()->addSpriteFramesWithFile("lakesi_attack.plist");
            SpriteFrameCache::getInstance()->addSpriteFramesWithFile("lanbo_attack.plist");
            SpriteFrameCache::getInstance()->addSpriteFramesWithFile("lienata_attack.plist");
            SpriteFrameCache::getInstance()->addSpriteFramesWithFile("niutou_attack.plist");
            SpriteFrameCache::getInstance()->addSpriteFramesWithFile("tienan_attack.plist");
            SpriteFrameCache::getInstance()->addSpriteFramesWithFile("wuqi_attack.plist");
            SpriteFrameCache::getInstance()->addSpriteFramesWithFile("bobi_other.plist");
            SpriteFrameCache::getInstance()->addSpriteFramesWithFile("gewen_other.plist");
            SpriteFrameCache::getInstance()->addSpriteFramesWithFile("kaerma_other.plist");
            SpriteFrameCache::getInstance()->addSpriteFramesWithFile("lakesi_other.plist");
            SpriteFrameCache::getInstance()->addSpriteFramesWithFile("lanbo_other.plist");
            SpriteFrameCache::getInstance()->addSpriteFramesWithFile("lienata_other.plist");
            SpriteFrameCache::getInstance()->addSpriteFramesWithFile("niutou_other.plist");
            SpriteFrameCache::getInstance()->addSpriteFramesWithFile("tienan_other.plist");
            SpriteFrameCache::getInstance()->addSpriteFramesWithFile("wuqi_other.plist");
            Texture2D* texture1 = Director::getInstance()->getTextureCache()->addImage("hotball.png");
            Texture2D* texture2 = Director::getInstance()->getTextureCache()->addImage("bingnv_zidan.png");
            Texture2D* texture3 = Director::getInstance()->getTextureCache()->addImage("kaerma_zidan.png");
            Texture2D* texture4 = Director::getInstance()->getTextureCache()->addImage("lakesi_zidan.png");
            Texture2D* texture5 = Director::getInstance()->getTextureCache()->addImage("lienata_zidan.png");
            //AudioEngine::preload("magic_attack.m4a");
            //AudioEngine::preload("gongji.m4a");

            Director::getInstance()->replaceScene(TransitionFade::create(1.0, HomeScene::scene()));
            }),
        nullptr));

    // �л�����������ļ��س���
    Director::getInstance()->replaceScene(TransitionFade::create(1.0, scene));
}