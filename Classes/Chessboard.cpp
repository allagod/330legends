#include "Chessboard.h"
#include "AIGameScene.h"
#include "Player.h"
#include "EndScene.h"


USING_NS_CC;

Scene* Chessboard::createScene()
{
    Scene* scene = Scene::create();

    Chessboard* layer = Chessboard::create();

    scene->addChild(layer);

    return scene;
}


static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in ChessboardScene.cpp\n");
}

// 初始化
bool Chessboard::init()
{
    //////////////////////////////
    // 1.画面的初定义
    if (!Layer::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2.画面部分：棋盘、倒计时
    
    // 增加棋盘精灵
    auto chessboard = Sprite::create("gameBackground.png");
    if (chessboard == nullptr)
    {
        problemLoading("gameBackground.png");
    }
    else
    {
        chessboard->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
        chessboard->setScale(0.64f);
        this->addChild(chessboard, 0);
    }

    //倒计时模块
    countdownLabel = Label::createWithTTF("Countdown: 0", "fonts/arial.ttf", 24);
    countdownLabel->setPosition(Vec2(1165, 980));
    this->addChild(countdownLabel);
    countdownTimer = ProgressTimer::create(Sprite::create("loadingbar.png"));
    countdownTimer->setType(ProgressTimer::Type::BAR);
    countdownTimer->setMidpoint(Vec2(0, 0)); // 设置进度条的起点
    countdownTimer->setBarChangeRate(Vec2(1, 0)); // 设置进度条的变化率，表示水平方向上的变化
    countdownTimer->setPercentage(100); // 初始为100%
    // 将整体长度减小一半
    float scaleFactor = 0.5f;
    countdownTimer->setScaleY(scaleFactor);
    countdownTimer->setPosition(Vec2(780, 980));
    this->addChild(countdownTimer);
    remainingTime = 30;// 可以修改，设定时间为30秒
    startCountdown(remainingTime); // 开始倒计时 

    /*
    3.精灵创建+初始化函数
    */
    ChessHero_create();
  
     /////////////////////////////
     // 4.update
     //棋盘的更新函数

     this->schedule([this](float dt) {
         this->update(dt);
         }, "update_key");
 
     //5.周期计时器
     this->schedule(CC_SCHEDULE_SELECTOR(Chessboard::generateValue), 1.0f, CC_REPEAT_FOREVER, 0);//暴击血量显示

     

    return true;
}


void Chessboard::startCountdown(float duration)
{
    remainingTime = duration;
    schedule(CC_SCHEDULE_SELECTOR(Chessboard::updateCountdown), 1.0f, duration, 0.0f);
}


void Chessboard::updateCountdown(float dt)
{
    remainingTime -= 1.0f;

    // 更新倒计时文本
    countdownLabel->setString(StringUtils::format("Countdown: %.0f", remainingTime));

    // 更新进度条
    float percentage = (remainingTime / 30) * 100;
    // 直接更新进度条的百分比
    countdownTimer->setPercentage(percentage);

    if (remainingTime <= 0&& end_status==1 )
    {
        unschedule(CC_SCHEDULE_SELECTOR(Chessboard::updateCountdown));
        Director::getInstance()->popScene();
    }

}

void Chessboard::ChessHero_create()
{
    std::string tmp(150, 0), tmp2(150, 0);//tmp为我方棋盘
    tmp = Player::getInstance()->getBoardCode();
    tmp2 = Player::getInstance()->getEnemyCode();

    // %开头，用五位字符表示一个位置的棋子
    // 第一位是棋子的类型，第二位是棋子的星级，第三到五位表示棋子所携带的装备
    // "%a212000000c1000"表示(1,1)位置棋子类型是a，二星，携带编号为1和2的两个装备，
    // (1,2)位置无棋子，(1,3)位置棋子类型是c，一星，无装备

        //前5个棋子的信息是在备战区的信息
    int chess_type = 0, chess_grade = 0, m_x = 0, m_y = 0;
    int weapon1 = 0, weapon2 = 0, weapon3 = 0;

    for (int i = 1; i <= 136; i += 5) {
        chess_type = tmp[i], chess_grade = tmp[i + 1] - '1', weapon1 = tmp[i + 2] - '0', weapon2 = tmp[i + 3] - '0', weapon3 = tmp[i + 4] - '0';
        m_x = (1 + ((i - 1) / 5) / 7);
        m_y = (1 + ((i - 1) / 5) % 7);

        int tmp = m_x;
        m_x = 400 + m_y * 100;
        m_y = 365 + tmp * 70;

        switch (chess_type) {
        case 'a':
        {
            player.push_back(ChessHero::create("bingnv_attack.png", 1, chess_grade));//#图片、序数
            player_num++;
            player[player_num - 1]->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            player[player_num - 1]->setPosition(Vec2(m_x, m_y));
            player[player_num - 1]->position_status = 1;

            //缩放
            player[player_num - 1]->setScale(player[player_num - 1]->m_SpriteTypeValue);
            player[player_num - 1]->setTag(1);                                        //#序数
            this->addChild(player[player_num - 1]);
            std::string health = "maxHealth: 200\n";
            std::string name = "Name:bingnv \n";
            std::string defense = "Attack: 1\n";
            generateInformation(health, name, defense);
            break;
        }
        case 'b':
        {
            player.push_back(ChessHero::create("bobi_stand.png", 2, chess_grade));//#图片、序数
            player_num++;
            player[player_num - 1]->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            player[player_num - 1]->setPosition(Vec2(m_x, m_y));
            player[player_num - 1]->position_status = 1;

            //缩放
            player[player_num - 1]->setScale(player[player_num - 1]->m_SpriteTypeValue);
            player[player_num - 1]->setTag(2);
            this->addChild(player[player_num - 1]);
            std::string health = "maxHealth: 250\n";
            std::string name = "Name:bobi \n";
            std::string defense = "Attack: 1.5\n";
            generateInformation(health, name, defense);
            break;
        }
        case 'c':
        {
            player.push_back(ChessHero::create("gewen_stand.png", 3, chess_grade));//#图片、序数
            player_num++;
            player[player_num - 1]->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            player[player_num - 1]->setPosition(Vec2(m_x, m_y));
            player[player_num - 1]->position_status = 1;

            //缩放
            player[player_num - 1]->setScale(player[player_num - 1]->m_SpriteTypeValue);
            player[player_num - 1]->setTag(3);                                        //#序数
            this->addChild(player[player_num - 1]);
            std::string health = "maxHealth: 200\n";
            std::string name = "Name:gewen \n";
            std::string defense = "Attack: 1.5\n";
            generateInformation(health, name, defense);
            break;
        }
        case 'd':
        {
            player.push_back(ChessHero::create("kaerma_stand.png", 4, chess_grade));//#图片、序数
            player_num++;
            player[player_num - 1]->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            player[player_num - 1]->setPosition(Vec2(m_x, m_y));
            player[player_num - 1]->position_status = 1;

            //缩放
            player[player_num - 1]->setScale(player[player_num - 1]->m_SpriteTypeValue);
            player[player_num - 1]->setTag(4);                                        //#序数
            this->addChild(player[player_num - 1]);
            std::string health = "maxHealth: 200\n";
            std::string name = "Name:kaerma \n";
            std::string defense = "Attack: 1.5\n";
            generateInformation(health, name, defense);
            break;
        }
        case 'e':
        {
            player.push_back(ChessHero::create("lakesi_stand.png", 5, chess_grade));//#图片、序数
            player_num++;
            player[player_num - 1]->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            player[player_num - 1]->setPosition(Vec2(m_x, m_y));
            player[player_num - 1]->position_status = 1;

            //缩放
            player[player_num - 1]->setScale(player[player_num - 1]->m_SpriteTypeValue);
            player[player_num - 1]->setTag(5);                                        //#序数
            this->addChild(player[player_num - 1]);
            std::string health = "maxHealth: 200\n";
            std::string name = "Name:lakesi \n";
            std::string defense = "Attack: 1.5\n";
            generateInformation(health, name, defense);
            break;
        }
        case 'f':
        {
            player.push_back(ChessHero::create("lanbo_stand.png", 6, chess_grade));//#图片、序数
            player_num++;
            player[player_num - 1]->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            player[player_num - 1]->setPosition(Vec2(m_x, m_y));
            player[player_num - 1]->position_status = 1;

            //缩放
            player[player_num - 1]->setScale(player[player_num - 1]->m_SpriteTypeValue);
            player[player_num - 1]->setTag(6);                                        //#序数
            this->addChild(player[player_num - 1]);
            std::string health = "maxHealth: 200\n";
            std::string name = "Name:lanbo \n";
            std::string defense = "Attack: 1.5\n";
            generateInformation(health, name, defense);
            break;
        }
        case 'g':
        {
            player.push_back(ChessHero::create("lienata_stand.png", 7, chess_grade));//#图片、序数
            player_num++;
            player[player_num - 1]->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            player[player_num - 1]->setPosition(Vec2(m_x, m_y));
            player[player_num - 1]->position_status = 1;

            //缩放
            player[player_num - 1]->setScale(player[player_num - 1]->m_SpriteTypeValue);
            player[player_num - 1]->setTag(7);                                        //#序数
            this->addChild(player[player_num - 1]);
            std::string health = "maxHealth: 200\n";
            std::string name = "Name:lienata \n";
            std::string defense = "Attack: 1.5\n";
            generateInformation(health, name, defense);
            break;
        }
        case'h':
        {
            player.push_back(ChessHero::create("niutou_stand.png", 8, chess_grade));//#图片、序数
            player_num++;
            player[player_num - 1]->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            player[player_num - 1]->setPosition(Vec2(m_x, m_y));
            player[player_num - 1]->position_status = 1;

            //缩放
            player[player_num - 1]->setScale(player[player_num - 1]->m_SpriteTypeValue);
            player[player_num - 1]->setTag(8);                                        //#序数
            this->addChild(player[player_num - 1]);
            std::string health = "maxHealth: 250\n";
            std::string name = "Name:niutou \n";
            std::string defense = "Attack: 1.5\n";
            generateInformation(health, name, defense);
            break;
        }
        case 'i':
        {
            player.push_back(ChessHero::create("tienan_stand.png", 9, chess_grade));//#图片、序数
            player_num++;
            player[player_num - 1]->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            player[player_num - 1]->setPosition(Vec2(m_x, m_y));
            player[player_num - 1]->position_status = 1;

            //缩放
            player[player_num - 1]->setScale(player[player_num - 1]->m_SpriteTypeValue);
            player[player_num - 1]->setTag(9);                                     //#序数
            this->addChild(player[player_num - 1]);
            std::string health = "maxHealth: 200\n";
            std::string name = "Name:tienan \n";
            std::string defense = "Attack: 1.5\n";
            generateInformation(health, name, defense);
            break;
        }
        case 'j':
        {
            player.push_back(ChessHero::create("wuqi_stand.png", 10, chess_grade));//#图片、序数
            player_num++;
            player[player_num - 1]->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            player[player_num - 1]->setPosition(Vec2(m_x, m_y));
            player[player_num - 1]->position_status = 1;

            //缩放
            player[player_num - 1]->setScale(player[player_num - 1]->m_SpriteTypeValue);
            player[player_num - 1]->setTag(10);                                    //#序数
            this->addChild(player[player_num - 1]);
            std::string health = "maxHealth: 200\n";
            std::string name = "Name:wuqi \n";
            std::string defense = "Attack: 1.5\n";
            generateInformation(health, name, defense);
            break;
        }

        }
        if (chess_type >= 'a' && chess_type <= 'j') {
            player[player_num - 1]->weapon[0] = weapon1 - '0';
            player[player_num - 1]->weapon[1] = weapon2 - '0';
            player[player_num - 1]->weapon[2] = weapon3 - '0';
            player[player_num - 1]->wear_weapon();

            Size playerSize = player[player_num - 1]->getContentSize();
            player[player_num - 1]->healthBar->init(getPositionX() - playerSize.width / 3, getPositionY() + playerSize.height / 0.9, player[player_num - 1]->position_status);
            player[player_num - 1]->healthBar->setScale(0.7f);
        }
    }

   
    //…………………………………………………………………………………等待传递第二个数组………………………………………………………………………………………
    for (int i = 1; i <= 136; i += 5) {
        chess_type = tmp2[i], chess_grade = tmp2[i + 1] - '1', weapon1 = tmp2[i + 2] - '0', weapon2 = tmp2[i + 3] - '0', weapon3 = tmp2[i + 4] - '0';
        m_x = (8 - ((i - 1) / 5) / 7);
        m_y = (7 - ((i - 1) / 5) % 7);
            
        int tmp = m_x;
        m_x = 400 + m_y * 100;
        m_y = 365 + tmp * 70;

        switch (chess_type) {
        case 'a':
        {
            enemy.push_back(ChessHero::create("bingnv_attack.png", 1, chess_grade));//#图片、序数
            enemy_num++;
            enemy[enemy_num - 1]->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            enemy[enemy_num - 1]->setPosition(Vec2(m_x, m_y));
            enemy[enemy_num - 1]->position_status = 0;

            //缩放
            enemy[enemy_num - 1]->setScale(enemy[enemy_num - 1]->m_SpriteTypeValue);
            enemy[enemy_num - 1]->setTag(1);                                        //#序数
            this->addChild(enemy[enemy_num - 1]);
          
            break;
        }
        case 'b':
        {
            enemy.push_back(ChessHero::create("bobi_stand_down.png", 2, chess_grade));//#图片、序数
            enemy_num++;
            enemy[enemy_num - 1]->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            enemy[enemy_num - 1]->setPosition(Vec2(m_x, m_y));
            enemy[enemy_num - 1]->position_status = 0;

            //缩放
            enemy[enemy_num - 1]->setScale(enemy[enemy_num - 1]->m_SpriteTypeValue);
            enemy[enemy_num - 1]->setTag(2);
            this->addChild(enemy[enemy_num - 1]);
          
            break;
        }
        case 'c':
        {
            enemy.push_back(ChessHero::create("gewen_stand_down.png", 3, chess_grade));//#图片、序数
            enemy_num++;
            enemy[enemy_num - 1]->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            enemy[enemy_num - 1]->setPosition(Vec2(m_x, m_y));
            enemy[enemy_num - 1]->position_status = 0;

            //缩放
            enemy[enemy_num - 1]->setScale(enemy[enemy_num - 1]->m_SpriteTypeValue);
            enemy[enemy_num - 1]->setTag(3);                                        //#序数
            this->addChild(enemy[enemy_num - 1]);
           
            break;
        }
        case 'd':
        {
            enemy.push_back(ChessHero::create("kaerma_stand_down.png", 4, chess_grade));//#图片、序数
            enemy_num++;
            enemy[enemy_num - 1]->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            enemy[enemy_num - 1]->setPosition(Vec2(m_x, m_y));
            enemy[enemy_num - 1]->position_status = 0;

            //缩放
            enemy[enemy_num - 1]->setScale(enemy[enemy_num - 1]->m_SpriteTypeValue);
            enemy[enemy_num - 1]->setTag(4);                                        //#序数
            this->addChild(enemy[enemy_num - 1]);
           
            break;
        }
        case 'e':
        {
            enemy.push_back(ChessHero::create("lakesi_stand_down.png", 5, chess_grade));//#图片、序数
            enemy_num++;
            enemy[enemy_num - 1]->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            enemy[enemy_num - 1]->setPosition(Vec2(m_x, m_y));
            enemy[enemy_num - 1]->position_status = 0;

            //缩放
            enemy[enemy_num - 1]->setScale(enemy[enemy_num - 1]->m_SpriteTypeValue);
            enemy[enemy_num - 1]->setTag(5);                                        //#序数
            this->addChild(enemy[enemy_num - 1]);
           
            break;
        }
        case 'f':
        {
            enemy.push_back(ChessHero::create("lanbo_stand_down.png", 6, chess_grade));//#图片、序数
            enemy_num++;
            enemy[enemy_num - 1]->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            enemy[enemy_num - 1]->setPosition(Vec2(m_x, m_y));
            enemy[enemy_num - 1]->position_status = 0;

            //缩放
            enemy[enemy_num - 1]->setScale(enemy[enemy_num - 1]->m_SpriteTypeValue);
            enemy[enemy_num - 1]->setTag(6);                                        //#序数
            this->addChild(enemy[enemy_num - 1]);
          
            break;
        }
        case 'g':
        {
            enemy.push_back(ChessHero::create("lienata_stand_down.png", 7, chess_grade));//#图片、序数
            enemy_num++;
            enemy[enemy_num - 1]->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            enemy[enemy_num - 1]->setPosition(Vec2(m_x, m_y));
            enemy[enemy_num - 1]->position_status = 0;

            //缩放
            enemy[enemy_num - 1]->setScale(enemy[enemy_num - 1]->m_SpriteTypeValue);
            enemy[enemy_num - 1]->setTag(7);                                        //#序数
            this->addChild(enemy[enemy_num - 1]);
          
            break;
        }
        case'h':
        {
            enemy.push_back(ChessHero::create("niutou_stand_down.png", 8, chess_grade));//#图片、序数
            enemy_num++;
            enemy[enemy_num - 1]->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            enemy[enemy_num - 1]->setPosition(Vec2(m_x, m_y));
            enemy[enemy_num - 1]->position_status = 0;

            //缩放
            enemy[enemy_num - 1]->setScale(enemy[enemy_num - 1]->m_SpriteTypeValue);
            enemy[enemy_num - 1]->setTag(8);                                        //#序数
            this->addChild(enemy[enemy_num - 1]);
           
            break;
        }
        case 'i':
        {
            enemy.push_back(ChessHero::create("tienan_stand_down.png", 9, chess_grade));//#图片、序数
            enemy_num++;
            enemy[enemy_num - 1]->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            enemy[enemy_num - 1]->setPosition(Vec2(m_x, m_y));
            enemy[enemy_num - 1]->position_status = 0;

            //缩放
            enemy[enemy_num - 1]->setScale(enemy[enemy_num - 1]->m_SpriteTypeValue);
            enemy[enemy_num - 1]->setTag(9);                                     //#序数
            this->addChild(enemy[enemy_num - 1]);
          
            break;
        }
        case 'j':
        {
            enemy.push_back(ChessHero::create("wuqi_stand_down.png", 10, chess_grade));//#图片、序数
            enemy_num++;
            enemy[enemy_num - 1]->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            enemy[enemy_num - 1]->setPosition(Vec2(m_x, m_y));
            enemy[enemy_num - 1]->position_status = 0;

            //缩放
            enemy[enemy_num - 1]->setScale(enemy[enemy_num - 1]->m_SpriteTypeValue);
            enemy[enemy_num - 1]->setTag(10);                                    //#序数
            this->addChild(enemy[enemy_num - 1]);
           
            break;
        }
        }

        if (chess_type >= 'a' && chess_type <= 'j') {
            enemy[enemy_num - 1]->weapon[0] = weapon1 - '0';
            enemy[enemy_num - 1]->weapon[1] = weapon2 - '0';
            enemy[enemy_num - 1]->weapon[2] = weapon3 - '0';
            enemy[enemy_num - 1]->wear_weapon();

            Size enemySize = enemy[enemy_num - 1]->getContentSize();
            enemy[enemy_num - 1]->healthBar->init(getPositionX() , getPositionY() + enemySize.height / 0.8, enemy[enemy_num - 1]->position_status);
            enemy[enemy_num - 1]->healthBar->setScale(0.7f);
        }
    }
}


void  Chessboard::generateValue(float dt) {

    // 使用当前时间作为随机数种子
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    for (int i = 1; i <= player_num; i++) {
        if (player[i - 1]->enemy != nullptr && player[i - 1]->m_HP > 0 && player[i - 1]->enemy->m_HP > 0)
        {
            // 产生100-300范围内的随机数
            int health = std::rand() % 201 + 100; // 0到200之间的随机数再加上100

            std::string healthString = std::to_string(health); // 将整数转换为字符串

            auto label = Label::createWithTTF(healthString, "fonts/arial.ttf", 24); // 创建一个艺术字 Label

            Size playerSize = player[i-1]->getContentSize();

            label->setPosition(Vec2(player[i - 1]->enemy->getPositionX()+playerSize.width/2, player[i - 1]->enemy->getPositionY()+ playerSize.height/2)); // 设置文本的位置为 (x, y)
            this->addChild(label); // 添加到场景中

            label->setTextColor(Color4B(255, 0, 0, 255)); // 设置文本颜色为红色

            label->setScale(3.0f); // 设置初始大小为2倍

            // 创建一个 ScaleTo 动作，从当前大小（2倍）缩放到最终大小（0.5倍），并在2秒内完成
            auto scaleAction = ScaleTo::create(0.5f, 0.5f);

            // 让 label 执行这个动作
            label->runAction(scaleAction);

            auto fadeOutAction = FadeOut::create(1.0f);

            // 执行 FadeOut 动作
            label->runAction(fadeOutAction);
        }
    }
}

// Modify the update function to include continuous animation
void Chessboard::update(float dt)
{
  
    //处理用户输入(是不需要担忧的，因为像：买英雄、卖英雄、买装备、装备合成、选择铭文、这样的输入在备战区层实现)，传入这个类的时候使用回调函数就可以
    //1.唯一要实现的是：将装备给正在战斗的英雄
    //……………………update如何处理输入呢？……………………
    MessagePutIn();

    /////////////////////////////
    // 2.时刻更新匹配的对手
    BattleMatch();
    
    int live_player = 0, live_enemy = 0;

   //3. 遍历每一个血量大于0的棋子的状态（攻击状态、释放技能状态（识别后就关闭这个状态）），用于给他的敌人扣血，然后配上死亡检测
    for (int i = 1; i <= player_num; i++) {
        if (player[i - 1]->animation_end == 0)
            live_player++;
        if (player[i - 1]->enemy != nullptr && player[i - 1]->m_HP > 0 && player[i - 1]->enemy->m_HP > 0) {
            if (player[i - 1]->attack_status == 1) {
                player[i - 1]->enemy->m_HP -= player[i - 1]->m_SpritePowerValue * player[i - 1]->m_SpriteAttackSpeed / player[i - 1]->enemy->m_defence;//精灵攻击速度;
                player[i - 1]->enemy->healthBar->decreaseHealth(player[i - 1]->m_SpritePowerValue * player[i - 1]->m_SpriteAttackSpeed / player[i - 1]->enemy->m_defence, player[i - 1]->enemy->position_status);
            }
            if (player[i - 1]->skill_status == 1) {
                player[i - 1]->enemy->m_HP -= player[i - 1]->skill_injury;//技能;
                player[i - 1]->skill_status = 0;
            }
        }
    }

    

    for (int i = 1; i <= enemy_num; i++) {
        if (enemy[i - 1]->animation_end == 0)
            live_enemy++;
        if (enemy[i - 1]->enemy != nullptr && enemy[i - 1]->m_HP > 0 && enemy[i - 1]->enemy->m_HP > 0) {        
            if (enemy[i - 1]->attack_status == 1) {
                enemy[i - 1]->enemy->m_HP -= enemy[i - 1]->m_SpritePowerValue * enemy[i - 1]->m_SpriteAttackSpeed/ enemy[i - 1]->enemy->m_defence;//精灵攻击速度;
                enemy[i - 1]->enemy->healthBar->decreaseHealth(enemy[i - 1]->m_SpritePowerValue * enemy[i - 1]->m_SpriteAttackSpeed / enemy[i - 1]->enemy->m_defence, enemy[i - 1]->enemy->position_status);
            }
            if (enemy[i - 1]->skill_status == 1) {
                enemy[i - 1]->enemy->m_HP -= enemy[i - 1]->skill_injury;//技能伤害;
                enemy[i - 1]->skill_status = 0;
            }
        }
    }


    //4. 判断这一局对战有没有结束
    if (end_status == 0)
    {
        if ((live_player == 0 || live_enemy == 0) && remainingTime <= 0)
        {

            if (live_player > 0) { //我们赢了……………………………………………………………………………………………………………………………………退出
                
                auto emitter = ParticleSystemQuad::create("Begain.plist");
                if (emitter) {
                    // 创建批处理节点并使用粒子纹理来初始化
                    emitter->setScale(0.2f);
                    emitter->setPosition(1536 / 2 - getPositionX(), 1024 / 2 - getPositionY());

                    // 将粒子系统添加到批处理节点中
                    emitter->setDuration(2.4f);
                    addChild(emitter);
                }

                for (int i = 0; i < player_num; i++) {
                    if (player[i] != nullptr && player[i]->m_HP > 0) {
                        Vec2 newChessHeroPosition = Vec2(1536 / 2 - getPositionX(), 1024 / 2 - getPositionY());
                        float moveDuration = 2.0f;
                        auto moveTo = MoveTo::create(moveDuration, newChessHeroPosition);
                        player[i]->runAction(moveTo);
                    }
                    if(i==player_num-1)
                        end_status = 1;
                }
            }
            else {//他们赢了

                auto emitter = ParticleSystemQuad::create("Begain.plist");
                if (emitter) {
                    // 创建批处理节点并使用粒子纹理来初始化
                    emitter->setScale(0.2f);
                    emitter->setPosition(1536 / 2 - getPositionX(), 1024 / 2 - getPositionY());

                    // 将粒子系统添加到批处理节点中
                    emitter->setDuration(2.4f);
                    addChild(emitter);
                }

                for (int i = 0; i < enemy_num; i++) {
                    if (enemy[i] != nullptr && enemy[i]->m_HP > 0) {
                        Vec2 newChessHeroPosition = Vec2(1536 / 2 - getPositionX(), 1024 / 2 - getPositionY());
                        float moveDuration = 2.0f;
                        auto moveTo = MoveTo::create(moveDuration, newChessHeroPosition);
                        enemy[i]->runAction(moveTo);
                    }
                    if (i ==enemy_num - 1)
                        end_status = 1;
                }
            }
        }
    }

    for (int i = 0; i < player_num; i++) {
        if (player[i] != nullptr && player[i]->m_HP > 0) {
            if (end_status == 1 && player[i]->getPositionX() == 1536 / 2 - getPositionX() && player[i]->getPositionY() == 1024 / 2 - getPositionY())
                player[i]->setVisible(false);
        }
    }

    for (int i = 0; i < enemy_num; i++) {
        if (enemy[i] != nullptr && enemy[i]->m_HP > 0) {
            if (end_status == 1 && enemy[i]->getPositionX() == 1536 / 2 - getPositionX() && enemy[i]->getPositionY() == 1024 / 2 - getPositionY())
                enemy[i]->setVisible(false);
        }
    }

    if (end_status == 1) {
       exitLayer();
       end_status = 2;
    }
}

void  Chessboard::exitLayer() {
    if (Player::getInstance()->healthDown(40)) {
        Director::getInstance()->end();
        //Director::getInstance()->replaceScene(TransitionFade::create(1.0, EndScene::scene()));
    }
    else {
        Director::getInstance()->getScheduler()->schedule([=](float dt) {
            Director::getInstance()->popScene();
            }, this, 2.0f, 0, 0.0f, false, "popSceneCallback");
    }
}



void Chessboard::generateInformation(std::string& a, std::string& b, std::string& c)
{
    auto listener = EventListenerMouse::create();
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    std::string allInfo = a + b + c;

    // 创建标签并设置内容
    auto label = Label::createWithTTF(allInfo, "fonts/arial.ttf", 24);
    label->setPosition(1350, 600);
    this->addChild(label);
    label->setVisible(false);
    chesshero_label.push_back(label);
    listener->onMouseMove = [=](Event* event) {
        EventMouse* e = dynamic_cast<EventMouse*>(event);
        if (e) {
            Vec2 mousePos = Vec2(e->getCursorX(), e->getCursorY());
            Vec2 mouseSpritePos = convertToNodeSpace(mousePos);
            for (int i = 0; i < player_num; i++)
            {
                hideSpriteInfo(i); // 隐藏精灵信息
            }
            for (int i = 0; i < player_num; i++)
            {
                if (player[i]->getBoundingBox().containsPoint(mouseSpritePos)) {
                    showSpriteInfo(i);
                }
                else {
                    hideSpriteInfo(i); // 隐藏精灵信息
                }
            }
        }
    };
}

void Chessboard::hideSpriteInfo(int i)
{
    chesshero_label[i]->setVisible(false);
}

void Chessboard::showSpriteInfo(int i)
{
    chesshero_label[i]->setVisible(true);
}

void Chessboard::BattleMatch() {
    float dist = 0, dist_min = 0;
    int match[2][11];

    for (int j = 0; j <= 1; j++)
        for (int i = 0; i <= 10; i++)
            match[j][i] = 0;

   
    for (int i = 1; i <= player_num; i++) {
        dist_min = 0;
        player[i - 1]->enemy = nullptr;
        if (player[i - 1]->m_HP <= 0)
            continue;
        for (int j = 1; j <= enemy_num; j++) {
            if (enemy[j - 1]->m_HP <= 0)
                continue;
            if (dist_min == 0)
                dist_min = player[i - 1]->getPosition().distance(enemy[j - 1]->getPosition());
            dist = player[i - 1]->getPosition().distance(enemy[j - 1]->getPosition());
            if (dist <= dist_min)
            {
                match[0][i - 1] = j - 1;
                dist_min = dist;
            }
            player[i - 1]->enemy = enemy[match[0][i - 1]];
        }
        
    }

    
    for (int i = 1; i <= enemy_num; i++) {
        dist_min = 0;
        enemy[i - 1]->enemy = nullptr;
        if (enemy[i - 1]->m_HP <= 0)
            continue;
        for (int j = 1; j <= player_num; j++) {
            if (player[j - 1]->m_HP <= 0)
                continue;
            if (dist_min == 0)
                dist_min = enemy[i - 1]->getPosition().distance(player[j - 1]->getPosition());
            dist = enemy[i - 1]->getPosition().distance(player[j - 1]->getPosition());
            if (dist <= dist_min)
            {
                match[1][i - 1] = j - 1;
                dist_min = dist;
            }
            enemy[i - 1]->enemy = player[match[1][i - 1]];
        }
       
    }
}

//进行装备信息的传入与传递
void Chessboard::MessagePutIn() {
    return;
}
