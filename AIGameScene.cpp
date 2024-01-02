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

//ɾ��λ�õ����������⣻x
//û���Ƴ�ԭʼ�̳ǿ�Ƭ��x
//�ϳ������⣻
//���ۺ���ϳ�����û�б仯
//װ���϶�������������Խ�����x
//���ƴ���������x

bool AIGameScene::init() {
    remainingTime = 30;
    if (!Layer::init())
        return false;
    //Director::getInstance()->pushScene(HomeScene::scene());//�� HomeScene���ڵĳ�������ջ��
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();


    GlobalRes::getInstance()->setPlayers(4);

    //��ӱ���
    auto background = Sprite::create("gameBackground.png");
    background->setPosition(visibleSize / 2);
    // ��������ͼƬ�����ű�����ʹ���������
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

    //���������λ�û�ȡ
    // �����������
    //auto listener = EventListenerMouse::create();
    //listener->onMouseDown = CC_CALLBACK_1(AIGameScene::onMouseDown, this);
    //_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    //// ����Label
    //label = Label::createWithTTF("Mouse Position: (0, 0)", "fonts/arial.ttf", 24);
    //label->setAnchorPoint(Vec2(0.5f, 0.5f));
    //label->setPosition(Vec2(200, 200)); // ����Label�ĳ�ʼλ��
    //this->addChild(label);

    // ����CoinsLabel
    Coinslabel = Label::createWithTTF("4", "fonts/arial.ttf", 24);
    Coinslabel->setAnchorPoint(Vec2(0.5f, 0.5f));
    Coinslabel->setPosition(Vec2(350, 160));
    Coinslabel->setTag(-1);
    this->addChild(Coinslabel);

    // ����LevelLabel
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

    // �����˵�����Ӱ�ť
    auto menu = Menu::create(UpdateBtn, RefreshBtn, nullptr);
    menu->setPosition(Vec2(visibleSize.width - menu->getContentSize().width, 0));
    this->addChild(menu, 1);

    //����ʱģ��
    countdownLabel = Label::createWithTTF("Countdown: 0", "fonts/arial.ttf", 24);
    //countdownLabel->setAnchorPoint(Vec2(0, 0));
    countdownLabel->setPosition(Vec2(1165, 980));
    this->addChild(countdownLabel);
    countdownTimer = ProgressTimer::create(Sprite::create("loadingbar.png"));
    countdownTimer->setType(ProgressTimer::Type::BAR);
    countdownTimer->setMidpoint(Vec2(0, 0)); // ���ý����������
    countdownTimer->setBarChangeRate(Vec2(1, 0)); // ���ý������ı仯�ʣ���ʾˮƽ�����ϵı仯
    countdownTimer->setPercentage(100); // ��ʼΪ100%
    countdownTimer->setPosition(Vec2(780, 980));
    countdownTimer ->setTag(-1);
    this->addChild(countdownTimer);
    //total_time = 30;// �����޸ģ��趨ʱ��Ϊ30��
    //startCountdown(total_time); // ��ʼ����ʱ    

    //������̸�  ���½ǣ�400,340�� ���Ͻǣ�400,605�� ���½ǣ�1100,340��
    Boardbackground = Sprite::create("ChessBoard.png");
    Boardbackground->setPosition(Vec2(750, 470));
    Boardbackground->setContentSize(Size(710, 270));
    Boardbackground->setVisible(false);//��ʼ���ɼ�
    Boardbackground->setTag(-1);
    this->addChild(Boardbackground);

    //��ӵ���ѡ�������̸� �������ߴ�100*70  ÿ����������λ�á�445+i*100,365+j*70��
    Board_selected = Sprite::create("Board_selected.png");
    Board_selected->setContentSize(Size(100, 70));
    Board_selected->setVisible(false);
    Board_selected->setTag(-1);
    this->addChild(Board_selected);

    //װ�������ӵĹ��ü���������
    auto touchlistener = EventListenerTouchOneByOne::create();
    touchlistener->setSwallowTouches(true);

    touchlistener->onTouchBegan = CC_CALLBACK_2(AIGameScene::touchBegan, this);
    touchlistener->onTouchMoved = CC_CALLBACK_2(AIGameScene::touchMoved, this);
    touchlistener->onTouchEnded = CC_CALLBACK_2(AIGameScene::touchEnded, this);
    eventDispatcher = Director::getInstance()->getEventDispatcher();

    //װ����
    equipmentInfo = "123";//��ʼ��1,2,3��װ����һ��
    //equipmentInfo = Player::getInstance()->getEquipments(); ����Ұ�ֺ������������װ����Ϣ�����ظ����´���
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
        Equipment[i]->setTag(10 + equipmentInfo[i] - '1'); //��10��ʼ��15
        this->addChild(Equipment[i]);
        //Ϊװ������������
        touchlistenerEquipment.push_back(touchlistener->clone());
        eventDispatcher->addEventListenerWithSceneGraphPriority(touchlistenerEquipment.back(), Equipment[i]);
    }

    //�̳��� ����̳ǿ�Ƭ
    auto c_touchlistener = EventListenerTouchOneByOne::create();
    c_touchlistener->setSwallowTouches(true);
    c_touchlistener->onTouchBegan = CC_CALLBACK_2(AIGameScene::c_touchBegan, this);

    cardInfo = Player::getInstance()->marketRefresh();//��ʼ���̳ǿ��� 
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
        Card[i]->setTag(20 + cardInfo[i] - 'a'); //��20��ʼ��ʶ���Ƶ�29
        this->addChild(Card[i]);
        touchlistenerCard.push_back(c_touchlistener->clone());
        eventDispatcher->addEventListenerWithSceneGraphPriority(touchlistenerCard[i], Card[i]);
    }

    return true;
}



Vec2 AIGameScene::Tranverse(Vec2 worldPos)
{
    //EventMouse* event
    // ��ȡ������Ĵ�������
    //Vec2 mousePos = Vec2(event->getCursorX(), event->getCursorY());
    // ����������ת��ΪCocos2d-x����ϵ�µ�����
    //Vec2 worldPos = convertToNodeSpace(mousePos);

    //// ����Label���ı���ʾ�����λ�õ�����
    Vec2 TransPos;
    if (worldPos.x >= 395 && worldPos.x <=395 + 100 * 7) {
        if (worldPos.y >= 330 && worldPos.y <= 330 + 70 * 4) {//����
            TransPos.x = (int)((worldPos.x - 395) / 100) + 1;
            TransPos.y = (int)((worldPos.y - 330) / 70) + 1;
        }
        else if (worldPos.y <= 330 && worldPos.y >= 260) {//��ս��
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
        if (tag >= 10 && tag <= 15) {  //װ��
            EuipRetPos = target->getPosition();
        }
        else if (tag >= 0 && tag <= 9)
            ChessRetPos = target->getPosition();
        return true; // ����ǣ���ʼ��ק
    }
    return false;
}


void AIGameScene::touchMoved(Touch* touch, Event* event) {
    auto target = static_cast<Sprite*>(event->getCurrentTarget());
    Boardbackground->setVisible(true);
    clearLogo->setVisible(true);
    //��������̸�����ֵ �ҷ�4*7
    const float length = 100.0f;
    const float width = 70.0f;//�������ĳ���
    //��ӵ���ѡ�������̸� 
    const float beginX = 445.0f;
    const float beginY = 365.0f;//���½����̵�λ��
    const int row = 4;
    const int col = 7;//�к���
    Vec2 position = touch->getLocation();//��õ�ǰ������λ��

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

    //��������̸�����ֵ �ҷ�4*7
    const float length = 100.0f;
    const float width = 70.0f;//�������ĳ���
    //��ӵ���ѡ�������̸� �������ߴ�100*70  ÿ����������λ�á�445+i*100,365+j*70��
    const float beginX = 445.0f;
    const float beginY = 365.0f;//���½����̵�λ��
    const int row = 4;
    const int col = 7;//�к���

    auto target = static_cast<Sprite*>(event->getCurrentTarget());
    Vec2 position = touch->getLocation();//��õ�ǰ������λ��
    bool is_back = true;

    const float _length = 100.0f;
    const float _width = 100.0f;
    const float start_x = 50.0f;
    const float start_y = 50.0f;

    Rect clear(start_x, start_y, _length, _width);
    if (clear.containsPoint(position)&&target->getTag()>=0&&target->getTag()<=9) {
        Vec2 pos = Tranverse(ChessRetPos);
        //���ϰ�
        Player::getInstance()->sold(pos.y, pos.x);
        Coinslabel->setString(StringUtils::format("%d", Player::getInstance()->getCoins()));
        //���԰�
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
                    //��Ҫ������������ж����λ���������ӣ�����Player�е�eqiup�����жϼ���
                    //����index��װ��װ��(x,y)�������� bool equip(int index, int x, int y);  
                    //ÿ���������װ������������false˵������  ��0��ʼװ����
                }
                else if(Player::getInstance()->move(Tranverse(ChessRetPos).y,Tranverse(ChessRetPos).x,Tranverse(position).y,Tranverse(position).x))
                //�ж�����ط����������Լ��Ƿ�ﵽ����ϳ������Լ����Ըı䣺����move����  �����λ�������ӣ�����true������޷��ƶ�������false
                /*��(old_x,old_y)���������ƶ���(new_x,new_y)
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
   //ִ�з���ԭλ��
    if (is_back) {
        Vec2 retPos;
        int tag = target->getTag();
        if (tag >= 10 && tag <= 15)//װ��
            retPos = EuipRetPos;
        else//����
            retPos = ChessRetPos;
        target->setPosition(retPos);//�ص�ԭλ��
    }

    Boardbackground->setVisible(false);
    Board_selected->setVisible(false);
    clearLogo->setVisible(false);
}

bool AIGameScene::c_touchBegan(Touch* touch, Event* event) {
    //���԰�
    //auto target = static_cast<Sprite*>(event->getCurrentTarget());
    //int tag = target->getTag();//���Ƶ�tag��ʶ����
    ////��ӱ�ս������   
    //if (tag >= 20&&target->getBoundingBox().containsPoint(touch->getLocation())) { //tag���ڵ���20��˵���������Ϊ����
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

        //���ϰ�
           auto target = static_cast<Sprite*>(event->getCurrentTarget());
           int tag = target->getTag();//���Ƶ�tag��ʶ����
           int ChessPos;
           //if( Player::getInstance()->getCoins()>=ChessCoin)  //ÿ�����ӵ�Chesscoin��ȡ�ȴ�����

           if (tag >= 20 && target->getBoundingBox().containsPoint(touch->getLocation())) { //tag���ڵ���20��˵���������Ϊ����
               int x = (target->getPosition().x - 580) / 150;
               std::string info = Player::getInstance()->marketBuy(x);// ���̳��е�x�ſ�Ƭ,x��0��ʼ  ÿ����������λ�á� 580+150*i(i=0,1..)  �� 80��
              //���������ַ����� ʵ�ֺϳ�����  ÿ����һ�����Ӿ���Ҫ�ж�һ��
               /*����һ���ַ���
    "*"��ʾ��ǰ��սϯ�������޷�����
    "04"��ʾ��ǰ������ͬ�������Ӳ���3�ţ���Ҫɾ��"0"�����飬�����������ӷ��ڱ�սϯ���ĺ�λ
    "201231"��ʾ��Ҫɾ��"2"�����飬��Ҫɾ���ľ���λ����"(0,1)""(2,3)"������(0,1)��ʾ��սϯ�ĵ�һ��λ�ã���սϯ��λ�ô�һ�ſ�ʼ����BoardChessIfo[0][0]���ã�
            ��ɾ��������������¹���ľ�����ڱ�սϯ�ĵ�"1"��λ��
    "401022654"��ʾ��Ҫɾ��4�����飬����8λ����Ҫɾ�������λ�ã����һλ���¹�������õ�λ�ã���սϯ���ĸ�
    ���ڹ��������ֻ��ɾ��2��4�������������������ɾ��2�����¹������Ϊ2�ǣ�ɾ��4�����¹������Ϊ3��
    */
               if (info[0] == '*')
                   return false;
               if (info[0] == '0') {
                   ChessPos = info[1] - '0' - 1;
                   //return false;
               }
               else {
                   Vector<Node*> nodesToRemove;
                   int num = info[0] - '0'; //��Ҫɾ������ĸ���  
                   for (int i = 0, j = 1; i < num; i++) {
                       int x = info[j] - '0';
                       int y = info[j + 1] - '0' ;
                       const auto& allNodes = this->getChildren();
                       // �ҵ���Ҫ�Ƴ��Ľڵ㲢���浽��ʱ������
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
                   // �ڵ�����ɺ��Ƴ��ڵ�
                   for (const auto& node : nodesToRemove) {
                       this->removeChild(node);
                   }
                   ChessPos = info[info.size() - 1] - '0' - 1; //��ս��ÿ�����ӵ�����λ�� ��426+100*i(i=0,1,2..) , 290��
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
        Chess.back()->setTag(No);//���ӱ�ʶ��0��9��
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
              //touchlistenerChess.push_back(touchlistener->clone()); //��������������
              //eventDispatcher->addEventListenerWithSceneGraphPriority(touchlistenerChess.back(), Chess.back());
              //this->removeChild(target);
              //return false; 

    }
    return false;// ���� false ��ʾ�����¼�δ������
}


void AIGameScene::onMouseDown(EventMouse* event)
{
    //// ��ȡ������Ĵ�������
    //Vec2 mousePos = Vec2(event->getCursorX(), event->getCursorY());

    //// ����������ת��ΪCocos2d-x����ϵ�µ�����
    //Vec2 worldPos = convertToNodeSpace(mousePos);

    //// ����Label���ı���ʾ�����λ�õ�����
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
//    // ���µ���ʱ�ı�
//    countdownLabel->setString(StringUtils::format("Countdown: %.0f", remainingTime));
//
//    // ���½�����
//    float percentage = (remainingTime / total_time) * 100;
//    // ֱ�Ӹ��½������İٷֱ�
//    countdownTimer->setPercentage(percentage);
//
//    if (remainingTime <= 0){
//        unschedule(CC_SCHEDULE_SELECTOR(AIGameScene::updateCountdown));
//        //�л����������ĳ���
//        //Director::getInstance()->popScene();
//        Director::getInstance()->pushScene(this->scene());
//        Director::getInstance()->replaceScene(TransitionFade::create(1.0, Chessboard::createScene()));
//    }
//
//}

//ˢ���̳ǣ�����һ���ַ�����"abcc"��ʾ�̳�ˢ�³��Ŀ���abcc��������
void AIGameScene::onRefBtnClicked(Ref* sender) {

    vector<cocos2d::Sprite*> Card;//���浱ǰ�ĸ�����
    //������
    vector<EventListenerTouchOneByOne*> touchlistenerCard;
    auto c_touchlistener = EventListenerTouchOneByOne::create();
    c_touchlistener->setSwallowTouches(true);
    c_touchlistener->onTouchBegan = CC_CALLBACK_2(AIGameScene::c_touchBegan, this);


    const auto& allNodes = this->getChildren();

    Vector<Node*> nodesToRemove;

    // �ҵ���Ҫ�Ƴ��Ľڵ㲢���浽��ʱ������
    for (const auto& node : allNodes) {
        if (node->getTag() >= 20) {
            nodesToRemove.pushBack(node);
        }
    }

    // �ڵ�����ɺ��Ƴ��ڵ�
    for (const auto& node : nodesToRemove) {
        this->removeChild(node);
    }

    //���ϰ�
   cardInfo = Player::getInstance()->marketRefresh();  
    //���԰�
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
    //�ı�����
    //���԰�
    //coins--;
    //Coinslabel->setString(StringUtils::format("%d", coins));
    //���ϰ�
    Coinslabel->setString(StringUtils::format("%d", Player::getInstance()->getCoins()));

}

void AIGameScene::onUpBtnClicked(Ref* sender) {
    //���ϰ�
    Player::getInstance()->experienceUp();
    Levellabel->setString(StringUtils::format("%d", Player::getInstance()->getLevel()));
    Coinslabel->setString(StringUtils::format("%d", Player::getInstance()->getCoins()));
    
    //���԰�
    //level++;
    //coins--;
    //Levellabel->setString(StringUtils::format("%d", level));
    //Coinslabel->setString(StringUtils::format("%d", coins));

}