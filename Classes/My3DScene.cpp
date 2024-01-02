// ����3D������
#include"cocos2d.h"
USING_NS_CC;


class My3DScene : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(My3DScene);
};

cocos2d::Scene* My3DScene::createScene() {
    return My3DScene::create();
}

bool My3DScene::init() {
    if (!Scene::init()) {
        return false;
    }

    // ����3D����
    auto sprite3D = cocos2d::Sprite3D::create("path_to_your_model.c3b");

    // ����3D�����λ�á���ת������
    sprite3D->setPosition3D(cocos2d::Vec3(0, 0, 0));
    sprite3D->setScale(0.1f);

    // ���3D���鵽������
    this->addChild(sprite3D);

    // ���ù���
    auto light = cocos2d::AmbientLight::create(cocos2d::Color3B(200, 200, 200));
    this->addChild(light);

    // �������
    auto camera = cocos2d::Camera::createPerspective(60, (float)Director::getInstance()->getWinSize().width / Director::getInstance()->getWinSize().height, 1.0f, 1000.0f);
    camera->setPosition3D(cocos2d::Vec3(0, 0, 10));
    camera->lookAt(cocos2d::Vec3(0, 0, 0), cocos2d::Vec3(0, 1, 0));
    this->addChild(camera);

    return true;
}

