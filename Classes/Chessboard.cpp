#include "Chessboard.h"
#include "ChessHero.h"

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

// ��ʼ��
bool Chessboard::init()
{
    //////////////////////////////
    // 1.����ĳ�����
    if (!Layer::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2.�˵�����

    // ����ͼƬ״̬�趨��ص��������
    auto closeItem = MenuItemImage::create(
        "CloseNormal.png",
        "CloseSelected.png",
        CC_CALLBACK_1(Chessboard::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
        float y = origin.y + closeItem->getContentSize().height / 2;
        closeItem->setPosition(Vec2(x, y));
    }

    // ���𽫲˵���ӵ�ָ��λ��
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 4.���鲿��
    
    // �������̾���
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
    // �����峤�ȼ�Сһ��
    float scaleFactor = 0.5f;
    countdownTimer->setScaleY(scaleFactor);
    countdownTimer->setPosition(Vec2(780, 980));
    this->addChild(countdownTimer);
    remainingTime = 30;// �����޸ģ��趨ʱ��Ϊ30��

    /*
    5.���鴴��+��ʼ������
    */
    ChessHero_create();
  
     /////////////////////////////
     // 6.update
     //���̵ĸ��º���

     this->schedule([this](float dt) {
         this->update(dt);
         }, "update_key");
 
     this->schedule(CC_SCHEDULE_SELECTOR(Chessboard::generateValue), 1.0f, CC_REPEAT_FOREVER, 0);

     std::string health = "Health: 100\n";
     std::string name = "Name: Player\n";
     std::string defense = "Defense: 50\n";
     generateInformation(health, name, defense);

    return true;
}

void Chessboard::ChessHero_create() 
{
    char tmp[150] = { 0 }, tmp2[150] = { 0 };
    tmp[1] = 'd'; tmp[2] = '1';
    tmp[11] = 'e'; tmp[12] = '1';
    //tmp[21] = 'g'; tmp[22] = '1';

    tmp2[1] = 'j'; tmp2[2] = '1';

    //ǰ5�����ӵ���Ϣ���ڱ�ս������Ϣ
    int chess_type = 0, chess_grade = 0, m_x = 0, m_y = 0;
    int weapon1 = 0, weapon2 = 0, weapon3 = 0;

    for (int i = 1; i <= 140; i += 5) {
        chess_type = tmp[i], chess_grade = tmp[i + 1] - '1', weapon1 = tmp[i + 2] - '0', weapon2 = tmp[i + 3] - '0', weapon3 = tmp[i + 4] - '0';
        m_x = (1 + ((i - 1) / 5) / 7);
        m_y = (1 + ((i - 1) / 5) % 7);

        m_x = 400 + m_x * 100;
        m_y = 365 + m_y * 70;

        switch (chess_type) {
            case 'a':
                {
                    player.push_back(ChessHero::create("bingnv_attack1.png", 1, chess_grade));//#ͼƬ������
                    player_num++;
                    player[player_num - 1]->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                    player[player_num - 1]->setPosition(Vec2(m_x, m_y));   
                    player[player_num - 1]->position_status = 1;

                    //����
                    player[player_num - 1]->setScale(player[player_num - 1]->m_SpriteTypeValue);
                    player[player_num - 1]->setTag(1);                                        //#����
                    this->addChild(player[player_num - 1]);
                    break;
                }
            case 'b':
                {
                player.push_back(ChessHero::create("bobi_stand.png", 2, chess_grade));//#ͼƬ������
                player_num++;
                player[player_num - 1]->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                player[player_num - 1]->setPosition(Vec2(m_x, m_y));
                player[player_num - 1]->position_status = 1;

                //����
                player[player_num - 1]->setScale(player[player_num - 1]->m_SpriteTypeValue);
                player[player_num - 1]->setTag(2);                                       
                this->addChild(player[player_num - 1]);
                break;
                }
            case 'c':
                {
                player.push_back(ChessHero::create("gewen_stand.png", 3, chess_grade));//#ͼƬ������
                player_num++;
                player[player_num - 1]->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                player[player_num - 1]->setPosition(Vec2(m_x, m_y));
                player[player_num - 1]->position_status = 1;

                //����
                player[player_num - 1]->setScale(player[player_num - 1]->m_SpriteTypeValue);
                player[player_num - 1]->setTag(3);                                        //#����
                this->addChild(player[player_num - 1]);
                break;
                }
            case 'd':
                {
                player.push_back(ChessHero::create("kaerma_stand.png", 4, chess_grade));//#ͼƬ������
                player_num++;
                player[player_num - 1]->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                player[player_num - 1]->setPosition(Vec2(m_x, m_y));
                player[player_num - 1]->position_status = 1;

                //����
                player[player_num - 1]->setScale(player[player_num - 1]->m_SpriteTypeValue);
                player[player_num - 1]->setTag(4);                                        //#����
                this->addChild(player[player_num - 1]);
                break;
                }
            case 'e':
                {
                player.push_back(ChessHero::create("lakesi_stand.png", 5, chess_grade));//#ͼƬ������
                player_num++;
                player[player_num - 1]->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                player[player_num - 1]->setPosition(Vec2(m_x, m_y));
                player[player_num - 1]->position_status = 1;

                //����
                player[player_num - 1]->setScale(player[player_num - 1]->m_SpriteTypeValue);
                player[player_num - 1]->setTag(5);                                        //#����
                this->addChild(player[player_num - 1]);
                break;
                }
            case 'f':
                {
                player.push_back(ChessHero::create("lanbo_stand.png", 6, chess_grade));//#ͼƬ������
                player_num++;
                player[player_num - 1]->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                player[player_num - 1]->setPosition(Vec2(m_x, m_y));
                player[player_num - 1]->position_status = 1;

                //����
                player[player_num - 1]->setScale(player[player_num - 1]->m_SpriteTypeValue);
                player[player_num - 1]->setTag(6);                                        //#����
                this->addChild(player[player_num - 1]);
                break;
                }
            case 'g':
                {
                player.push_back(ChessHero::create("lienata_stand.png", 7, chess_grade));//#ͼƬ������
                player_num++;
                player[player_num - 1]->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                player[player_num - 1]->setPosition(Vec2(m_x, m_y));
                player[player_num - 1]->position_status = 1;

                //����
                player[player_num - 1]->setScale(player[player_num - 1]->m_SpriteTypeValue);
                player[player_num - 1]->setTag(7);                                        //#����
                this->addChild(player[player_num - 1]);
                break;
                }
            case'h':
                {
                player.push_back(ChessHero::create("niutou_stand.png", 8, chess_grade));//#ͼƬ������
                player_num++;
                player[player_num - 1]->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                player[player_num - 1]->setPosition(Vec2(m_x, m_y));
                player[player_num - 1]->position_status = 1;

                //����
                player[player_num - 1]->setScale(player[player_num - 1]->m_SpriteTypeValue);
                player[player_num - 1]->setTag(8);                                        //#����
                this->addChild(player[player_num - 1]);
                break;
                }
            case 'i':
                {
                player.push_back(ChessHero::create("tienan_stand.png", 9, chess_grade));//#ͼƬ������
                player_num++;
                player[player_num - 1]->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                player[player_num - 1]->setPosition(Vec2(m_x, m_y));
                player[player_num - 1]->position_status = 1;

                //����
                player[player_num - 1]->setScale(player[player_num - 1]->m_SpriteTypeValue);
                player[player_num - 1]->setTag(9);                                     //#����
                this->addChild(player[player_num - 1]);
                break;
                }
            case 'j':
                {
                player.push_back(ChessHero::create("wuqi_stand.png", 10, chess_grade));//#ͼƬ������
                player_num++;
                player[player_num - 1]->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                player[player_num - 1]->setPosition(Vec2(m_x, m_y));
                player[player_num - 1]->position_status = 1;

                //����
                player[player_num - 1]->setScale(player[player_num - 1]->m_SpriteTypeValue);
                player[player_num - 1]->setTag(10);                                    //#����
                this->addChild(player[player_num - 1]);
                break;
                }

        }

        player[player_num - 1]->weapon[0] = weapon1 - '0';
        player[player_num - 1]->weapon[1] = weapon2 - '0';
        player[player_num - 1]->weapon[2] = weapon3 - '0';

        player[player_num - 1]->wear_weapon();
    }

    //���������������������������������������������������������������ȴ����ݵڶ������顭����������������������������������������������������������������
    for (int i = 1; i <= 140; i += 5) {
        chess_type = tmp2[i], chess_grade = tmp2[i + 1] - '1', weapon1 = tmp2[i + 2] - '0', weapon2 = tmp2[i + 3] - '0', weapon3 = tmp2[i + 4] - '0';
        m_x = (8 - ((i - 1) / 5) / 7);
        m_y = (8 - ((i - 1) / 5) % 7);
            
        m_x = 400 + m_x * 100;
        m_y = 365 + m_y * 70;

        switch (chess_type) {
        case 'a':
        {
            enemy.push_back(ChessHero::create("bingnv_attack1.png", 1, chess_grade));//#ͼƬ������
            enemy_num++;
            enemy[enemy_num - 1]->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            enemy[enemy_num - 1]->setPosition(Vec2(m_x, m_y));
            enemy[enemy_num - 1]->position_status = 0;

            //����
            enemy[enemy_num - 1]->setScale(enemy[enemy_num - 1]->m_SpriteTypeValue);
            enemy[enemy_num - 1]->setTag(1);                                        //#����
            this->addChild(enemy[enemy_num - 1]);
            break;
        }
        case 'b':
        {
            enemy.push_back(ChessHero::create("bobi_stand_down.png", 2, chess_grade));//#ͼƬ������
            enemy_num++;
            enemy[enemy_num - 1]->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            enemy[enemy_num - 1]->setPosition(Vec2(m_x, m_y));
            enemy[enemy_num - 1]->position_status = 0;

            //����
            enemy[enemy_num - 1]->setScale(enemy[enemy_num - 1]->m_SpriteTypeValue);
            enemy[enemy_num - 1]->setTag(2);
            this->addChild(enemy[enemy_num - 1]);
            break;
        }
        case 'c':
        {
            enemy.push_back(ChessHero::create("gewen_stand_down.png", 3, chess_grade));//#ͼƬ������
            enemy_num++;
            enemy[enemy_num - 1]->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            enemy[enemy_num - 1]->setPosition(Vec2(m_x, m_y));
            enemy[enemy_num - 1]->position_status = 0;

            //����
            enemy[enemy_num - 1]->setScale(enemy[enemy_num - 1]->m_SpriteTypeValue);
            enemy[enemy_num - 1]->setTag(3);                                        //#����
            this->addChild(enemy[enemy_num - 1]);
            break;
        }
        case 'd':
        {
            enemy.push_back(ChessHero::create("kaerma_stand_down.png", 4, chess_grade));//#ͼƬ������
            enemy_num++;
            enemy[enemy_num - 1]->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            enemy[enemy_num - 1]->setPosition(Vec2(m_x, m_y));
            enemy[enemy_num - 1]->position_status = 0;

            //����
            enemy[enemy_num - 1]->setScale(enemy[enemy_num - 1]->m_SpriteTypeValue);
            enemy[enemy_num - 1]->setTag(4);                                        //#����
            this->addChild(enemy[enemy_num - 1]);
            break;
        }
        case 'e':
        {
            enemy.push_back(ChessHero::create("lakesi_stand_down.png", 5, chess_grade));//#ͼƬ������
            enemy_num++;
            enemy[enemy_num - 1]->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            enemy[enemy_num - 1]->setPosition(Vec2(m_x, m_y));
            enemy[enemy_num - 1]->position_status = 0;

            //����
            enemy[enemy_num - 1]->setScale(enemy[enemy_num - 1]->m_SpriteTypeValue);
            enemy[enemy_num - 1]->setTag(5);                                        //#����
            this->addChild(enemy[enemy_num - 1]);
            break;
        }
        case 'f':
        {
            enemy.push_back(ChessHero::create("lanbo_stand_down.png", 6, chess_grade));//#ͼƬ������
            enemy_num++;
            enemy[enemy_num - 1]->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            enemy[enemy_num - 1]->setPosition(Vec2(m_x, m_y));
            enemy[enemy_num - 1]->position_status = 0;

            //����
            enemy[enemy_num - 1]->setScale(enemy[enemy_num - 1]->m_SpriteTypeValue);
            enemy[enemy_num - 1]->setTag(6);                                        //#����
            this->addChild(enemy[enemy_num - 1]);
            break;
        }
        case 'g':
        {
            enemy.push_back(ChessHero::create("lienata_stand_down.png", 7, chess_grade));//#ͼƬ������
            enemy_num++;
            enemy[enemy_num - 1]->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            enemy[enemy_num - 1]->setPosition(Vec2(m_x, m_y));
            enemy[enemy_num - 1]->position_status = 0;

            //����
            enemy[enemy_num - 1]->setScale(enemy[enemy_num - 1]->m_SpriteTypeValue);
            enemy[enemy_num - 1]->setTag(7);                                        //#����
            this->addChild(enemy[enemy_num - 1]);
            break;
        }
        case'h':
        {
            enemy.push_back(ChessHero::create("niutou_stand_down.png", 8, chess_grade));//#ͼƬ������
            enemy_num++;
            enemy[enemy_num - 1]->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            enemy[enemy_num - 1]->setPosition(Vec2(m_x, m_y));
            enemy[enemy_num - 1]->position_status = 0;

            //����
            enemy[enemy_num - 1]->setScale(enemy[enemy_num - 1]->m_SpriteTypeValue);
            enemy[enemy_num - 1]->setTag(8);                                        //#����
            this->addChild(enemy[enemy_num - 1]);
            break;
        }
        case 'i':
        {
            enemy.push_back(ChessHero::create("tienan_stand_down.png", 9, chess_grade));//#ͼƬ������
            enemy_num++;
            enemy[enemy_num - 1]->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            enemy[enemy_num - 1]->setPosition(Vec2(m_x, m_y));
            enemy[enemy_num - 1]->position_status = 0;

            //����
            enemy[enemy_num - 1]->setScale(enemy[enemy_num - 1]->m_SpriteTypeValue);
            enemy[enemy_num - 1]->setTag(9);                                     //#����
            this->addChild(enemy[enemy_num - 1]);
            break;
        }
        case 'j':
        {
            enemy.push_back(ChessHero::create("wuqi_stand_down.png", 10, chess_grade));//#ͼƬ������
            enemy_num++;
            enemy[enemy_num - 1]->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            enemy[enemy_num - 1]->setPosition(Vec2(m_x, m_y));
            enemy[enemy_num - 1]->position_status = 0;

            //����
            enemy[enemy_num - 1]->setScale(enemy[enemy_num - 1]->m_SpriteTypeValue);
            enemy[enemy_num - 1]->setTag(10);                                    //#����
            this->addChild(enemy[enemy_num - 1]);
            break;
        }

        }

        enemy[enemy_num - 1]->weapon[0] = weapon1 - '0';
        enemy[enemy_num - 1]->weapon[1] = weapon2 - '0';
        enemy[enemy_num - 1]->weapon[2] = weapon3 - '0';

        enemy[enemy_num - 1]->wear_weapon();
    }
}


void  Chessboard::generateValue(float dt) {

    // ʹ�õ�ǰʱ����Ϊ���������
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    for (int i = 1; i <= player_num; i++) {
        if (player[i - 1]->enemy != nullptr && player[i - 1]->m_HP > 0 && player[i - 1]->enemy->m_HP > 0)
        {
            // ����100-300��Χ�ڵ������
            int health = std::rand() % 201 + 100; // 0��200֮���������ټ���100

            std::string healthString = std::to_string(health); // ������ת��Ϊ�ַ���

            auto label = Label::createWithTTF(healthString, "fonts/arial.ttf", 24); // ����һ�������� Label

            Size playerSize = player[i-1]->getContentSize();

            label->setPosition(Vec2(player[i - 1]->enemy->getPositionX()+playerSize.width/2, player[i - 1]->enemy->getPositionY()+ playerSize.height/2)); // �����ı���λ��Ϊ (x, y)
            this->addChild(label); // ��ӵ�������

            label->setTextColor(Color4B(255, 0, 0, 255)); // �����ı���ɫΪ��ɫ

            label->setScale(3.0f); // ���ó�ʼ��СΪ2��

            // ����һ�� ScaleTo �������ӵ�ǰ��С��2�������ŵ����մ�С��0.5����������2�������
            auto scaleAction = ScaleTo::create(0.5f, 0.5f);

            // �� label ִ���������
            label->runAction(scaleAction);

            auto fadeOutAction = FadeOut::create(1.0f);

            // ִ�� FadeOut ����
            label->runAction(fadeOutAction);
        }
    }
}

// Modify the update function to include continuous animation
void Chessboard::update(float dt)
{


    if(remainingTime > 0)
    {
        //����ʱ����
        remainingTime -= (1 / 60.0);

        //���µ���ʱ�ı�
        countdownLabel->setString(StringUtils::format("Countdown: %.0f", remainingTime));

        //���½�����
        float percentage = (remainingTime / attack_time) * 100;

        //ֱ�Ӹ��½������İٷֱ�
        countdownTimer->setPercentage(percentage);
    }
    
    //�����û�����(�ǲ���Ҫ���ǵģ���Ϊ����Ӣ�ۡ���Ӣ�ۡ���װ����װ���ϳɡ�ѡ�����ġ������������ڱ�ս����ʵ��)������������ʱ��ʹ�ûص������Ϳ���
    //1.ΨһҪʵ�ֵ��ǣ���װ��������ս����Ӣ��
    //����������������update��δ��������أ�����������������
    MessagePutIn();

    /////////////////////////////
    // 2.ʱ�̸���ƥ��Ķ���
    BattleMatch();
    
    int live_player = 0, live_enemy = 0;

   //3. ����ÿһ��Ѫ������0�����ӵ�״̬������״̬���ͷż���״̬��ʶ���͹ر����״̬���������ڸ����ĵ��˿�Ѫ��Ȼ�������������
    for (int i = 1; i <= player_num; i++) {
        if (player[i - 1]->animation_end == 0)
            live_player++;
        if (player[i - 1]->enemy != nullptr && player[i - 1]->m_HP > 0 && player[i - 1]->enemy->m_HP > 0) {
            if (player[i - 1]->attack_status == 1) {
                player[i - 1]->enemy->m_HP -= player[i - 1]->m_SpritePowerValue * player[i - 1]->m_SpriteAttackSpeed;//���鹥���ٶ�;
                player[i - 1]->enemy->healthBar->decreaseHealth(player[i - 1]->m_SpritePowerValue * player[i - 1]->m_SpriteAttackSpeed/3.5);
            }
            if (player[i - 1]->skill_status == 1) {
                player[i - 1]->enemy->m_HP -= player[i - 1]->skill_injury;//���鹥���ٶ�;
                player[i - 1]->skill_status = 0;
            }
        }
    }

    

    for (int i = 1; i <= enemy_num; i++) {
        if (enemy[i - 1]->animation_end == 0)
            live_enemy++;
        if (enemy[i - 1]->enemy != nullptr && enemy[i - 1]->m_HP > 0 && enemy[i - 1]->enemy->m_HP > 0) {        
            if (enemy[i - 1]->attack_status == 1) {
                enemy[i - 1]->enemy->m_HP -= enemy[i - 1]->m_SpritePowerValue * enemy[i - 1]->m_SpriteAttackSpeed;//���鹥���ٶ�;
                enemy[i - 1]->enemy->healthBar->decreaseHealth(enemy[i - 1]->m_SpritePowerValue * enemy[i - 1]->m_SpriteAttackSpeed/3);
            }
            if (enemy[i - 1]->skill_status == 1) {
                enemy[i - 1]->enemy->m_HP -= enemy[i - 1]->skill_injury;//���鹥���ٶ�;
                enemy[i - 1]->skill_status = 0;
            }
        }
    }


    //4. �ж���һ�ֶ�ս��û�н���
    if(end_status ==0 )
    {
        if ((live_player == 0 || live_enemy == 0) || remainingTime <= 0)
        {

            if (live_player > 0) { //����Ӯ�ˡ����������������������������������������������������������������������������������������������������˳�
                auto emitter = ParticleSystemQuad::create("Begain.plist");
                if (emitter) {
                    // ����������ڵ㲢ʹ��������������ʼ��
                    emitter->setScale(0.2f);
                    emitter->setPosition(1536 / 2 - getPositionX(), 1024 / 2 - getPositionY());

                    // ������ϵͳ��ӵ�������ڵ���
                    emitter->setDuration(2.4f);
                    addChild(emitter);
                }


                for (int i = 0; i < player_num; i++) {
                    if (player[i] != nullptr && player[i]->m_HP > 0) {
                        Vec2 newChessHeroPosition = Vec2(1536 / 2 - getPositionX(), 1024 / 2 - getPositionY());
                        float moveDuration = 2.0f;
                        auto moveTo = MoveTo::create(moveDuration, newChessHeroPosition);
                        auto exitLayerAction = CallFunc::create(CC_CALLBACK_0(Chessboard::exitLayer, this));
                        auto sequence = Sequence::create(moveTo, exitLayerAction, nullptr);
                        player[i]->runAction(sequence);
                    }
                }

                end_status = 1;
            }
            else {//����Ӯ��
                auto emitter = ParticleSystemQuad::create("Begain.plist");
                if (emitter) {
                    // ����������ڵ㲢ʹ��������������ʼ��
                    emitter->setScale(0.2f);
                    emitter->setPosition(1536 / 2 - getPositionX(), 1024 / 2 - getPositionY());

                    // ������ϵͳ��ӵ�������ڵ���
                    emitter->setDuration(2.4f);
                    addChild(emitter);
                }

                for (int i = 0; i < enemy_num; i++) {
                    if (enemy[i] != nullptr && enemy[i]->m_HP > 0) {
                        Vec2 newChessHeroPosition = Vec2(1536 / 2 - getPositionX(), 1024 / 2 - getPositionY());
                        float moveDuration = 2.0f;
                        auto moveTo = MoveTo::create(moveDuration, newChessHeroPosition);
                        auto exitLayerAction = CallFunc::create(CC_CALLBACK_0(Chessboard::exitLayer, this));
                        auto sequence = Sequence::create(moveTo, exitLayerAction, nullptr);
                        enemy[i]->runAction(sequence);
                    }
                }

                end_status = 1;
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
}
void  Chessboard::exitLayer() {
    //this->unscheduleAllSelectors();  // ��ѡ��ȡ�����ж�ʱ��
    Director::getInstance()->end(); // �滻Ϊ�����һ������
}
void Chessboard::generateInformation(std::string& a, std::string& b, std::string& c)
{
    auto listener = EventListenerMouse::create();
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    std::string allInfo = a + b + c;

    // ������ǩ����������
    chesshero_label = Label::createWithTTF(allInfo, "fonts/arial.ttf", 24);
    chesshero_label->setPosition(1350, 600);
    this->addChild(chesshero_label);
    chesshero_label->setVisible(false);

    listener->onMouseMove = [=](Event* event) {
        EventMouse* e = dynamic_cast<EventMouse*>(event);
        if (e) {
            Vec2 mousePos = Vec2(e->getCursorX(), e->getCursorY());
            Vec2 mouseSpritePos = convertToNodeSpace(mousePos);
            for(int i=0;i<player_num;i++)
            {
                if (player[i]->getBoundingBox().containsPoint(mouseSpritePos)) {
                    showSpriteInfo();
                }
                else {
                    hideSpriteInfo(); // ���ؾ�����Ϣ
                }
            }
        }
    };
}

void Chessboard::hideSpriteInfo()
{
    chesshero_label->setVisible(false);
}

void Chessboard::showSpriteInfo()
{
    chesshero_label->setVisible(true);
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

//����װ����Ϣ�Ĵ����봫��
void Chessboard::MessagePutIn() {
    return;
}

void Chessboard::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
