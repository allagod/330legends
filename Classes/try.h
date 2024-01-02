#pragma once
#include <string>

#include "cocos2d.h"
USING_NS_CC;

class New3DScene : public cocos2d::Scene {
public:
    enum State
    {
        State_None = 0,
        State_Idle = 0x01,
        State_Move = 0x02,
        State_Rotate = 0x04
    };
    void update(float delta);

    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(New3DScene);
    // 添加设置名字的方法
    std::string setOrcName(const std::string& name);
    void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);
    void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);
    void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);
private:
    cocos2d::Label* _nameLabel; // 用于显示名字的标签
    cocos2d::Camera* _camera;
    std::string MyName = "zzy is so handsome";

    cocos2d::Vec3 _targetPos;
    unsigned int   _curState;
    cocos2d::Sprite3D* _plane;
    cocos2d::Sprite3D* _orc;
    cocos2d::Label* _orcNameLabel;
    cocos2d::Action* _orcWalkAction = nullptr;
    cocos2d::Action* _orcIdleAction = nullptr;
    bool _isOrcWalking = false;
};
