#include"SettingsScene.h"
#include"cocos/audio/include/AudioEngine.h"


/*֪ʶ����  173ҳ
   ��Ϸ�е���Ƶ��Ϊ�������֣���ʱ��ѭ�����ţ�ռ�ýϴ��ڴ棬�޷�ͬʱ���Ŷ��������Ч���̵ģ�����ͬʱ���Ŷ����
   cocos���ṩ��Audio����
   ֧�������
    �������֣�MP3��ʽ������ѹ����ʽ���� WAV������ѹ����ʽ���ļ��ϴ󣬲��ʺ��ƶ��豸�� Windowsƽ̨
    ��Ч��WAV��ʽ  Windowsƽ̨
    Audio������Զ�����cocosʹ�ã�����ʹ�õ�API��SimpleAudioEngine //��ǰ�汾ò�Ʊ����AudioEngine
    Ԥ������������Ч����ѹ����ȣ����Է�ֹ���٣���Ϸ���й����н������һ�Σ����ڼ��س�����
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
    Director::getInstance()->pushScene(HomeScene::scene());//�� HomeScene���ڵĳ�������ջ��
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    isSoundOn = HomeScene::isSoundOn;
    audioId = HomeScene::audioId;//��Ч����ƵID 
    backgroundMusicId = HomeScene::backgroundMusicId; // �������ֵ���ƵID

    auto background = Sprite::create("empty.png");//settings_background.png
    // ��������ͼƬ�����ű�����ʹ���������
    background->setScaleX(visibleSize.width / background->getContentSize().width);
    background->setScaleY(visibleSize.height / background->getContentSize().height);
    background->setPosition(Director::getInstance()->getVisibleSize() / 2);
    this->addChild(background);

    //ǰ����ӱ�ǩ ��Ϸ��Ч���أ�
    MenuItemFont::setFontSize(40);
    MenuItemFont::setFontName("Arial");

    auto title1 = MenuItemFont::create("Sound");
    title1->setFontSize(26);
    title1->setFontName("Arial");
    title1->setEnabled(false);
    title1->setColor(Color3B(31, 45, 150));
    title1->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2,
        Director::getInstance()->getVisibleSize().height / 4*3-200));//λ����Ҫ��
    this->addChild(title1);

    //�����Ϸ��Ч�������õİ�ť
    /*֪ʶ���䣺�� MenuItemToggle �У�ֻ�е�ǰѡ�еİ�ť�ᱻ��ʾ���������İ�ť��������״̬��
    ������л���ťʱ��MenuItemToggle ���Զ��л���ǰ��ʾ�İ�ť��������������ť��*/
    auto musicToggle = MenuItemToggle::createWithCallback(
        CC_CALLBACK_1(SettingsScene::toggleMusic, this),
        MenuItemFont::create("On"), MenuItemFont::create("Off"),
        nullptr);
    musicToggle->setColor(Color3B(31, 45, 150));
    musicToggle->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2,
        Director::getInstance()->getVisibleSize().height / 2));

    //����������ڵ��϶���
    /*
    volumeSlider->loadBarTexture("slider_bar.png");: �����˻���ĵײ����������������ͨ����ʾ����������Χ��

    volumeSlider->loadSlidBallTextures("slider_thumb.png", "slider_thumb.png", "");: �����˻����ϻ����������
    ���������������������һ���͵ڶ��������Ǳ�ʾ����������������ֱ�������״̬�Ͱ���״̬�����������������ǽ���״̬�����������㴫����ǿ��ַ��� ""�������ڽ���״̬�»�������ܲ��������������仯��

    volumeSlider->loadProgressBarTexture("slider_progress.png");: �����˻����Ͻ�����������
    ��������ʾ��������ࣨ���ϲ࣬����ȡ���ڻ���ķ��򣩵Ľ�������
    */
    
    auto volumeSlider = ui::Slider::create();
    volumeSlider->loadBarTexture("slider_bar_empty.jpg");//��
    volumeSlider->loadSlidBallTextures("slider_thumb_normal.jpg", "slider_thumb_selected.jpg", "slider_thumb_forbidden.png");//�϶����״̬
    volumeSlider->loadProgressBarTexture("slider_progress.jpg");//��
    volumeSlider->setPercent(50); // ����Ĭ������Ϊ50%
    volumeSlider->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2,
        Director::getInstance()->getVisibleSize().height / 3));
    //�����϶�����ֵ�仯ʱ�Ļص�����
    volumeSlider->addEventListener(CC_CALLBACK_2(SettingsScene::onVolumeChanged, this));
    volumeSlider->setContentSize(Size(400, 32));


    this->addChild(volumeSlider);

    // ��ӹرհ�ť  
    auto backButton = MenuItemImage::create("close.png", "close_selected.png",
        [this](Ref* sender) {
            Director::getInstance()->popScene();//����ǰ�����ӳ���ջ�е��������ص���һ������
        });
    backButton->setPosition(Vec2(Director::getInstance()->getVisibleSize().width - 70,
        Director::getInstance()->getVisibleSize().height - 40)); //��ťͼ��������Ͻ�  ��Ҫ�޸�

    auto menu = Menu::create(musicToggle,backButton, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu);

    return true;
}

//�е�bug
void SettingsScene::toggleMusic(Ref* sender) {
    // ���ݵ�ǰ��Ч״̬�л�
    isSoundOn = !isSoundOn;
    if (isSoundOn)// ����Ч
        AudioEngine::resume(backgroundMusicId);
    else// �ر���Ч
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

// ���������϶�����ֵ�仯ʱ�Ļص�����
void SettingsScene::onVolumeChanged(Ref* sender, cocos2d::ui::Slider::EventType type) {
    if (type == cocos2d::ui::Slider::EventType::ON_PERCENTAGE_CHANGED) {
        // ��ȡ�϶�����ǰ�ٷֱ�
        int percent = dynamic_cast<cocos2d::ui::Slider*>(sender)->getPercent();
        // ���ٷֱ�ӳ�䵽������Χ��0 �� 1��
        float volume = percent / 100.0f;
        // ���ñ�����������
        cocos2d::AudioEngine::setVolume(backgroundMusicId, volume);
    }
}