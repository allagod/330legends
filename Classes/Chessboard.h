#ifndef __CHESSBOARD_H__
#define __CHESSBOARD_H__

#include <vector>
#include "cocos2d.h"
#include "ChessHero.h"

class Chessboard : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    void update(float dt);

    //���ڽ���AI���������������������ַ���ָ��
    //���ӵ����룬���˵�����
    void MessagePutIn();

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

    void ChessHero_create();

    void generateValue(float dt);

    void generateInformation(std::string& a, std::string& b, std::string& c);

    void hideSpriteInfo();

    void showSpriteInfo();

    void exitLayer();
    // implement the "static create()" method manually
    CREATE_FUNC(Chessboard);

private:
    float remainingTime;
    cocos2d::Label* countdownLabel;
    cocos2d::Label* chesshero_label;
    cocos2d::ProgressTimer* countdownTimer;
    float attack_time = 0;
    bool end_status=0;
    std::vector<ChessHero*> player;
    std::vector<ChessHero*> enemy;
    int player_num=0;//�ҷ�����Ӣ������
    int enemy_num=0;//�з�����Ӣ������
    int chess_hash[10];//��������ָ��������ݵ����飬Ҫ��Ȼ����ֻ��ͨ��ͼƬ���ֱ棬����ʵ������ҲӦ����һ����־����˵  settag

    void BattleMatch();

    float animationSpeed = 0.1f;

    int small_targetDistance = 30;
    float small_movementSpeed =0.2f;
    int medium_targetDistance = 100;
    float medium_movementSpeed = 0.5f;
    int large_targetDistance = 250;
    float large_movementSpeed = 0.4f;
   
};

#endif // __HELLOWORLD_SCENE_H__
