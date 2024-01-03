#include"EndScene.h"


Scene* EndScene::scene()
{
    // 'scene' is an autorelease object
    Scene* scene = Scene::create();
    // 'layer' is an autorelease object
    EndScene* layer = EndScene::create();
    // add layer as a child to scene
    scene->addChild(layer);
    // return the scene
    return scene;
}

bool EndScene::init()
{
        auto visibleSize = Director::getInstance()->getVisibleSize();
        //auto background1 = Sprite::create("empty.png");
        //this->addChild(background1);
        //background1->setPosition(visibleSize / 2);
        //auto background = Sprite::create("win.png");
        //background->setPosition(visibleSize / 2);
        //this->addChild(background);

        // 创建一个退出按钮
        auto exitButton = cocos2d::ui::Button::create("CloseNormal.png", "CloseSelected.png");
        exitButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 4));
        // 使用 Lambda 表达式作为按钮点击事件的回调函数
        exitButton->addClickEventListener([=](cocos2d::Ref* sender) {
            Director::getInstance()->end();
            });
        // 将按钮添加到场景
        this->addChild(exitButton);

    return true;
}
