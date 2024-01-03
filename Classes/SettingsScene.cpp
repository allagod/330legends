#include"SettingsScene.h"
#include"cocos/audio/include/AudioEngine.h"

Scene* SettingsScene::scene()
{
    // 'scene' is an autorelease object
    Scene* scene = Scene::create();
    // 'layer' is an autorelease object
    SettingsScene* layer = SettingsScene::create();
    // add layer as a child to scene
    scene->addChild(layer);
    // return the scene
    return scene;
}

bool SettingsScene::init() {
    if (!Layer::init()) {
        return false;
    }
    Director::getInstance()->pushScene(HomeScene::scene());//将 HomeScene所在的场景推入栈中
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    isSoundOn = HomeScene::isSoundOn;
    audioId = HomeScene::audioId;//音效的音频ID 
    backgroundMusicId = HomeScene::backgroundMusicId; // 背景音乐的音频ID

    auto background = Sprite::create("empty.png");//settings_background.png
    // 调整背景图片的缩放比例，使其充满窗口
    background->setScaleX(visibleSize.width / background->getContentSize().width);
    background->setScaleY(visibleSize.height / background->getContentSize().height);
    background->setPosition(Director::getInstance()->getVisibleSize() / 2);
    this->addChild(background);

    //前方添加标签 游戏音效开关：
    MenuItemFont::setFontSize(40);
    MenuItemFont::setFontName("Arial");

    auto title1 = MenuItemFont::create("Sound");
    title1->setFontSize(26);
    title1->setFontName("Arial");
    title1->setEnabled(false);
    title1->setColor(Color3B(31, 45, 150));
    title1->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2,
        Director::getInstance()->getVisibleSize().height / 4*3-200));//位置需要改
    this->addChild(title1);

    //添加游戏音效开关设置的按钮
    auto musicToggle = MenuItemToggle::createWithCallback(
        CC_CALLBACK_1(SettingsScene::toggleMusic, this),
        MenuItemFont::create("On"), MenuItemFont::create("Off"),
        nullptr);
    musicToggle->setColor(Color3B(31, 45, 150));
    musicToggle->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2,
        Director::getInstance()->getVisibleSize().height / 2));

    //添加音量调节的拖动条
    auto volumeSlider = ui::Slider::create();
    volumeSlider->loadBarTexture("slider_bar_empty.jpg");//空
    volumeSlider->loadSlidBallTextures("slider_thumb_normal.jpg", "slider_thumb_selected.jpg", "slider_thumb_forbidden.png");//拖动球的状态
    volumeSlider->loadProgressBarTexture("slider_progress.jpg");//满
    volumeSlider->setPercent(50); // 设置默认音量为50%
    volumeSlider->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2,
        Director::getInstance()->getVisibleSize().height / 3));
    //设置拖动条数值变化时的回调函数
    volumeSlider->addEventListener(CC_CALLBACK_2(SettingsScene::onVolumeChanged, this));
    volumeSlider->setContentSize(Size(400, 32));


    this->addChild(volumeSlider);

    // 添加关闭按钮  
    auto backButton = MenuItemImage::create("close.png", "close_selected.png",
        [this](Ref* sender) {
            Director::getInstance()->popScene();//将当前场景从场景栈中弹出，返回到上一个场景
        });
    backButton->setPosition(Vec2(Director::getInstance()->getVisibleSize().width - 70,
        Director::getInstance()->getVisibleSize().height - 40)); //按钮图标放在右上角  需要修改

    auto menu = Menu::create(musicToggle,backButton, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu);

    return true;
}

//有点bug
void SettingsScene::toggleMusic(Ref* sender) {
    // 根据当前音效状态切换
    isSoundOn = !isSoundOn;
    if (isSoundOn)// 打开音效
        AudioEngine::resume(backgroundMusicId);
    else// 关闭音效
        AudioEngine::stopAll();
}

// 音量调节拖动条数值变化时的回调函数
void SettingsScene::onVolumeChanged(Ref* sender, cocos2d::ui::Slider::EventType type) {
    if (type == cocos2d::ui::Slider::EventType::ON_PERCENTAGE_CHANGED) {
        // 获取拖动条当前百分比
        int percent = dynamic_cast<cocos2d::ui::Slider*>(sender)->getPercent();
        // 将百分比映射到音量范围（0 到 1）
        float volume = percent / 100.0f;
        // 设置背景音乐音量
        cocos2d::AudioEngine::setVolume(backgroundMusicId, volume);
    }
}