#include"SettingsScene.h"
#include"cocos/audio/include/AudioEngine.h"


/*知识补充  173页
   游戏中的音频分为背景音乐（长时间循环播放，占用较大内存，无法同时播放多个）与音效（短的，可以同时播放多个）
   cocos中提供了Audio引擎
   支持情况：
    背景音乐：MP3格式（有损压缩格式）或 WAV（无损压缩格式，文件较大，不适合移动设备） Windows平台
    音效：WAV格式  Windows平台
    Audio引擎可以独立于cocos使用，具体使用的API是SimpleAudioEngine //当前版本貌似变成了AudioEngine
    预处理音乐与音效（解压处理等）可以防止卡顿，游戏运行过程中仅需进行一次，放在加载场景处
 */

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
    /*知识补充：在 MenuItemToggle 中，只有当前选中的按钮会被显示，而其他的按钮则处于隐藏状态。
    当点击切换按钮时，MenuItemToggle 会自动切换当前显示的按钮，而隐藏其他按钮。*/
    auto musicToggle = MenuItemToggle::createWithCallback(
        CC_CALLBACK_1(SettingsScene::toggleMusic, this),
        MenuItemFont::create("On"), MenuItemFont::create("Off"),
        nullptr);
    musicToggle->setColor(Color3B(31, 45, 150));
    musicToggle->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2,
        Director::getInstance()->getVisibleSize().height / 2));

    //添加音量调节的拖动条
    /*
    volumeSlider->loadBarTexture("slider_bar.png");: 设置了滑块的底部条的纹理。这个纹理通常表示整个滑动范围。

    volumeSlider->loadSlidBallTextures("slider_thumb.png", "slider_thumb.png", "");: 设置了滑块上滑块球的纹理。
    这个方法有三个参数，第一个和第二个参数是表示滑块球的两个纹理，分别是正常状态和按下状态的纹理。第三个参数是禁用状态的纹理，由于你传入的是空字符串 ""，所以在禁用状态下滑块球可能不会有特殊的纹理变化。

    volumeSlider->loadProgressBarTexture("slider_progress.png");: 设置了滑块上进度条的纹理。
    这个纹理表示滑块球左侧（或上侧，具体取决于滑块的方向）的进度条。
    */
    
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

/*void SettingsLayer::onSoundControl(Ref* pSender)
{
    bool bSound = GlobalResManager::getInstance()->getSoundFlag();
    bSound = !bSound;
    GlobalResManager::getInstance()->setSoundFlag(bSound);
        if (bSound) {
            AudioEngine::resumeAll();
        }
        else {
            AudioEngine::pauseAll();
        }
}*/

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