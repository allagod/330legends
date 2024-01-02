#include "ChessHero.h"
std::mutex lock1;
std::vector<std::thread> threads;
USING_NS_CC;


ChessHero::ChessHero()
{
    retain();
}

ChessHero::~ChessHero()
{

    release();
}

/*注意：
* k是正数，代表精灵
* k是负数，代表小怪兽
* 其中：1-----------冰女
*      -1-----------法师小喽啰
*      -2-----------盾牌小喽啰
*/
bool ChessHero::init(const std::string& filename,const int &k,const int &upgrade)
{
    if (!Sprite::initWithFile(filename)) {
        return false;
    }

    switch (k) {
        case -1:
            m_SpriteAttackSpeed = 0.4f;//魔法怪物攻击速度（有小bug，数值增加以后伤害增加，攻击速度减小）

            m_HP = 400;//魔法怪物血量

            m_SpritePowerValue =2.0;//魔法怪物攻击力大小           

            m_AttackRange=820;//魔法怪物攻击范围

            m_action_num = 4;//攻击动作帧有几张
            m_action_num_down = 4;

            animation_end = 0;//用于等待死亡动作来播放结束动画

            m_SpriteTypeValue = 0.3f;//魔法小怪大小数值

            m_grade = 0;//等级

            attack_status = 0;//攻击状态

            firing_range = 2.6;//射程参数

            health_bar_scale = 2;

            health_bar_x = 10;

            health_bar_y = 1.8;
            break;

        case 1:
            m_SpriteAttackSpeed = 0.1f;//冰女攻击速度

            m_AttackRange = 1300;//冰女攻击范围

            m_HP = 600;//冰女血量

            m_MP = 10;//冰女蓝条

            m_grade = 1;//初始状态是1星

            m_SpriteTypeValue = 1.0f;//冰女大小数值

            m_SpritePowerValue = 2;//冰女攻击力大小

            firing_range = 1.1;

            m_action_num = 10;
            m_action_num_down = 10;

            animation_end = 0;

            m_grade = 0;

            attack_status = 0;

            health_bar_scale = 0.55;

            health_bar_x = 3;

            health_bar_y = 0.9;

            break;

        case 2:
            m_SpriteAttackSpeed = 0.15f;//波比攻击速度

            m_AttackRange = 130;//波比攻击范围

            m_HP = 900;//波比血量

            m_MP = 10;//波比蓝条

            m_grade = 1;//初始状态是1星

            m_SpriteTypeValue = 1.0f;//波比大小数值

            m_SpritePowerValue = 2;//波比攻击力大小

            firing_range = 1.1;

            m_action_num = 12;
            m_action_num_down = 12;

            animation_end = 0;

            m_grade = 0;

            attack_status = 0;

            health_bar_scale = 0.55;

            health_bar_x = 3;

            health_bar_y = 0.9;
            break;

        case 3://格温
            m_SpriteAttackSpeed = 0.15f;//攻击速度

            m_AttackRange = 130;//攻击范围

            m_HP = 700;//血量

            m_MP = 10;//蓝条

            m_grade = 1;//初始状态是1星

            m_SpriteTypeValue = 1.2f;//大小数值

            m_SpritePowerValue = 2;//攻击力大小

            firing_range = 1.1;

            m_action_num = 8;
            m_action_num_down = 10;

            animation_end = 0;

            m_grade = 0;

            attack_status = 0;

            health_bar_scale = 0.55;

            health_bar_x = 3;

            health_bar_y = 0.9;
            break;

        case 4://卡尔玛
            m_SpriteAttackSpeed = 0.15f;//攻击速度

            m_AttackRange = 830;//攻击范围

            m_HP = 600;//血量

            m_MP = 10;//蓝条

            m_grade = 1;//初始状态是1星

            m_SpriteTypeValue = 1.0f;//大小数值

            m_SpritePowerValue = 2;//攻击力大小

            firing_range = 1.1;

            m_action_num = 8;
            m_action_num_down = 9;

            animation_end = 0;

            m_grade = 0;

            attack_status = 0;

            health_bar_scale = 0.55;

            health_bar_x = 5;

            health_bar_y = 0.9;
            break;

        case 5://拉克丝
            m_SpriteAttackSpeed = 0.15f;//攻击速度

            m_AttackRange = 830;//攻击范围

            m_HP = 600;//血量

            m_MP = 10;//蓝条

            m_grade = 1;//初始状态是1星

            m_SpriteTypeValue = 1.3f;//大小数值

            m_SpritePowerValue = 2;//攻击力大小

            firing_range = 0.9;

            m_action_num = 9;
            m_action_num_down = 9;

            animation_end = 0;

            m_grade = 0;

            attack_status = 0;

            health_bar_scale = 0.4;

            health_bar_x = 3;

            health_bar_y = 0.9;
            break;

        case 6://兰博
            m_SpriteAttackSpeed = 0.15f;//攻击速度

            m_AttackRange = 130;//攻击范围

            m_HP = 800;//血量

            m_MP = 10;//蓝条

            m_grade = 1;//初始状态是1星

            m_SpriteTypeValue = 1.0f;//大小数值

            m_SpritePowerValue = 2;//攻击力大小

            firing_range = 1.1;

            m_action_num = 11;
            m_action_num_down = 10;

            animation_end = 0;

            m_grade = 0;

            attack_status = 0;

            health_bar_scale = 0.55;

            health_bar_x = 3;

            health_bar_y = 0.9;
            break;

        case 7://列娜塔
            m_SpriteAttackSpeed = 0.15f;//攻击速度

            m_AttackRange = 830;//攻击范围

            m_HP = 600;//血量

            m_MP = 10;//蓝条

            m_grade = 1;//初始状态是1星

            m_SpriteTypeValue = 1.0f;//大小数值

            m_SpritePowerValue = 2;//攻击力大小

            firing_range = 1.1;

            m_action_num = 9;
            m_action_num_down = 10;

            animation_end = 0;

            m_grade = 0;

            attack_status = 0;

            health_bar_scale = 0.55;

            health_bar_x = 3;

            health_bar_y = 0.9;
            break;

        case 8://牛头
            m_SpriteAttackSpeed = 0.15f;//攻击速度

            m_AttackRange = 130;//攻击范围

            m_HP = 900;//血量

            m_MP = 10;//蓝条

            m_grade = 1;//初始状态是1星

            m_SpriteTypeValue = 0.9f;//大小数值

            m_SpritePowerValue = 2;//攻击力大小

            firing_range = 1.1;

            m_action_num = 9;
            m_action_num_down = 8;

            animation_end = 0;

            m_grade = 0;

            attack_status = 0;

            health_bar_scale = 0.55;

            health_bar_x = 3;

            health_bar_y = 0.9;
            break;

        case 9://铁男
            m_SpriteAttackSpeed = 0.15f;//攻击速度

            m_AttackRange = 130;//攻击范围

            m_HP = 800;//血量

            m_MP = 10;//蓝条

            m_grade = 1;//初始状态是1星

            m_SpriteTypeValue = 0.9f;//大小数值

            m_SpritePowerValue = 2;//攻击力大小

            firing_range = 1.1;

            m_action_num = 3;
            m_action_num_down = 12;

            animation_end = 0;

            m_grade = 0;

            attack_status = 0;

            health_bar_scale = 0.55;

            health_bar_x = 3;

            health_bar_y = 0.9;
            break;

        case 10://武器
            m_SpriteAttackSpeed = 0.15f;//攻击速度

            m_AttackRange = 130;//攻击范围

            m_HP = 800;//血量

            m_MP = 10;//蓝条

            m_grade = 1;//初始状态是1星

            m_SpriteTypeValue = 1.0f;//大小数值

            m_SpritePowerValue = 2;//波比攻击力大小

            firing_range = 1.1;

            m_action_num = 13;
            m_action_num_down = 11;

            animation_end = 0;

            m_grade = 0;

            attack_status = 0;

            health_bar_scale = 0.55;

            health_bar_x = 3;

            health_bar_y = 0.9;
            break;


    }
   
    Size playerSize = getContentSize();
    healthBar = new HealthBar(100+50*(int(m_HP/300)));
    healthBar->init(getPositionX() - playerSize.width / health_bar_x, getPositionY() + playerSize.height / health_bar_y);
    healthBar->setScale(health_bar_scale);
    this->addChild(healthBar);

    ChessHero_upgrade(upgrade);

    this->schedule([this](float dt) {
        this->update(dt);
        }, "update_key");

    /////////////////////////////
    // 2.
    //棋子英雄的行为，每两秒更新一次，自己是移动还是攻击
    this->schedule(CC_SCHEDULE_SELECTOR(ChessHero::attack1), m_SpriteAttackSpeed* m_action_num, CC_REPEAT_FOREVER, 0);
    this->schedule(CC_SCHEDULE_SELECTOR(ChessHero::attack2), m_SpriteAttackSpeed* m_action_num_down, CC_REPEAT_FOREVER, 0);
    this->schedule(CC_SCHEDULE_SELECTOR(ChessHero::destroy), 2.0f, CC_REPEAT_FOREVER, 0);

    this->schedule(CC_SCHEDULE_SELECTOR(ChessHero::skill_make1), 5.0f, CC_REPEAT_FOREVER, 0);
    this->schedule(CC_SCHEDULE_SELECTOR(ChessHero::skill_make2), 5.0f, CC_REPEAT_FOREVER, 0);
    this->schedule(CC_SCHEDULE_SELECTOR(ChessHero::skill_make3), 8.0f, CC_REPEAT_FOREVER, 0);
   
    return true;
}



void ChessHero::wear_weapon() {

}

void ChessHero::skill_make1(float dt) {
    if (enemy == nullptr||m_HP<=0)
        return;
    if (getTag() == 2 || getTag() == 3) {
       
        auto emitter = ParticleSystemQuad::create("explosion.plist");
        if (emitter) {
            // 创建批处理节点并使用粒子纹理来初始化
            emitter->setScale(0.06f);

            emitter->setPosition(2 * (enemy->getPosition() - getPosition()));

            // 将粒子系统添加到批处理节点中
        
            emitter->setDuration(1.0f);

            // 将批处理节点添加到您的场景或其他节点中
            addChild(emitter);
        }

    }
        

}

void ChessHero::skill_make2(float dt) {
    if (enemy == nullptr || m_HP <= 0)
        return;
    if (getTag() == 1 || getTag() == 4) {
        auto emitter = ParticleSystemQuad::create("normalAttack.plist");
        if (emitter) {
            // 创建批处理节点并使用粒子纹理来初始化
            emitter->setScale(0.5);
            emitter->setPosition(1.2*(enemy->getPosition() - getPosition()));

            emitter->setDuration(0.5f);

            // 将批处理节点添加到您的场景或其他节点中
            addChild(emitter);
        }
    }
   
}

void ChessHero::skill_make3(float dt) {
    if (enemy == nullptr || m_HP <= 0)
        return;
    if (getTag() == 5 || getTag() == 7) {
        auto emitter = ParticleSystemQuad::create("Fire.plist");
        if (emitter) {
            // 创建批处理节点并使用粒子纹理来初始化
            emitter->setPosition(1.2 * (enemy->getPosition() - getPosition()));

            // 将粒子系统添加到批处理节点中
            emitter->setScale(0.1);
            emitter->setDuration(1.0f);

            // 将批处理节点添加到您的场景或其他节点中
            addChild(emitter);
        }
    }

    
}

//k代表是进步了几级
void ChessHero::ChessHero_upgrade(const int &k) {
    if (m_grade <= 3) {
        m_HP += 300 * k;//血量

        for (int i = 1; i <= k; i++){
            m_SpriteTypeValue *= 1.5;//精灵大小数值
        }
        m_SpritePowerValue += 0.4 * k;//精灵攻击力大小
        m_grade += k;//星级
    }
}


void ChessHero::update(float dt)
{
    /*HP<=0中后产生的影响要在这里写
    * 
    */
    if (m_HP <= 0) {
        enemy = nullptr;
        attack_status = 0;
        skill_status = 0;
    }
    else if (enemy != nullptr) {
        //时刻判定与enemy的距离，选择下一步行为
        Vec2 ChessHeroPosition = getPosition();
        Vec2 monsterPosition = enemy->getPosition();
        float distance = ChessHeroPosition.distance(monsterPosition);
        Vec2 direction = monsterPosition - ChessHeroPosition;
        direction.normalize();

        if (distance > m_AttackRange)
        {
            //逻辑移动部分
            attack_status = 0;
            Vec2 newChessHeroPosition = ChessHeroPosition + direction * 2;
            float distance_new = getPosition().getDistance(newChessHeroPosition);
            float moveDuration = distance_new / (50.0f);
            auto moveTo = MoveTo::create(moveDuration, newChessHeroPosition);
            runAction(moveTo);
        }
    }
}


void ChessHero::attack1(float dt)
{

    if (enemy == nullptr || m_HP <= 0)
    {
        return;
    }
    if (position_status == 0) {
        return;
    }
    //时刻判定与enemy的距离，选择下一步行为

    Vec2 ChessHeroPosition = getPosition();
    Vec2 monsterPosition = enemy->getPosition();
    float distance = ChessHeroPosition.distance(monsterPosition);
    Vec2 direction = monsterPosition - ChessHeroPosition;

    if (distance <= m_AttackRange)
        makeAttack();
}


void ChessHero::attack2(float dt)
{

    if (enemy == nullptr || m_HP <= 0)
    {
        return;
    }

    if (position_status == 1) {
        return;
    }
    //时刻判定与enemy的距离，选择下一步行为

    Vec2 ChessHeroPosition = getPosition();
    Vec2 monsterPosition = enemy->getPosition();
    float distance = ChessHeroPosition.distance(monsterPosition);
    Vec2 direction = monsterPosition - ChessHeroPosition;

    if (distance <= m_AttackRange)
    {

        attack_status = 1;

        if (getTag() == -1)
        {
            //攻击时，本体动作部分
            auto frames = getAnimation("magic_monster%d.png", m_action_num_down);
            auto animation = Animation::createWithSpriteFrames(frames, m_SpriteAttackSpeed);
            auto animate = Animate::create(animation);
            //注意这个保持两秒是要根据外面的周期定时器的周期时间来调整的
            auto sequence = Sequence::create(animate, nullptr);
            auto repeatSequence = Repeat::create(sequence, 1);
        
            this->runAction(repeatSequence);

            //攻击时，子弹动作部分
            fireProjectile();

        }
        else if (getTag() == 1) {
            //攻击时，本体动作部分
            auto frames = getAnimation("bingnv_attack_%d.png", m_action_num_down);
            auto animation = Animation::createWithSpriteFrames(frames, m_SpriteAttackSpeed);
            auto animate = Animate::create(animation);
            //注意这个保持两秒是要根据外面的周期定时器的周期时间来调整的
            auto sequence = Sequence::create(animate, nullptr);
            auto repeatSequence = Repeat::create(sequence, 1);
            this->runAction(repeatSequence);

            //攻击时，子弹动作部分
            fireProjectile();
        }
        else if (getTag() == 2)
        {
            //攻击时，本体动作部分
            auto frames = getAnimation("bobi_attack_down%d.png", m_action_num_down);
            auto animation = Animation::createWithSpriteFrames(frames, m_SpriteAttackSpeed);
            auto animate = Animate::create(animation);
            //注意这个保持两秒是要根据外面的周期定时器的周期时间来调整的
            auto sequence = Sequence::create(animate, nullptr);
            auto repeatSequence = Repeat::create(sequence, 1);
            this->runAction(repeatSequence);
        }
        else if (getTag() == 3) {
            //攻击时，本体动作部分
            auto frames = getAnimation("gewen_attack_down%d.png", m_action_num_down);
            auto animation = Animation::createWithSpriteFrames(frames, m_SpriteAttackSpeed);
            auto animate = Animate::create(animation);
            //注意这个保持两秒是要根据外面的周期定时器的周期时间来调整的
            auto sequence = Sequence::create(animate, nullptr);
            auto repeatSequence = Repeat::create(sequence, 1);
            this->runAction(repeatSequence);
        }
        else if (getTag() == 4) {
            //攻击时，本体动作部分
            auto frames = getAnimation("kaerma_attack_down%d.png", m_action_num_down);
            auto animation = Animation::createWithSpriteFrames(frames, m_SpriteAttackSpeed);
            auto animate = Animate::create(animation);
            //注意这个保持两秒是要根据外面的周期定时器的周期时间来调整的
            auto sequence = Sequence::create(animate, nullptr);
            auto repeatSequence = Repeat::create(sequence, 1);
            this->runAction(repeatSequence);

            //攻击时，子弹动作部分
            fireProjectile();
        }
        else if (getTag() == 5) {
            //攻击时，本体动作部分
            auto frames = getAnimation("lakesi_attack_down%d.png", m_action_num_down);
            auto animation = Animation::createWithSpriteFrames(frames, m_SpriteAttackSpeed);
            auto animate = Animate::create(animation);
            //注意这个保持两秒是要根据外面的周期定时器的周期时间来调整的
            auto sequence = Sequence::create(animate, nullptr);
            auto repeatSequence = Repeat::create(sequence, 1);
            this->runAction(repeatSequence);

            //攻击时，子弹动作部分
            fireProjectile();
        }
        else if (getTag() == 6) {
            //攻击时，本体动作部分
            auto frames = getAnimation("lanbo_attack_down%d.png", m_action_num_down);
            auto animation = Animation::createWithSpriteFrames(frames, m_SpriteAttackSpeed);
            auto animate = Animate::create(animation);
            //注意这个保持两秒是要根据外面的周期定时器的周期时间来调整的
            auto sequence = Sequence::create(animate, nullptr);
            auto repeatSequence = Repeat::create(sequence, 1);
            this->runAction(repeatSequence);
        }
        else if (getTag() == 7) {
            //攻击时，本体动作部分
            auto frames = getAnimation("lienata_attack_down%d.png", m_action_num_down);
            auto animation = Animation::createWithSpriteFrames(frames, m_SpriteAttackSpeed);
            auto animate = Animate::create(animation);
            //注意这个保持两秒是要根据外面的周期定时器的周期时间来调整的
            auto sequence = Sequence::create(animate, nullptr);
            auto repeatSequence = Repeat::create(sequence, 1);
            this->runAction(repeatSequence);

            //攻击时，子弹动作部分
            fireProjectile();

        }
        else if (getTag() == 8) {
            //攻击时，本体动作部分
            auto frames = getAnimation("niutou_attack_down%d.png", m_action_num_down);
            auto animation = Animation::createWithSpriteFrames(frames, m_SpriteAttackSpeed);
            auto animate = Animate::create(animation);
            //注意这个保持两秒是要根据外面的周期定时器的周期时间来调整的
            auto sequence = Sequence::create(animate, nullptr);
            auto repeatSequence = Repeat::create(sequence, 1);
            this->runAction(repeatSequence);
        }
        else if (getTag() == 9) {
            //攻击时，本体动作部分
            auto frames = getAnimation("tienan_attack_down%d.png", m_action_num_down);
            auto animation = Animation::createWithSpriteFrames(frames, m_SpriteAttackSpeed);
            auto animate = Animate::create(animation);
            //注意这个保持两秒是要根据外面的周期定时器的周期时间来调整的
            auto sequence = Sequence::create(animate, nullptr);
            auto repeatSequence = Repeat::create(sequence, 1);
            this->runAction(repeatSequence);
        }
        else if (getTag() == 10) {
            //攻击时，本体动作部分
            auto frames = getAnimation("wuqi_attack_down%d.png", m_action_num_down);
            auto animation = Animation::createWithSpriteFrames(frames, m_SpriteAttackSpeed);
            auto animate = Animate::create(animation);
            //注意这个保持两秒是要根据外面的周期定时器的周期时间来调整的
            auto sequence = Sequence::create(animate, nullptr);
            auto repeatSequence = Repeat::create(sequence, 1);
            this->runAction(repeatSequence);
        }
    }

}

void ChessHero::makeAttack()
{
    if (enemy == nullptr || m_HP <= 0)
    {
        return;
    }

    
    attack_status = 1;

    if(getTag() == -1)
    {
        //攻击时，本体动作部分
        
        auto audioID = AudioEngine::play2d("gongji.m4a", false);
        AudioEngine::setVolume(audioID, 0.5);
        auto frames = getAnimation("magic_monster%d.png", m_action_num);
        auto animation = Animation::createWithSpriteFrames(frames, m_SpriteAttackSpeed);
        auto animate = Animate::create(animation);
        //注意这个保持两秒是要根据外面的周期定时器的周期时间来调整的
       auto sequence = Sequence::create(animate,  nullptr);
        auto repeatSequence = Repeat::create(sequence, 1);

        //……………………………………………………………………………………………………………………………………
        //auto playSoundCallback = CallFunc::create([this]() {
        //    int audioId = AudioEngine::play2d("magic_attack.m4a", false);
        //    });

       

        //// 将回调函数与动画序列组合
        //auto sequence = Sequence::create(playSoundCallback, animate, nullptr);

        //// 重复执行整个序列一次
        //auto repeatSequence = Repeat::create(sequence, 1);
      
        //……………………………………………………………………………………………………………………………………
        this->runAction(repeatSequence);

        //攻击时，子弹动作部分
        fireProjectile();
      
    }
    else if (getTag() == 1) {
        //攻击时，本体动作部分
        auto frames = getAnimation("bingnv_attack_%d.png", m_action_num);
        auto animation = Animation::createWithSpriteFrames(frames, m_SpriteAttackSpeed);
        auto animate = Animate::create(animation);
        //注意这个保持两秒是要根据外面的周期定时器的周期时间来调整的
        auto sequence = Sequence::create(animate, nullptr);
        auto repeatSequence = Repeat::create(sequence, 1);
        this->runAction(repeatSequence);
     
        //攻击时，子弹动作部分
        fireProjectile();
    }
    else if (getTag() == 2)
    {
        //攻击时，本体动作部分
        auto frames = getAnimation("bobi_attack%d.png", m_action_num);
        auto animation = Animation::createWithSpriteFrames(frames, m_SpriteAttackSpeed);
        auto animate = Animate::create(animation);
        //注意这个保持两秒是要根据外面的周期定时器的周期时间来调整的
        auto sequence = Sequence::create(animate, nullptr);
        auto repeatSequence = Repeat::create(sequence, 1);
        this->runAction(repeatSequence);
    }
    else if (getTag() == 3) {
        //攻击时，本体动作部分
        auto frames = getAnimation("gewen_attack%d.png", m_action_num);
        auto animation = Animation::createWithSpriteFrames(frames, m_SpriteAttackSpeed);
        auto animate = Animate::create(animation);
        //注意这个保持两秒是要根据外面的周期定时器的周期时间来调整的
        auto sequence = Sequence::create(animate, nullptr);
        auto repeatSequence = Repeat::create(sequence, 1);
        this->runAction(repeatSequence);
    }
    else if (getTag() == 4) {
        //攻击时，本体动作部分
        auto frames = getAnimation("kaerma_attack%d.png", m_action_num);
        auto animation = Animation::createWithSpriteFrames(frames, m_SpriteAttackSpeed);
        auto animate = Animate::create(animation);
        //注意这个保持两秒是要根据外面的周期定时器的周期时间来调整的
        auto sequence = Sequence::create(animate, nullptr);
        auto repeatSequence = Repeat::create(sequence, 1);
        this->runAction(repeatSequence);

        //攻击时，子弹动作部分
        fireProjectile();
    }
    else if (getTag() == 5) {
        //攻击时，本体动作部分
        auto frames = getAnimation("lakesi_attack%d.png", m_action_num);
        auto animation = Animation::createWithSpriteFrames(frames, m_SpriteAttackSpeed);
        auto animate = Animate::create(animation);
        //注意这个保持两秒是要根据外面的周期定时器的周期时间来调整的
        auto sequence = Sequence::create(animate, nullptr);
        auto repeatSequence = Repeat::create(sequence, 1);
        this->runAction(repeatSequence);

        //攻击时，子弹动作部分
        fireProjectile();
    }
    else if (getTag() == 6) {
        //攻击时，本体动作部分
        auto frames = getAnimation("lanbo_attack%d.png", m_action_num);
        auto animation = Animation::createWithSpriteFrames(frames, m_SpriteAttackSpeed);
        auto animate = Animate::create(animation);
        //注意这个保持两秒是要根据外面的周期定时器的周期时间来调整的
        auto sequence = Sequence::create(animate, nullptr);
        auto repeatSequence = Repeat::create(sequence, 1);
        this->runAction(repeatSequence);
    }
    else if (getTag() ==7) {
        //攻击时，本体动作部分
        auto frames = getAnimation("lienata_attack%d.png", m_action_num);
        auto animation = Animation::createWithSpriteFrames(frames, m_SpriteAttackSpeed);
        auto animate = Animate::create(animation);
        //注意这个保持两秒是要根据外面的周期定时器的周期时间来调整的
        auto sequence = Sequence::create(animate, nullptr);
        auto repeatSequence = Repeat::create(sequence, 1);
        this->runAction(repeatSequence);

        //攻击时，子弹动作部分
        fireProjectile();

    }
    else if (getTag() == 8) {
        //攻击时，本体动作部分
        auto frames = getAnimation("niutou_attack%d.png", m_action_num);
        auto animation = Animation::createWithSpriteFrames(frames, m_SpriteAttackSpeed);
        auto animate = Animate::create(animation);
        //注意这个保持两秒是要根据外面的周期定时器的周期时间来调整的
        auto sequence = Sequence::create(animate, nullptr);
        auto repeatSequence = Repeat::create(sequence, 1);
        this->runAction(repeatSequence);
    }
    else if (getTag() == 9) {
        //攻击时，本体动作部分
        auto frames = getAnimation("tienan_attack%d.png", m_action_num);
        auto animation = Animation::createWithSpriteFrames(frames, m_SpriteAttackSpeed);
        auto animate = Animate::create(animation);
        //注意这个保持两秒是要根据外面的周期定时器的周期时间来调整的
        auto sequence = Sequence::create(animate, nullptr);
        auto repeatSequence = Repeat::create(sequence, 1);
        this->runAction(repeatSequence);
    }
    else if (getTag() == 10) {
        //攻击时，本体动作部分
        auto frames = getAnimation("wuqi_attack%d.png", m_action_num);
        auto animation = Animation::createWithSpriteFrames(frames, m_SpriteAttackSpeed);
        auto animate = Animate::create(animation);
        //注意这个保持两秒是要根据外面的周期定时器的周期时间来调整的
        auto sequence = Sequence::create(animate, nullptr);
        auto repeatSequence = Repeat::create(sequence, 1);
        this->runAction(repeatSequence);
    }
    
}

void ChessHero::fireProjectile()
{
    if (enemy == nullptr || m_HP <= 0)
    {
        return;
    }
    //…………法师创建子弹精灵部分…………
    Texture2D* texture1;
    if(getTag()==-1)
        texture1 = Director::getInstance()->getTextureCache()->getTextureForKey("hotball.png");
    else if (getTag() == 1) 
        texture1 = Director::getInstance()->getTextureCache()->getTextureForKey("bingnv_zidan.png");
    else if (getTag() == 4)
        texture1 = Director::getInstance()->getTextureCache()->getTextureForKey("kaerma_zidan.png");
    else if (getTag() == 5)
        texture1 = Director::getInstance()->getTextureCache()->getTextureForKey("lakesi_zidan.png");
    else if (getTag() == 7)
        texture1 = Director::getInstance()->getTextureCache()->getTextureForKey("lienata_zidan.png");

    auto projectile = Sprite::createWithTexture(texture1);
    projectile->setAnchorPoint(Vec2::ANCHOR_MIDDLE);

    //注意：20是一个未解之数字，是位置校准常数，但你千万别改
    projectile->setPosition(getPosition()/20);


    if (getTag() == -1)
        projectile->setScale(1.5f);
    else if(getTag()==1)
        projectile->setScale(0.5f);
    else if(getTag()==5)
        projectile->setScale(0.5f);
    else if(getTag()==7)
        projectile->setScale(0.5f);



    this->addChild(projectile);

    //…………子弹校准部分…………
    float tanValue = ( getPositionX() -  enemy->getPositionX()) / ( getPositionY() -  enemy->getPositionY());
    float rotationAngle = atan2(tanValue, 1.0f) * 180.0f / M_PI;
    projectile->setRotation(rotationAngle);

    //…………子弹发射部分…………
    Vec2 direction = (enemy->getPosition() -  getPosition()).getNormalized();
    double distance = std::sqrt(std::pow( enemy->getPositionX() -  getPositionX(), 2) + std::pow( enemy->getPositionY() -  getPositionY(), 2));
    auto moveAction = MoveBy::create(1.0f, direction * firing_range * distance);

    auto removeCallback = CallFuncN::create([this](Node* node) {
        node->removeFromParentAndCleanup(true);
        });

    projectile->runAction(Sequence::create(moveAction, removeCallback, nullptr));
}

void  ChessHero::destroy(float dt)
{
    if (m_HP <= 0&& end_flag == 0) {
        //第四步：死亡动画
        end_flag = 1;
        switch (getTag()) {
            case -1:
                {
                    auto frames1 = getAnimation("monster_death%d.png", 3);
                    auto animation1 = Animation::createWithSpriteFrames(frames1, 1.0f / 3);
                    auto animate1 = Animate::create(animation1);
                    auto repeatAnimate1 = Repeat::create(animate1, 1);
                    runAction(repeatAnimate1);
                    auto hideCallback1 = CallFunc::create([this]() {
                        this->setVisible(false);
                        animation_end = 1;
                        removeFromParent();
                        });
                    auto sequence1 = Sequence::create(repeatAnimate1, hideCallback1, nullptr);
                    runAction(sequence1);
                    break;
                }
            case 1:
                {
                    auto frames2 = getAnimation("bingnv_death%d.png", 3);
                    auto animation2 = Animation::createWithSpriteFrames(frames2, 2.0f / 3);
                    auto animate2 = Animate::create(animation2);
                    auto repeatAnimate2 = Repeat::create(animate2, 1);
                    runAction(repeatAnimate2);
                    auto hideCallback2 = CallFunc::create([this]() {
                        this->setVisible(false);
                        animation_end = 1;
                        removeFromParent();
                       
                        });
                    auto sequence2 = Sequence::create(repeatAnimate2, hideCallback2, nullptr);
                    runAction(sequence2);
                    break;
                }
            case 2: 
            {
                auto frames3 = getAnimation("bobi_death%d.png", 1);
                auto animation3 = Animation::createWithSpriteFrames(frames3, 1.0f / 1);
                auto animate3 = Animate::create(animation3);
                auto repeatAnimate3 = Repeat::create(animate3, 1);
                runAction(repeatAnimate3);
                auto hideCallback3 = CallFunc::create([this]() {
                    this->setVisible(false);
                    animation_end = 1;
                    removeFromParent();

                    });
                auto sequence3 = Sequence::create(repeatAnimate3, hideCallback3, nullptr);
                runAction(sequence3);
                break;
            }
            case 3:
            {
                auto frames0 = getAnimation("gewen_death%d.png", 3);
                auto animation0 = Animation::createWithSpriteFrames(frames0, 2.0f / 3);
                auto animate0 = Animate::create(animation0);
                auto repeatAnimate0 = Repeat::create(animate0, 1);
                runAction(repeatAnimate0);
                auto hideCallback0 = CallFunc::create([this]() {
                    this->setVisible(false);
                    animation_end = 1;
                    removeFromParent();

                    });
                auto sequence0 = Sequence::create(repeatAnimate0, hideCallback0, nullptr);
                runAction(sequence0);
                break;
            }
            case 4:
            {
                auto frames4 = getAnimation("kaerma_death%d.png", 3);
                auto animation4 = Animation::createWithSpriteFrames(frames4, 2.0f / 3);
                auto animate4 = Animate::create(animation4);
                auto repeatAnimate4 = Repeat::create(animate4, 1);
                runAction(repeatAnimate4);
                auto hideCallback4 = CallFunc::create([this]() {
                    this->setVisible(false);
                    animation_end = 1;
                    removeFromParent();

                    });
                auto sequence4 = Sequence::create(repeatAnimate4, hideCallback4, nullptr);
                runAction(sequence4);
                break;
            }
            case 5:
            {
                auto frames5 = getAnimation("lakesi_death%d.png", 3);
                auto animation5 = Animation::createWithSpriteFrames(frames5, 2.0f / 3);
                auto animate5 = Animate::create(animation5);
                auto repeatAnimate5 = Repeat::create(animate5, 1);
                runAction(repeatAnimate5);
                auto hideCallback5 = CallFunc::create([this]() {
                    this->setVisible(false);
                    animation_end = 1;
                    removeFromParent();

                    });
                auto sequence5 = Sequence::create(repeatAnimate5, hideCallback5, nullptr);
                runAction(sequence5);
                break;
            }
            case 6:
            {
                auto frames6 = getAnimation("lanbo_death%d.png", 3);
                auto animation6 = Animation::createWithSpriteFrames(frames6, 2.0f / 3);
                auto animate6 = Animate::create(animation6);
                auto repeatAnimate6 = Repeat::create(animate6, 1);
                runAction(repeatAnimate6);
                auto hideCallback6 = CallFunc::create([this]() {
                    this->setVisible(false);
                    animation_end = 1;
                    removeFromParent();

                    });
                auto sequence6 = Sequence::create(repeatAnimate6, hideCallback6, nullptr);
                runAction(sequence6);
                break;
            }
            case 7:
            {
                auto frames7 = getAnimation("lienata_death%d.png", 3);
                auto animation7 = Animation::createWithSpriteFrames(frames7, 2.0f / 3);
                auto animate7 = Animate::create(animation7);
                auto repeatAnimate7 = Repeat::create(animate7, 1);
                runAction(repeatAnimate7);
                auto hideCallback7 = CallFunc::create([this]() {
                    this->setVisible(false);
                    animation_end = 1;
                    removeFromParent();

                    });
                auto sequence7 = Sequence::create(repeatAnimate7, hideCallback7, nullptr);
                runAction(sequence7);
                break;
            }
            case 8:
            {
                auto frames8 = getAnimation("niutou_death%d.png", 3);
                auto animation8 = Animation::createWithSpriteFrames(frames8, 2.0f / 3);
                auto animate8 = Animate::create(animation8);
                auto repeatAnimate8 = Repeat::create(animate8, 1);
                runAction(repeatAnimate8);
                auto hideCallback8 = CallFunc::create([this]() {
                    this->setVisible(false);
                    animation_end = 1;
                    removeFromParent();

                    });
                auto sequence8 = Sequence::create(repeatAnimate8, hideCallback8, nullptr);
                runAction(sequence8);
                break;
            }
            case 9:
            {
                auto frames9 = getAnimation("tienan_death%d.png", 3);
                auto animation9 = Animation::createWithSpriteFrames(frames9, 2.0f / 3);
                auto animate9 = Animate::create(animation9);
                auto repeatAnimate9 = Repeat::create(animate9, 1);
                runAction(repeatAnimate9);
                auto hideCallback9 = CallFunc::create([this]() {
                    this->setVisible(false);
                    animation_end = 1;
                    removeFromParent();

                    });
                auto sequence9 = Sequence::create(repeatAnimate9, hideCallback9, nullptr);
                runAction(sequence9);
                break;
            }
            case 10:
            {
                auto frames10 = getAnimation("wuqi_death%d.png", 3);
                auto animation10 = Animation::createWithSpriteFrames(frames10, 2.0f / 3);
                auto animate10 = Animate::create(animation10);
                auto repeatAnimate10 = Repeat::create(animate10, 1);
                runAction(repeatAnimate10);
                auto hideCallback10 = CallFunc::create([this]() {
                    this->setVisible(false);
                    animation_end = 1;
                    removeFromParent();

                    });
                auto sequence10 = Sequence::create(repeatAnimate10, hideCallback10, nullptr);
                runAction(sequence10);
                break;
            }
        }
    }
   

   
}

Vector<SpriteFrame*> ChessHero::getAnimation(const char* format, int count)
{
    auto spritecache = SpriteFrameCache::getInstance();
    Vector<SpriteFrame*> animFrames;
    char str[100];
    for (int i = 1; i <= count; i++)
    {
        sprintf(str, format, i);
        animFrames.pushBack(spritecache->getSpriteFrameByName(str));
    }
    return animFrames;
}

