#pragma once
#ifndef __Particle__
#define __Particle__
#include "cocos2d.h"
using namespace cocos2d;

class particle {
protected:
	cocos2d::ParticleSystemQuad* emitter;
public:
	~particle();
	virtual void onenter(float x, float y);
};


class particleBegain :public particle{
public:
	virtual void onenter(float x, float y);
};

class particleExplosion :public particle {
public:
	virtual void onenter(float x, float y);
};

class particleFire :public particle {
public:
	virtual void onenter(float x, float y);
};

class particleMetorRain :public particle {
public:
	virtual void onenter(float x, float y);
};

class particleNormalAttack :public particle {
public:
	virtual void onenter(float x, float y);
};

class particleSend :public particle {
public:
	virtual void onenter(float x, float y);
};

#endif // !__Particle__
