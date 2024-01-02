#ifndef __MY3DSPRITE_H__
#define __MY3DSPRITE_H__

#include "cocos2d.h"
#include "3d/cocos3d.h"

USING_NS_CC;

class My3DScene : public cocos2d::Scene {
public:
public:
	enum State
	{
		State_None = 0,
		State_Idle = 0x01,
		State_Move = 0x02,
		State_Rotate = 0x04
	};
	static cocos2d::Scene* createScene();
	virtual bool init();
	void updateCamera(float fDelta);
	void move3D(float elapsedTime);
	void updateState(float elapsedTime);
	bool isState(unsigned int state, unsigned int bit) const;
	void onTouchesBegan(const std::vector<Touch*>& touches, cocos2d::Event* event);
	void onTouchesMoved(const std::vector<Touch*>& touches, cocos2d::Event* event);
	void onTouchesEnded(const std::vector<Touch*>& touches, cocos2d::Event* event);


	// implement the "static create()" method manually
	CREATE_FUNC(My3DScene);
private:
	cocos2d::Sprite3D* sprite3D;
	cocos2d::Camera* _camera;
	cocos2d::Vec3 _targetPos;
	unsigned int   _curState;
	cocos2d::Sprite3D* _plane;
	cocos2d::Sprite3D* _orc;
	cocos2d::backend::ProgramState* _state = nullptr;

};


#endif // __MY3DSPRITE_H__
