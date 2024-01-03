#include "cocos2d.h"
#include "3d/CCSprite3D.h"
#include "AIGameScene.h"
#include <string>
USING_NS_CC;
//#define movement
class New3DScene : public cocos2d::Scene {
public:
    enum State {
        State_None = 0,
        State_Idle = 0x01,
        State_Move = 0x02,
        State_Rotate = 0x04
    };
   /* ~New3DScene();*/
    static cocos2d::Scene* createScene();
    virtual bool init();
    void update(float delta);
    //用于更改名字
    std::string setOrcName(const std::string& name);
    CREATE_FUNC(New3DScene);
    // 切换到下一个场景的函数

    void switchToNextScene(float dt) {
        // 创建并切换到下一个场景
        auto nextScene = AIGameScene::scene(); // 替换为你的下一个场景类
        cocos2d::Director::getInstance()->replaceScene(nextScene);
    }
    ~New3DScene();
    /*virtual void onExit();*/

private:
    cocos2d::Label* _nameLabel; // 用于显示名字的标签
    std::string MyName;
    cocos2d::Camera* _camera;
    cocos2d::Vec3 _targetPos;
    unsigned int _curState;
    cocos2d::Sprite3D* _plane;
    cocos2d::Sprite3D* _orc;
    cocos2d::Label* _orcNameLabel;
    void onTouchesBegan(const std::vector<Touch*>& touches, cocos2d::Event* event);
    void onTouchesMoved(const std::vector<Touch*>& touches, cocos2d::Event* event);
    void onTouchesEnded(const std::vector<Touch*>& touches, cocos2d::Event* event);
    cocos2d::Action* _orcJumpAction = nullptr;
};