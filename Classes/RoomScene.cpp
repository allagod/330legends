#include "RoomScene.h"
#include "AIGameScene.h"
#include "ui/CocosGUI.h"
#include "Network.h"

//����һ��Layer: ���layer�������ڵı�������ʾһ���µı����������д������䰴ť�뿪ʼ��Ϸ��ť
// ͬʱ��ʾ��ǰ���ߵ����з��䣨ע����Բ��ϸ��£����統���µķ��䱻����ʱ�������˵���ʾ�б�ͻ�仯��
// ����ϵͳ�������Ϣ(��ȡinformationHall),������ʾ�ɵ���ı�ǩ�����ĳһ��ǩ��(�ص������ڲ�����net.intoRoom(index),
// indexΪ�����ǩ��Ӧ�ķ�����ţ���ʾ���Ѽ��롱)����ʱ��������ط���Ч���������˳����䣩
// �д������䰴ť�����(�ص������ڲ�����net.buildRoom())����������ǳƴ���һ�����䲢��ʾ��
// ֻ�з��������ʼ��Ϸ��ť���Խ�����Ϸ�����������˸ü������Ч

Scene* RoomScene::scene()
{
    // 'scene' is an autorelease object
    Scene* scene = Scene::create();
    // 'layer' is an autorelease object
    RoomScene* layer = RoomScene::create();
    // add layer as a child to scene
    scene->addChild(layer);
    // return the scene
    return scene;
}

bool RoomScene::init() {

    Network::getInstance();
    // ��ӱ���
    auto winSize = Director::getInstance()->getVisibleSize();
    auto background = cocos2d::Sprite::create("empty.png");
    background->setScaleX(winSize.width / background->getContentSize().width);
    background->setScaleY(winSize.height / background->getContentSize().height);
    background->setPosition(cocos2d::Director::getInstance()->getVisibleSize() / 2);
    this->addChild(background);

    // ���������ǳ��ı���
     editBox = cocos2d::ui::EditBox::create(Size(200, 40), "");//���ַ���
    //�����ı������ڵ�λ��
    editBox->setPosition(Vec2(300, 950));
    //��������
    editBox->setFontName("Arial");
    //���������С
    editBox->setFontSize(24);
    //�������û�����ʾ��Ϣ
    editBox->setPlaceHolder("Enter your nickname");
    //�����û�������������Ϣ
    editBox->setMaxLength(50);
    //����Ϊ�����ı�����
    editBox->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
    //������������ΪĬ�ϼ�������
    editBox->setReturnType(ui::EditBox::KeyboardReturnType::DEFAULT);
    addChild(editBox);

    // ȷ���ǳư�ť  ����ȡ���߷�����Ϣ,����ʾ������Ϣ���Կɵ���ı�ǩ��ʽ��
    auto ConfirmBtn = cocos2d::ui::Button::create("ConfirmBtn.png", "ConfirmBtn_pressed.png");
    ConfirmBtn->setPosition(Vec2(600, 950));  
    ConfirmBtn->setContentSize(Size(20, 10));
    ConfirmBtn->addClickEventListener(CC_CALLBACK_1(RoomScene::ConfirmBtnCallback, this));
    this->addChild(ConfirmBtn);

    // �������䰴ť
    auto createRoomButton = cocos2d::ui::Button::create("button_create_room.png", "button_create_room_pressed.png");
    createRoomButton->setPosition(Vec2(300, 400));  
    createRoomButton->addClickEventListener(CC_CALLBACK_1(RoomScene::createRoomCallback, this));
    this->addChild(createRoomButton);

    // ��ʼ��Ϸ��ť
    auto startGameButton = cocos2d::ui::Button::create("button_start_game.png", "button_start_game_pressed.png");
    startGameButton->setPosition(Vec2(300, 600));  
    startGameButton->addClickEventListener(CC_CALLBACK_1(RoomScene::startGameCallback, this));
    this->addChild(startGameButton);

    // ˢ�·�����Ϣ��ť  ����ȡ���߷�����Ϣ,����ʾ������Ϣ���Կɵ���ı�ǩ��ʽ��
    auto RefButton = cocos2d::ui::Button::create("refBtn.png", "refBtn_pressed.png");
    RefButton->setPosition(Vec2(300, 800)); 
    RefButton->addClickEventListener(CC_CALLBACK_1(RoomScene::refreshCallback, this));
    this->addChild(RefButton);

    return true;
}

void RoomScene::refreshCallback(cocos2d::Ref* sender) {
    //�Ƴ�ԭ�������еİ�ť
    if (isCreateRoom == true) {
        Scene* currentScene = Director::getInstance()->getRunningScene();    // ��ȡ��ǰ����
        for (Node* node : currentScene->getChildren()) {
            // �жϽڵ��Ƿ��� Sprite ����
            if (Sprite* sprite = dynamic_cast<Sprite*>(node)) {
                // �ж�ָ��λ���Ƿ��ھ���ķ�Χ��
                if (sprite->getTag() >= 9) {
                    this->removeChild(sprite);
                }
            }
        }
    }
    //����ڴ��������ظ����������Ϣ
    //����"3# 1guolinghao##2yg##5chl#"��ʾ���������䣬��������ķ�����#...#���������Ϊ#�������

    //����ڷ����ڣ����ط����ڵ���Ϣ  ����Ŵ�0��ʼ����
    //"2#guolinghao##yg#"��һ���ַ���ʾ�������ж����ˣ�#..#�м��Ƿ���������ǳ�
    std::string information(Network::getInstance()->refresh());
    for (int i = 0, k = 0; i < information.size(); i++, k++) {
        if (i == 0) {
            num = information[i] - '0';
        }
        if (information[i] == '#'){
            int num = -1;
            if (information[i] >= '0' && information[i] <= '9') {
                num = information[i] - '0';
                i++;
            }
            std::string name;
            i++;
            for (int j = i + 1; j < information.size()&&information[j] != '#'; j++, i++)
                name += information[j];
            //�����ɵ���ı�ǩ�����ûص�����
            labelButton.push_back(cocos2d::ui::Button::create("Namebtn_normal.png", "Namebtn_clicked.png", "Namebtn_disabled.png"));
            labelButton.back()->setTitleText(name);  // ���ð�ť�ı�
            labelButton.back()->setTitleFontSize(30);       // ���ð�ť�ı������С
            labelButton.back()->setPosition(Vec2(900, 800 - k * 200));  // ���ð�ťλ��  ��Ҫ�޸�
            labelButton.back()->setTag(10+num);
            labelButton.back()->addClickEventListener(CC_CALLBACK_1(RoomScene::LableButtonClickCallback, this));  //���ð�ť����ص�
            this->addChild(labelButton.back());//����ǩ��ӵ��ó���
        }
    }
}

void  RoomScene::createRoomCallback(cocos2d::Ref* sender) {
    if (isCreateRoom == false||isjoinRoom==true) {
        Network::getInstance()->buildRoom();
        auto visibleSize = Director::getInstance()->getVisibleSize();
        auto Label = Label::createWithTTF("Creat successfully!", "fonts/arial.ttf", 24);
        Label->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 3 / 4));
        addChild(Label);
        isCreateRoom = true;
        isHomeowner = true;
    }
}

void  RoomScene::startGameCallback(cocos2d::Ref* sender) {
    //����ڷ����ڣ����ط����ڵ���Ϣ
    //"2#guolinghao##yg#"��һ���ַ���ʾ�������ж����ˣ�#..#�м��Ƿ���������ǳ�
    if (isCreateRoom == true) {
        std::string _informationRoom = Network::getInstance()->refresh();
        if (_informationRoom[0] - '0' <= 1)
            return;
    }
    // �ж��Ƿ�Ϊ������ֻ�з������Կ�ʼ��Ϸ;δ��������ʱ�����Ҳ��Ч
    if (isHomeowner == false || isCreateRoom == false)
        return;
    // ������Ϸ����
    Network::getInstance()->startGame();
    Director::getInstance()->replaceScene(AIGameScene::scene());
}

void  RoomScene::ConfirmBtnCallback(cocos2d::Ref* sender) {
    if (isconfirm == false) {
        Network::getInstance()->sendNikename(editBox->getText());
        editBox->setEnabled(false);
        isconfirm = true;
    }
}

void RoomScene::LableButtonClickCallback(Ref* sender) {
    // ������ת��Ϊ��ť����
    auto button = dynamic_cast<cocos2d::ui::Button*>(sender);
    //��ʾ�Ѽ��룬�������λ����Ч
    if (isjoinRoom == false&&isHomeowner==false&& Network::getInstance()->intoRoom(button->getTag()-10)) {
        isjoinRoom = true;
        auto visibleSize = Director::getInstance()->getVisibleSize();
        auto joinedLabel = Label::createWithTTF("Join successfully!Waiting for begin!", "fonts/arial.ttf", 24);
        joinedLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 3 / 4));
        addChild(joinedLabel);

       // Network::getInstance()->wait();
    }
}

/*	
//������һ�ֿ��ܶ�ս˭
	char* getNextEnemy();
 */