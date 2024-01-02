#include "cocos2d.h"
#include "3d/CCSprite3D.h"
#include <string>
#include"try.h"

USING_NS_CC;
//#define movement
cocos2d::Scene* New3DScene::createScene() {
    return New3DScene::create();
}

bool New3DScene::init() {
    if (!Scene::init()) {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();

    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesBegan = CC_CALLBACK_2(New3DScene::onTouchesBegan, this);
    listener->onTouchesMoved = CC_CALLBACK_2(New3DScene::onTouchesMoved, this);
    listener->onTouchesEnded = CC_CALLBACK_2(New3DScene::onTouchesEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    auto layer = Layer::create();
    addChild(layer, 0);

    // Create Camera
    _camera = Camera::createPerspective(60, visibleSize.width / visibleSize.height, 0.1f, 200);
    _camera->setCameraFlag(CameraFlag::USER1);
    //// �����ʼλ�ú���ת��ˮƽ����orc
    _camera->setPosition3D(Vec3(0.0f, 2.0f, 8.0f));


    /*_camera->setPosition3D(Vec3(0.0f, 17.5f, 8.0f));
    _camera->setRotation3D(Vec3(-90.0f, 0.0f, 0.0f));*/

    // Create a plane
    _plane = Sprite3D::create("plane.c3t");
    _plane->setRotation3D(Vec3(90.0f, 0.0f, 0.0f));
    layer->addChild(_plane);

    Vec2 originalPlaneSize(5000.0f, 5000.0f); 
    Size windowSize = Director::getInstance()->getWinSize();
    float scaleX = windowSize.width / originalPlaneSize.x;
    float scaleY = windowSize.height / originalPlaneSize.y;
    _plane->setScaleX(scaleX*1.2);
    _plane->setScaleY(scaleY*1.2);
    _plane->setPosition3D(Vec3(0.0f, 0.0f, 10.0f));


    // Create the orc
    _orc = Sprite3D::create("girl.c3b");
    _orc->setScale(0.03f);
    _orc->setRotation3D(Vec3(0.0f, 0.0f, 0.0f));
    _orc->setPosition3D(Vec3(0.0f, 0.0f, 10.0f));
    _targetPos = _orc->getPosition3D();
    layer->addChild(_orc);
    //��Ӷ���
    auto animation = Animation3D::create("girl.c3b");

    if (animation)
    {
        auto animate = Animate3D::create(animation);

        _orc->runAction(RepeatForever::create(animate));
    }


    layer->addChild(_camera);
    // �����������
    auto circleAndRise = CallFunc::create([this]() {
        Vec3 center = _orc->getPosition3D();
        Vec3 finalPosition = Vec3(0.0f, 17.5f, 8.0f);
        Vec3 finalRotation = Vec3(-90.0f, 0.0f, 0.0f);
        float radius = 8.0f;
        float totalDuration = 7.0f; // �ܶ���ʱ��
        float circleDuration = 4.0f; // ����ʱ��
        float riseDuration = 1.8f; // ����ʱ��
        float stayDuration = 1.0f; // ��ֹʱ��

        this->schedule([this, center, finalPosition, finalRotation, radius, circleDuration, riseDuration, stayDuration, totalDuration](float dt) {
            static float elapsed = 0;
            elapsed += dt;

            if (elapsed >= totalDuration) {
                this->unschedule("camera_update");
                _camera->setPosition3D(finalPosition);
                _camera->setRotation3D(finalRotation);
                return;
            }

            float progress;
            Vec3 newPosition;
            if (elapsed <= stayDuration) {
                // ��ֹ״̬
                newPosition = center + Vec3(0.0f, 2.0f, radius);
            }
            else if (elapsed <= (stayDuration + circleDuration)) {
                // ����״̬
                progress = (elapsed - stayDuration) / circleDuration;
                float angle = progress * 2 * M_PI;
                newPosition = center + Vec3(radius * sinf(angle), 2.0f, radius * cosf(angle));
            }
            else {
                // ����״̬
                progress = (elapsed - stayDuration - circleDuration) / riseDuration;
                float angle = progress * 2 * M_PI; // ������Բ
                newPosition = center + Vec3(radius * sinf(angle), 2.0f + (finalPosition.y - 2.0f) * progress, radius * cosf(angle));
            }

            _camera->setPosition3D(newPosition);
            _camera->lookAt(center);
            }, 0, "camera_update");
        });

    this->runAction(Sequence::create(DelayTime::create(0.1f), circleAndRise, nullptr));
    layer->setCameraMask(2);
#ifdef movement
    // ���ز�������·����
    auto walkAnimation = Animation3D::create("orc.c3b", "walk");
    _orcWalkAction = RepeatForever::create(Animate3D::create(walkAnimation));
    _orcWalkAction->retain(); // ��������

    // ���ز�������������
    auto idleAnimation = Animation3D::create("orc.c3b", "idle"); // ȷ���˴����ļ����Ͷ���������ȷ
    _orcIdleAction = RepeatForever::create(Animate3D::create(idleAnimation));
    _orcIdleAction->retain(); // ��������

    _orc->runAction(_orcIdleAction); // ��ʼʱ���Ŵ�������

#endif // movement


    _orcNameLabel = Label::createWithTTF(MyName, "fonts/Marker Felt.ttf", 24);
    if (_orcNameLabel != nullptr) {
        _orcNameLabel->setColor(Color3B::WHITE);
        this->addChild(_orcNameLabel, 1); // ȷ����ǩ��3D����֮��
    }

    // ������պ���
    auto skybox = Skybox::create("pink.png", "pink.png", "pink.png", "pink.png", "pink.png", "pink.png");
    // ����պ�����ӵ�����
    this->addChild(skybox);

    // Create a Sprite and apply it as a texture to the plane
    auto texture = Director::getInstance()->getTextureCache()->addImage("chessboard.png");
    auto sprite = Sprite::createWithTexture(texture);
    sprite->setFlippedY(true); // Uncomment if you need to flip the texture
    
    _plane->setTexture("chessboard.png");
    _plane->addChild(sprite);
    scheduleUpdate(); // Schedule update method

    return true;
}

void New3DScene::update(float delta) {
    if (_orc) {
        Vec3 direction = _targetPos - _orc->getPosition3D();
        float distance = direction.length();
        if (distance > 0.5f) { // Set a threshold to stop the movement
            direction.normalize();
            float speed = std::min(distance, 50.0f); // Increase the speed
            _orc->setPosition3D(_orc->getPosition3D() + direction * speed * delta);

            // Update the rotation to face the target position
            Vec3 newFaceDir = _targetPos - _orc->getPosition3D();
            newFaceDir.y = 0.0f; // Keep the y component zero to rotate only on the y-axis
            newFaceDir.normalize();
            float newAngle = CC_RADIANS_TO_DEGREES(atan2(newFaceDir.z, -newFaceDir.x));
            _orc->setRotation3D(Vec3(0.0f, newAngle - 90.0f, 0.0f)); // Adjusting by -90 degrees
        }
#ifdef movement
        // �ж�orc�Ƿ����ƶ�
        if (distance > 0.5f) { // �ƶ���ֵΪ0.5f
            if (!_isOrcWalking) {
                _orc->stopAllActions();
                _orc->runAction(_orcWalkAction);
                _isOrcWalking = true;
            }
        }
        else {
            if (_isOrcWalking) {
                _orc->stopAllActions();
                _orc->runAction(_orcIdleAction);
                _isOrcWalking = false;
            }
        }
#endif // movement

        if (_orc && _orcNameLabel) {
            Vec3 orcPosition = _orc->getPosition3D();
            Vec2 labelPosition = _camera->project(Vec3(orcPosition.x, orcPosition.y, orcPosition.z));
            labelPosition.y = Director::getInstance()->getVisibleSize().height - labelPosition.y; // ��תY��
            _orcNameLabel->setPosition(labelPosition.x, labelPosition.y + 10); // ��������ϴ�ֱƫ����
        }
    }
}
// ʵ���������ֵķ���
std::string New3DScene::setOrcName(const std::string& name="zzy is handsome") {
    MyName=name;
    return name;
}

void New3DScene::onTouchesBegan(const std::vector<Touch*>& touches, cocos2d::Event* event) {
    // Implement if needed
}

void New3DScene::onTouchesMoved(const std::vector<Touch*>& touches, cocos2d::Event* event) {
    // Implement if needed
}

void New3DScene::onTouchesEnded(const std::vector<Touch*>& touches, cocos2d::Event* event) {
    for (auto& touch : touches) {
        auto location = touch->getLocationInView();
        if (_camera) {
            Vec3 nearP = _camera->unproject(Vec3(location.x, location.y, -1.0f));
            Vec3 farP = _camera->unproject(Vec3(location.x, location.y, 1.0f));
            Vec3 dir = farP - nearP;
            dir.normalize();
            float dist = (0.0f - nearP.y) / dir.y; // Assuming the plane is at y = 0
            Vec3 p = nearP + dir * dist;

            // Limit the target position within a certain range
            p.x = std::max(std::min(p.x, 100.0f), -100.0f);
            p.z = std::max(std::min(p.z, 100.0f), -100.0f);
            p.y = 0.0f; // Assuming the plane is at y = 0

            _targetPos = p;
        }
    }
}
