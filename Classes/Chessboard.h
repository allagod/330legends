#ifndef __CHESSBOARD_H__
#define __CHESSBOARD_H__

#include <vector>
#include "cocos2d.h"
#include "ChessHero.h"

class Chessboard : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    bool init();

    void update(float dt);

    //���ڽ���AI���������������������ַ���ָ��
    //���ӵ����룬���˵�����
    void MessagePutIn();

    //���ڽ��ܷ���������ս�����ӵ���Ϣ
    void ChessHero_create();

    //����չʾ����ս�������б����˺�����ֵ
    void generateValue(float dt);

    //��ս���д������ṩ���ӵ���Ϣ
    void generateInformation(std::string& a, std::string& b, std::string& c);

    //��Ϣ�˵�������
    void hideSpriteInfo(int i);

    //��Ϣ�˵�����ʾ
    void showSpriteInfo(int i);

    //�л�ս�����뱸ս��Ľӿ�
    void  exitLayer();

    //��ʼ����ʱ
    void startCountdown(float duration);

    //ÿ����µ���ʱ
    void updateCountdown(float dt);

    //Ϊս������ÿһ�����ӷ������
    void BattleMatch();
 
    // implement the "static create()" method manually
    CREATE_FUNC(Chessboard);

private:
    std::vector<ChessHero*> player;//ս�������ҷ�����Ӣ������

    std::vector<ChessHero*> enemy;//ս�����ϵз�����Ӣ������

    int player_num = 0;//�ҷ�����Ӣ������
    int enemy_num = 0;//�з�����Ӣ������

    int end_status = 0;//ս������ֹ���л���־

    float remainingTime = 30.0;//ս��ҳ�浹��ʱ��ʱ��

    cocos2d::Label* countdownLabel;//����ʱ�˵�

    std::vector<cocos2d::Label*> chesshero_label;//����Ӣ�۵���Ϣ�˵�

    cocos2d::ProgressTimer* countdownTimer;//����ʱ��
};

#endif // __HELLOWORLD_SCENE_H__
