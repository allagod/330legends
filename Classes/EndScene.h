#pragma once
#ifndef __END_SCENE_H__
#define __END_SCENE_H__
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "ui/CocosGUI.h"
#include "cocos2d.h"
using namespace cocos2d; //��Ҫ���������
/*
��Դ�ļ���Ҫ��ɴӽ�����Ϸ���л���HomeScene���˵�������ϵ�в���
*/

class EndScene : public Layer {
public:
    bool init(); //�����Ҫ�����Ĵ�������
    static Scene* scene(); //д���̶�

    CREATE_FUNC(EndScene);
};

#endif
