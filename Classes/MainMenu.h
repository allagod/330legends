#ifndef __MW_CPP_SYSMENU_H__
#define __MW_CPP_SYSMENU_H__

#include "cocos2d.h"

class MainMenu : public cocos2d::Layer
{
private:
    Size winSize;
public:

    virtual bool init();


    static cocos2d::Scene* scene();


    CREATE_FUNC(MainMenu);
};


#endif/*__MW_CPP_SYSMENU_H__*/
