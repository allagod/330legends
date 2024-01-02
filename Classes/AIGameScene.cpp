#include "AIGameScene.h"
#include "HomeScene.h"
#include "Player.h"
#include "Chessboard.h"

Scene* AIGameScene::scene() {
    // 'scene' is an autorelease object
    Scene* scene = Scene::create();
    // 'layer' is an autorelease object
    AIGameScene* layer = AIGameScene::create();
    // add layer as a child to scene
    scene->addChild(layer);
    // return the scene
    return scene;
}

//删除位置的区域有问题；x
//没有移除原始商城卡片；x
//合成有问题；
//出售后可上场人数没有变化
//装备拖动到棋子上数组越界崩溃x
//卡牌处放置棋子x

bool AIGameScene::init() {
    remainingTime = 30;
    if (!Layer::init())
        return false;
    //Director::getInstance()->pushScene(HomeScene::scene());//将 HomeScene所在的场景推入栈中
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();


    GlobalRes::getInstance()->setPlayers(4);

    //添加背景
    auto background = Sprite::create("gameBackground.png");
    background->setPosition(visibleSize / 2);
    // 调整背景图片的缩放比例，使其充满窗口
    background->setScaleX(visibleSize.width / background->getContentSize().width);
    background->setScaleY(visibleSize.height / background->getContentSize().height);
    background->setTag(-1);
    this->addChild(background);

    clearLogo = Sprite::create("clear.png");
    clearLogo->setPosition(Vec2(100, 100));
    clearLogo->setContentSize(Size(100, 100));
    clearLogo->setVisible(false);
    clearLogo->setTag(-1);
    this->addChild(clearLogo);

    //鼠标点击坐标位置获取
    // 启用鼠标输入
    //auto listener = EventListenerMouse::create();
    //listener->onMouseDown = CC_CALLBACK_1(AIGameScene::onMouseDown, this);
    //_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    //// 创建Label
    //label = Label::createWithTTF("Mouse Position: (0, 0)", "fonts/arial.ttf", 24);
    //label->setAnchorPoint(Vec2(0.5f, 0.5f));
    //label->setPosition(Vec2(200, 200)); // 设置Label的初始位置
    //this->addChild(label);

    // 创建CoinsLabel
    Coinslabel = Label::createWithTTF("4", "fonts/arial.ttf", 24);
    Coinslabel->setAnchorPoint(Vec2(0.5f, 0.5f));
    Coinslabel->setPosition(Vec2(350, 160));
    Coinslabel->setTag(-1);
    this->addChild(Coinslabel);

    // 创建LevelLabel
    Levellabel = Label::createWithTTF("1", "fonts/arial.ttf", 24);
    Levellabel->setAnchorPoint(Vec2(0.5f, 0.5f));
    Levellabel->setPosition(Vec2(447, 160));
    Levellabel->setTag(-1);
    this->addChild(Levellabel);

    auto UpdateBtn = MenuItemImage::create("update_btn.png", "update_btn_selected.png");
    UpdateBtn->setAnchorPoint(Vec2(0, 0));
    UpdateBtn->setTag(-1);
    UpdateBtn->setPosition(Vec2(306, 93));
    UpdateBtn->setCallback(CC_CALLBACK_1(AIGameScene::onUpBtnClicked, this));

    auto RefreshBtn = MenuItemImage::create("refresh.png", "refresh_selected.png");
    RefreshBtn->setAnchorPoint(Vec2(0, 0));
    RefreshBtn->setTag(-1);
    RefreshBtn->setPosition(Vec2(306, 93 - 50));
    RefreshBtn->setCallback(CC_CALLBACK_1(AIGameScene::onRefBtnClicked, this));

    // 创建菜单并添加按钮
    auto menu = Menu::create(UpdateBtn, RefreshBtn, nullptr);
    menu->setPosition(Vec2(visibleSize.width - menu->getContentSize().width, 0));
    this->addChild(menu, 1);

    //倒计时模块
    countdownLabel = Label::createWithTTF("Countdown: 0", "fonts/arial.ttf", 24);
    //countdownLabel->setAnchorPoint(Vec2(0, 0));
    countdownLabel->setPosition(Vec2(1165, 980));
    this->addChild(countdownLabel);
    countdownTimer = ProgressTimer::create(Sprite::create("loadingbar.png"));
    countdownTimer->setType(ProgressTimer::Type::BAR);
    countdownTimer->setMidpoint(Vec2(0, 0)); // 设置进度条的起点
    countdownTimer->setBarChangeRate(Vec2(1, 0)); // 设置进度条的变化率，表示水平方向上的变化
    countdownTimer->setPercentage(100); // 初始为100%
    countdownTimer->setPosition(Vec2(780, 980));
    countdownTimer ->setTag(-1);
    this->addChild(countdownTimer);
    //total_time = 30;// 可以修改，设定时间为30秒
    //startCountdown(total_time); // 开始倒计时    

    //添加棋盘格  左下角（400,340） 左上角（400,605） 右下角（1100,340）
    Boardbackground = Sprite::create("ChessBoard.png");
    Boardbackground->setPosition(Vec2(750, 470));
    Boardbackground->setContentSize(Size(710, 270));
    Boardbackground->setVisible(false);//初始不可见
    Boardbackground->setTag(-1);
    this->addChild(Boardbackground);

    //添加单个选择后的棋盘格 单个棋格尺寸100*70  每个棋格的中心位置【445+i*100,365+j*70】
    Board_selected = Sprite::create("Board_selected.png");
    Board_selected->setContentSize(Size(100, 70));
    Board_selected->setVisible(false);
    Board_selected->setTag(-1);
    this->addChild(Board_selected);

    //装备与棋子的公用监听器设置
    auto touchlistener = EventListenerTouchOneByOne::create();
    touchlistener->setSwallowTouches(true);

    touchlistener->onTouchBegan = CC_CALLBACK_2(AIGameScene::touchBegan, this);
    touchlistener->onTouchMoved = CC_CALLBACK_2(AIGameScene::touchMoved, this);
    touchlistener->onTouchEnded = CC_CALLBACK_2(AIGameScene::touchEnded, this);
    eventDispatcher = Director::getInstance()->getEventDispatcher();

    //装备区
    equipmentInfo = "123";//初始化1,2,3号装备各一件
    //equipmentInfo = Player::getInstance()->getEquipments(); 打完野怪后调用它来更新装备信息并且重复以下代码
    for (int i = 0; i < 3; i++) {
        switch (equipmentInfo[i]) {
        case '1':
            Equipment.push_back(Sprite::create("sword.png"));
            break;
        case '2':
            Equipment.push_back(Sprite::create("red_crystal.png"));
            break;
        case '3':
            Equipment.push_back(Sprite::create("cloth.png"));
            break;
        case '4':
            Equipment.push_back(Sprite::create("amplified_classics.jpg"));
            break;
        case '5':
            Equipment.push_back(Sprite::create("big_sword.png"));
            break;
        case '6':
            Equipment.push_back(Sprite::create("cloth_armor.jpg"));
            break;
        }
        Equipment[i]->setPosition(Vec2(150, 495 - 90 * i));
        Equipment[i]->setContentSize(Size(50, 45));
        Equipment[i]->setTag(10 + equipmentInfo[i] - '1'); //从10开始到15
        this->addChild(Equipment[i]);
        //为装备创建监听器
        touchlistenerEquipment.push_back(touchlistener->clone());
        eventDispatcher->addEventListenerWithSceneGraphPriority(touchlistenerEquipment.back(), Equipment[i]);
    }

    //商城区 添加商城卡片
    auto c_touchlistener = EventListenerTouchOneByOne::create();
    c_touchlistener->setSwallowTouches(true);
    c_touchlistener->onTouchBegan = CC_CALLBACK_2(AIGameScene::c_touchBegan, this);

    cardInfo = Player::getInstance()->marketRefresh();//初始化商城卡牌 
    for (int i = 0; i < 4; i++) {
        switch (cardInfo[i]) {
        case 'a':
            Card.push_back(Sprite::create("a.png"));
            break;
        case 'b':
            Card.push_back(Sprite::create("b.png"));
            break;
            case 'c':
                Card.push_back(Sprite::create("c.png"));
                break;
            case 'd':
                Card.push_back(Sprite::create("d.png"));
                break;
            case 'e':
                Card.push_back(Sprite::create("e.png"));
                break;
            case 'f':
                Card.push_back(Sprite::create("f.png"));
                break;
            case 'g':
                Card.push_back(Sprite::create("g.png"));
                break;
            case 'h':
                Card.push_back(Sprite::create("h.png"));
                break;
            case 'i':
                Card.push_back(Sprite::create("i.png"));
                break;
            case 'j':
                Card.push_back(Sprite::create("j.png"));
                break;
        }
        Card[i]->setPosition(Vec2(580 + 150 * i, 80));
        Card[i]->setContentSize(Size(120, 140));
        Card[i]->setTag(20 + cardInfo[i] - 'a'); //从20开始标识卡牌到29
        this->addChild(Card[i]);
        touchlistenerCard.push_back(c_touchlistener->clone());
        eventDispatcher->addEventListenerWithSceneGraphPriority(touchlistenerCard[i], Card[i]);
    }

    return true;
}



Vec2 AIGameScene::Tranverse(Vec2 worldPos)
{
    //EventMouse* event
    // 获取鼠标点击的窗口坐标
    //Vec2 mousePos = Vec2(event->getCursorX(), event->getCursorY());
    // 将窗口坐标转换为Cocos2d-x坐标系下的坐标
    //Vec2 worldPos = convertToNodeSpace(mousePos);

    //// 更新Label的文本显示鼠标点击位置的坐标
    Vec2 TransPos;
    if (worldPos.x >= 395 && worldPos.x <=395 + 100 * 7) {
        if (worldPos.y >= 330 && worldPos.y <= 330 + 70 * 4) {//棋盘
            TransPos.x = (int)((worldPos.x - 395) / 100) + 1;
            TransPos.y = (int)((worldPos.y - 330) / 70) + 1;
        }
        else if (worldPos.y <= 330 && worldPos.y >= 260) {//备战区
            TransPos.x = (int)((worldPos.x - 390 ) / 100) + 1;
            TransPos.y = 0;
        }
        else {
            TransPos.x = 0;
            TransPos.y = 0;
        }
    }
    else {
        TransPos.x = 0;
        TransPos.y = 0;
    }
    //label->setString(StringUtils::format("Mouse Position: (%.2f, %.2f)", TransPos.x, TransPos.y));
    return TransPos;
}

bool AIGameScene::touchBegan(Touch* touch, Event* event) {
    auto target = static_cast<Sprite*>(event->getCurrentTarget());
    int tag = target->getTag();
    if ((tag <= 15 && tag >= 0) && target->getBoundingBox().containsPoint(touch->getLocation())) {
        if (tag >= 10 && tag <= 15) {  //装备
            EuipRetPos = target->getPosition();
        }
        else if (tag >= 0 && tag <= 9)
            ChessRetPos = target->getPosition();
        return true; // 如果是，则开始拖拽
    }
    return false;
}


void AIGameScene::touchMoved(Touch* touch, Event* event) {
    auto target = static_cast<Sprite*>(event->getCurrentTarget());
    Boardbackground->setVisible(true);
    clearLogo->setVisible(true);
    //定义的棋盘各个数值 我方4*7
    const float length = 100.0f;
    const float width = 70.0f;//单个棋格的长宽
    //添加单个选择后的棋盘格 
    const float beginX = 445.0f;
    const float beginY = 365.0f;//左下角棋盘的位置
    const int row = 4;
    const int col = 7;//行和列
    Vec2 position = touch->getLocation();//获得当前触摸的位置

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            Rect rect(beginX + j * length, beginY + i * width, length, width);
            if (rect.containsPoint(position)) {
                Board_selected->setVisible(true);
                Board_selected->setPosition(Vec2(beginX + j * length, beginY + i * width));
                break;
            }
        }
    }
    target->setPosition(touch->getLocation());
}

void AIGameScene::touchEnded(Touch* touch, Event* event) {

    //定义的棋盘各个数值 我方4*7
    const float length = 100.0f;
    const float width = 70.0f;//单个棋格的长宽
    //添加单个选择后的棋盘格 单个棋格尺寸100*70  每个棋格的中心位置【445+i*100,365+j*70】
    const float beginX = 445.0f;
    const float beginY = 365.0f;//左下角棋盘的位置
    const int row = 4;
    const int col = 7;//行和列

    auto target = static_cast<Sprite*>(event->getCurrentTarget());
    Vec2 position = touch->getLocation();//获得当前触摸的位置
    bool is_back = true;

    const float _length = 100.0f;
    const float _width = 100.0f;
    const float start_x = 50.0f;
    const float start_y = 50.0f;

    Rect clear(start_x, start_y, _length, _width);
    if (clear.containsPoint(position)&&target->getTag()>=0&&target->getTag()<=9) {
        Vec2 pos = Tranverse(ChessRetPos);
        //联合版
        Player::getInstance()->sold(pos.y, pos.x);
        Coinslabel->setString(StringUtils::format("%d", Player::getInstance()->getCoins()));
        //测试版
        //coins--;
        //Coinslabel->setString(StringUtils::format("%d", coins));

        this->removeChild(target);
        is_back = false;
    }


    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {

            Rect rect(beginX + j * length, beginY + i * width, length, width);

            if (rect.containsPoint(position)) {
                int tag = target->getTag();
                if (tag >= 10 && tag <= 15) {
                    int x = Tranverse(position).x;
                    int y = Tranverse(position).y;
                    if (Player::getInstance()->equip(0, y, x)) {
                        this->removeChild(target);
                        is_back = false;
                    }
                    //需要补充的条件：判断这个位置有无棋子：调用Player中的eqiup函数判断即可
                    //将第index个装备装到(x,y)处棋子上 bool equip(int index, int x, int y);  
                    //每个棋子最多装上三个，返回false说明已满  从0开始装备；
                }
                else if(Player::getInstance()->move(Tranverse(ChessRetPos).y,Tranverse(ChessRetPos).x,Tranverse(position).y,Tranverse(position).x))
                //判断这个地方有无棋子以及是否达到最大上场人数以及属性改变：调用move函数  如果新位置无棋子，返回true，如果无法移动，返回false
                /*将(old_x,old_y)处的棋子移动到(new_x,new_y)
                bool move(int old_x, int old_y, int new_x, int new_y);*/
                {
                    target->setPosition(Vec2(beginX + j * length, beginY + i * width));
                    is_back = false;
                }
                Board_selected->setPosition(Vec2(beginX + j * length, beginY + i * width));
                break;
            }

        }
    }
   //执行返回原位置
    if (is_back) {
        Vec2 retPos;
        int tag = target->getTag();
        if (tag >= 10 && tag <= 15)//装备
            retPos = EuipRetPos;
        else//棋子
            retPos = ChessRetPos;
        target->setPosition(retPos);//回到原位置
    }

    Boardbackground->setVisible(false);
    Board_selected->setVisible(false);
    clearLogo->setVisible(false);
}

bool AIGameScene::c_touchBegan(Touch* touch, Event* event) {
    //测试版
    //auto target = static_cast<Sprite*>(event->getCurrentTarget());
    //int tag = target->getTag();//卡牌的tag标识棋子
    ////添加备战区棋子   
    //if (tag >= 20&&target->getBoundingBox().containsPoint(touch->getLocation())) { //tag大于等于20，说明点击对象为卡牌
    //    //if(currentCoin>=ChessCoin)
    //    int No = tag - 20;
    //    switch (No) {
    //    case 0:
    //        Chess.push_back(Sprite::create("chess1.png"));
    //        break;
    //    case 1:
    //        Chess.push_back(Sprite::create("chess2.png"));
    //        break;
    //        /*case 2:
    //            Chess.push_back(Sprite::create("chess2.png"));
    //            break;
    //        case 3:
    //            Chess.push_back(Sprite::create("chess2.png"));
    //            break;
    //        case 4:
    //            Chess.push_back(Sprite::create("chess2.png"));
    //            break;
    //        case 5:
    //            Chess.push_back(Sprite::create("chess2.png"));
    //            break;
    //        case 6:
    //            Chess.push_back(Sprite::create("chess2.png"));
    //            break;
    //        case 7:
    //            Chess.push_back(Sprite::create("chess2.png"));
    //            break;
    //        case 8:
    //            Chess.push_back(Sprite::create("chess2.png"));
    //            break;
    //        case 9:
    //            Chess.push_back(Sprite::create("chess2.png"));
    //            break;*/
    //    }
    //    Chess.back()->setPosition(Vec2(426 + 100 * preStartPos, 290));
    //    preStartPos++;
    //    Chess.back()->setContentSize(Size(50, 70));
    //    Chess.back()->setTag(No);
    //    this->addChild(Chess.back());

    //    this->removeChild(target);

    //    auto listener = EventListenerTouchOneByOne::create();
    //    listener->onTouchBegan = CC_CALLBACK_2(AIGameScene::touchBegan, this);
    //    listener->onTouchMoved = CC_CALLBACK_2(AIGameScene::touchMoved, this);
    //    listener->onTouchEnded = CC_CALLBACK_2(AIGameScene::touchEnded, this);

    //    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener->clone(), Chess.back());

    //    return true;

        //联合版
           auto target = static_cast<Sprite*>(event->getCurrentTarget());
           int tag = target->getTag();//卡牌的tag标识棋子
           int ChessPos;
           //if( Player::getInstance()->getCoins()>=ChessCoin)  //每个棋子的Chesscoin获取等待完善

           if (tag >= 20 && target->getBoundingBox().containsPoint(touch->getLocation())) { //tag大于等于20，说明点击对象为卡牌
               int x = (target->getPosition().x - 580) / 150;
               std::string info = Player::getInstance()->marketBuy(x);// 买商城中第x张卡片,x从0开始  每个卡牌中心位置【 580+150*i(i=0,1..)  ， 80】
              //解析上面字符串： 实现合成升级  每加入一个棋子就需要判断一次
               /*返回一个字符串
    "*"表示当前备战席已满，无法购买
    "04"表示当前棋盘中同类型棋子不满3张，需要删除"0"个精灵，被购买后的棋子放在备战席的四号位
    "201231"表示需要删除"2"个精灵，需要删除的精灵位置是"(0,1)""(2,3)"，其中(0,1)表示备战席的第一个位置（备战席的位置从一号开始，即BoardChessIfo[0][0]弃用）
            在删除这两个精灵后，新购买的精灵放在备战席的第"1"个位置
    "401022654"表示需要删除4个精灵，后续8位是需要删除精灵的位置，最后一位是新购买精灵放置的位置，备战席第四个
    由于购买后生星只有删除2和4个精灵的情况，所以如果删除2个，新购买精灵变为2星，删除4个，新购买精灵变为3星
    */
               if (info[0] == '*')
                   return false;
               if (info[0] == '0') {
                   ChessPos = info[1] - '0' - 1;
                   //return false;
               }
               else {
                   Vector<Node*> nodesToRemove;
                   int num = info[0] - '0'; //需要删除精灵的个数  
                   for (int i = 0, j = 1; i < num; i++) {
                       int x = info[j] - '0';
                       int y = info[j + 1] - '0' ;
                       const auto& allNodes = this->getChildren();
                       // 找到需要移除的节点并保存到临时容器中
                       for (const auto& node : allNodes) {
                           if (node->getTag()>=0&& node->getTag()<=9) {
                               Vec2 pos = node->getPosition();
                               if ((x == 0 && node && node->getBoundingBox().containsPoint(Vec2(426 + (y-1) * 100, 290))) ||
                                   (node && node->getBoundingBox().containsPoint(Vec2(445 + (y - 1) * 100, 365 +  (x-1)* 70))))
                                            nodesToRemove.pushBack(node);
                           }
                       }
                       j += 2;
                   }
                   // 在迭代完成后移除节点
                   for (const auto& node : nodesToRemove) {
                       this->removeChild(node);
                   }
                   ChessPos = info[info.size() - 1] - '0' - 1; //备战区每个棋子的中心位置 【426+100*i(i=0,1,2..) , 290】
               }
              int No = tag - 20;
              switch (No) {
                  case 0:
                      Chess.push_back(Sprite::create("cbingnv.png"));
                      break;
                  case 1:
                      Chess.push_back(Sprite::create("cbobi.png"));
                      break;
                  case 2:
                      Chess.push_back(Sprite::create("cgewen.png"));
                      break;
                  case 3:
                      Chess.push_back(Sprite::create("ckaerma.png"));

                      break;
                  case 4:
                      Chess.push_back(Sprite::create("clakesi.png"));

                      break;
                  case 5:
                      Chess.push_back(Sprite::create("clanbo.png"));

                      break;
                  case 6:
                      Chess.push_back(Sprite::create("clienata.png"));

                      break;
                  case 7:
                      Chess.push_back(Sprite::create("cniutou.png"));

                      break;
                  case 8:
                      Chess.push_back(Sprite::create("ctienan.png"));

                      break;
                  case 9:
                      Chess.push_back(Sprite::create("cwuqi.png"));

                      break;
              }
        Chess.back()->setPosition(Vec2(426 + 100 * ChessPos, 290));
        //preStartPos++;
        Chess.back()->setContentSize(Size(80, 110));
        Chess.back()->setTag(No);//棋子标识从0到9号
        this->addChild(Chess.back());
        this->removeChild(target);
        auto listener = EventListenerTouchOneByOne::create();
        listener->onTouchBegan = CC_CALLBACK_2(AIGameScene::touchBegan, this);
        listener->onTouchMoved = CC_CALLBACK_2(AIGameScene::touchMoved, this);
        listener->onTouchEnded = CC_CALLBACK_2(AIGameScene::touchEnded, this);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener->clone(), Chess.back());
        return true;

              //Chess.back()->setPosition(Vec2(426 + 100 * ChessPos, 290));
              //Chess.back()->setContentSize(Size(50, 70));
              //Chess.back()->setTag(No);
              //this->addChild(Chess.back());
              //touchlistenerChess.push_back(touchlistener->clone()); //不会出问题吧这里
              //eventDispatcher->addEventListenerWithSceneGraphPriority(touchlistenerChess.back(), Chess.back());
              //this->removeChild(target);
              //return false; 

    }
    return false;// 返回 false 表示触摸事件未被消费
}


void AIGameScene::onMouseDown(EventMouse* event)
{
    //// 获取鼠标点击的窗口坐标
    //Vec2 mousePos = Vec2(event->getCursorX(), event->getCursorY());

    //// 将窗口坐标转换为Cocos2d-x坐标系下的坐标
    //Vec2 worldPos = convertToNodeSpace(mousePos);

    //// 更新Label的文本显示鼠标点击位置的坐标
    //label->setString(StringUtils::format("Mouse Position: (%.2f, %.2f)", worldPos.x, worldPos.y));
}

//void AIGameScene::startCountdown(float duration)
//{
//    remainingTime = duration;
//    schedule(CC_SCHEDULE_SELECTOR(AIGameScene::updateCountdown), 1.0f, duration, 0.0f);
//}
//
//void AIGameScene::updateCountdown(float dt)
//{
//    remainingTime -= 1.0f;
//
//    // 更新倒计时文本
//    countdownLabel->setString(StringUtils::format("Countdown: %.0f", remainingTime));
//
//    // 更新进度条
//    float percentage = (remainingTime / total_time) * 100;
//    // 直接更新进度条的百分比
//    countdownTimer->setPercentage(percentage);
//
//    if (remainingTime <= 0){
//        unschedule(CC_SCHEDULE_SELECTOR(AIGameScene::updateCountdown));
//        //切换到杨光那里的场景
//        //Director::getInstance()->popScene();
//        Director::getInstance()->pushScene(this->scene());
//        Director::getInstance()->replaceScene(TransitionFade::create(1.0, Chessboard::createScene()));
//    }
//
//}

//刷新商城，返回一个字符串，"abcc"表示商城刷新出的卡是abcc四种类型
void AIGameScene::onRefBtnClicked(Ref* sender) {

    vector<cocos2d::Sprite*> Card;//储存当前四个卡牌
    //监听器
    vector<EventListenerTouchOneByOne*> touchlistenerCard;
    auto c_touchlistener = EventListenerTouchOneByOne::create();
    c_touchlistener->setSwallowTouches(true);
    c_touchlistener->onTouchBegan = CC_CALLBACK_2(AIGameScene::c_touchBegan, this);


    const auto& allNodes = this->getChildren();

    Vector<Node*> nodesToRemove;

    // 找到需要移除的节点并保存到临时容器中
    for (const auto& node : allNodes) {
        if (node->getTag() >= 20) {
            nodesToRemove.pushBack(node);
        }
    }

    // 在迭代完成后移除节点
    for (const auto& node : nodesToRemove) {
        this->removeChild(node);
    }

    //联合版
   cardInfo = Player::getInstance()->marketRefresh();  
    //测试版
    //cardInfo ="aaab";

    for (int i = 0; i < 4; i++) {
        switch (cardInfo[i]) {
        case 'a':
            Card.push_back(Sprite::create("a.png"));
            break;
        case 'b':
            Card.push_back(Sprite::create("b.png"));
            break;
        case 'c':
                Card.push_back(Sprite::create("c.png"));
                break;
        case 'd':
                Card.push_back(Sprite::create("d.png"));
                break;
        case 'e':
                Card.push_back(Sprite::create("e.png"));
                break;
        case 'f':
                Card.push_back(Sprite::create("f.png"));
                break;
            case 'g':
                Card.push_back(Sprite::create("g.png"));
                break;
            case 'h':
                Card.push_back(Sprite::create("h.png"));
                break;
            case 'i':
                Card.push_back(Sprite::create("i.png"));
                break;
            case 'j':
                Card.push_back(Sprite::create("j.png"));
                break;
        }
        Card[i]->setPosition(Vec2(580 + 150 * i, 80));
        Card[i]->setContentSize(Size(120, 140));
        Card[i]->setTag(20 + cardInfo[i] - 'a');
        this->addChild(Card[i]);
        touchlistenerCard.push_back(c_touchlistener->clone());
        eventDispatcher->addEventListenerWithSceneGraphPriority(touchlistenerCard.back(), Card.back());
       
    }
    //改变金币数
    //测试版
    //coins--;
    //Coinslabel->setString(StringUtils::format("%d", coins));
    //联合版
    Coinslabel->setString(StringUtils::format("%d", Player::getInstance()->getCoins()));

}

void AIGameScene::onUpBtnClicked(Ref* sender) {
    //联合版
    Player::getInstance()->experienceUp();
    Levellabel->setString(StringUtils::format("%d", Player::getInstance()->getLevel()));
    Coinslabel->setString(StringUtils::format("%d", Player::getInstance()->getCoins()));
    
    //测试版
    //level++;
    //coins--;
    //Levellabel->setString(StringUtils::format("%d", level));
    //Coinslabel->setString(StringUtils::format("%d", coins));

}