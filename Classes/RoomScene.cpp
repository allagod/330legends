#include "RoomScene.h"
#include "AIGameScene.h"
#include "ui/CocosGUI.h"
#include "Network.h"

//创建一个Layer: 这个layer会在现在的背景上显示一个新的背景：还有有创建房间按钮与开始游戏按钮
// 同时显示当前在线的所有房间（注意可以不断更新，比如当有新的房间被创建时，其他人的显示列表就会变化）
// 根据系统传入的信息(读取informationHall),创建显示可点击的标签，点击某一标签后(回调函数内部调用net.intoRoom(index),
// index为点击标签对应的房间序号，显示“已加入”)，此时点击其他地方无效（不可以退出房间）
// 有创建房间按钮点击后(回调函数内部调用net.buildRoom())，以自身的昵称创建一个房间并显示；
// 只有房主点击开始游戏按钮可以进入游戏场景，其他人该键点击无效

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
    // 添加背景
    auto winSize = Director::getInstance()->getVisibleSize();
    auto background = cocos2d::Sprite::create("empty.png");
    background->setScaleX(winSize.width / background->getContentSize().width);
    background->setScaleY(winSize.height / background->getContentSize().height);
    background->setPosition(cocos2d::Director::getInstance()->getVisibleSize() / 2);
    this->addChild(background);

    // 创建输入昵称文本框
     editBox = cocos2d::ui::EditBox::create(Size(200, 40), "");//空字符串
    //设置文本框所在的位置
    editBox->setPosition(Vec2(300, 950));
    //设置字体
    editBox->setFontName("Arial");
    //设置字体大小
    editBox->setFontSize(24);
    //给出与用户的提示信息
    editBox->setPlaceHolder("Enter your nickname");
    //设置用户可以输入的最长信息
    editBox->setMaxLength(50);
    //设置为单行文本输入
    editBox->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
    //键盘类型设置为默认键盘类型
    editBox->setReturnType(ui::EditBox::KeyboardReturnType::DEFAULT);
    addChild(editBox);

    // 确认昵称按钮  ：获取在线房间信息,并显示房间信息（以可点击的标签形式）
    auto ConfirmBtn = cocos2d::ui::Button::create("ConfirmBtn.png", "ConfirmBtn_pressed.png");
    ConfirmBtn->setPosition(Vec2(600, 950));  
    ConfirmBtn->setContentSize(Size(20, 10));
    ConfirmBtn->addClickEventListener(CC_CALLBACK_1(RoomScene::ConfirmBtnCallback, this));
    this->addChild(ConfirmBtn);

    // 创建房间按钮
    auto createRoomButton = cocos2d::ui::Button::create("button_create_room.png", "button_create_room_pressed.png");
    createRoomButton->setPosition(Vec2(300, 400));  
    createRoomButton->addClickEventListener(CC_CALLBACK_1(RoomScene::createRoomCallback, this));
    this->addChild(createRoomButton);

    // 开始游戏按钮
    auto startGameButton = cocos2d::ui::Button::create("button_start_game.png", "button_start_game_pressed.png");
    startGameButton->setPosition(Vec2(300, 600));  
    startGameButton->addClickEventListener(CC_CALLBACK_1(RoomScene::startGameCallback, this));
    this->addChild(startGameButton);

    // 刷新房间信息按钮  ：获取在线房间信息,并显示房间信息（以可点击的标签形式）
    auto RefButton = cocos2d::ui::Button::create("refBtn.png", "refBtn_pressed.png");
    RefButton->setPosition(Vec2(300, 800)); 
    RefButton->addClickEventListener(CC_CALLBACK_1(RoomScene::refreshCallback, this));
    this->addChild(RefButton);

    return true;
}

void RoomScene::refreshCallback(cocos2d::Ref* sender) {
    //移除原来场景中的按钮
    if (isCreateRoom == true) {
        Scene* currentScene = Director::getInstance()->getRunningScene();    // 获取当前场景
        for (Node* node : currentScene->getChildren()) {
            // 判断节点是否是 Sprite 类型
            if (Sprite* sprite = dynamic_cast<Sprite*>(node)) {
                // 判断指定位置是否在精灵的范围内
                if (sprite->getTag() >= 9) {
                    this->removeChild(sprite);
                }
            }
        }
    }
    //如果在大厅，返回各个房间的信息
    //例如"3# 1guolinghao##2yg##5chl#"表示共三个房间，各个房间的房主是#...#，房间序号为#后的数字

    //如果在房间内，返回房间内的信息  房间号从0开始编码
    //"2#guolinghao##yg#"第一个字符表示房间内有多少人，#..#中间是房间内玩家昵称
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
            //创建可点击的标签并设置回调函数
            labelButton.push_back(cocos2d::ui::Button::create("Namebtn_normal.png", "Namebtn_clicked.png", "Namebtn_disabled.png"));
            labelButton.back()->setTitleText(name);  // 设置按钮文本
            labelButton.back()->setTitleFontSize(30);       // 设置按钮文本字体大小
            labelButton.back()->setPosition(Vec2(900, 800 - k * 200));  // 设置按钮位置  需要修改
            labelButton.back()->setTag(10+num);
            labelButton.back()->addClickEventListener(CC_CALLBACK_1(RoomScene::LableButtonClickCallback, this));  //设置按钮点击回调
            this->addChild(labelButton.back());//将标签添加到该场景
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
    //如果在房间内，返回房间内的信息
    //"2#guolinghao##yg#"第一个字符表示房间内有多少人，#..#中间是房间内玩家昵称
    if (isCreateRoom == true) {
        std::string _informationRoom = Network::getInstance()->refresh();
        if (_informationRoom[0] - '0' <= 1)
            return;
    }
    // 判断是否为房主，只有房主可以开始游戏;未创建房间时，点击也无效
    if (isHomeowner == false || isCreateRoom == false)
        return;
    // 进入游戏场景
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
    // 将参数转换为按钮类型
    auto button = dynamic_cast<cocos2d::ui::Button*>(sender);
    //显示已加入，点击其他位置无效
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
//返回下一局可能对战谁
	char* getNextEnemy();
 */