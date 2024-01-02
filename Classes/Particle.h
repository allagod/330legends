#pragma once
#ifndef __Particle__
#define __Particle__

class particle {
protected:
	cocos2d::ParticleSystemQuad* emitter;
public:
	~Particle();
	virtual void onenter(int x,int y);
};


class particleBegain :public particle{
public:
	virtual void onenter(int x, int y);
};

class particleExplosion :public particle {
public:
	virtual void onenter(int x, int y);
};

class particleFire :public particle {
public:
	virtual void onenter(int x, int y);
};

class particleMetorRain :public particle {
public:
	virtual void onenter(int x, int y);
};

class particleNormalAttack :public particle {
public:
	virtual void onenter(int x, int y);
};

class particleSend :public particle {
public:
	virtual void onenter(int x, int y);
};

#endif // !__Particle__
