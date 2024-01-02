#include"Particle.h"

/*auto emitter = ParticleSystemQuad::create("Send.plist");
    if (emitter) {
        // 创建批处理节点并使用粒子纹理来初始化
        auto batchNode = ParticleBatchNode::createWithTexture(emitter->getTexture());
        Size visibleSize = Director::getInstance()->getVisibleSize();
        emitter->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
        // 将粒子系统添加到批处理节点中
        batchNode->addChild(particleSystem);
        emitter->setDuration(-1);
        // 将批处理节点添加到您的场景或其他节点中
        addChild(batchNode);
    }
*/

particle::~particle()
{
    CC_SAFE_RELEASE(emitter);
}

void particle::onenter(int x, int y)
{
    emitter = nullptr;
}

void particleBegain::onenter(int x, int y)
{
    auto emitter = ParticleSystemQuad::create("Begain.plist");
    if (emitter) {
        // 创建批处理节点并使用粒子纹理来初始化
        auto batchNode = ParticleBatchNode::createWithTexture(emitter->getTexture());

        emitter->setPosition(Vec2(x,y));

        // 将粒子系统添加到批处理节点中
        batchNode->addChild(emitter);
        emitter->setDuration(-1);

        // 将批处理节点添加到您的场景或其他节点中
        addChild(batchNode);
    }
}
void particleExplosion::onenter(int x, int y)
{
    auto emitter = ParticleSystemQuad::create("explosion.plist");
    if (emitter) {
        // 创建批处理节点并使用粒子纹理来初始化
        auto batchNode = ParticleBatchNode::createWithTexture(emitter->getTexture());

        emitter->setPosition(Vec2(x, y));

        // 将粒子系统添加到批处理节点中
        batchNode->addChild(emitter);
        emitter->setDuration(-1);

        // 将批处理节点添加到您的场景或其他节点中
        addChild(batchNode);
    }
}

void particleFire::onenter(int x, int y)
{
    auto emitter = ParticleSystemQuad::create("fire.plist");
    if (emitter) {
        // 创建批处理节点并使用粒子纹理来初始化
        auto batchNode = ParticleBatchNode::createWithTexture(emitter->getTexture());

        emitter->setPosition(Vec2(x, y));

        // 将粒子系统添加到批处理节点中
        batchNode->addChild(emitter);
        emitter->setDuration(-1);

        // 将批处理节点添加到您的场景或其他节点中
        addChild(batchNode);
    }
}

void particleMetorRain::onenter(int x, int y)
{
    auto emitter = ParticleSystemQuad::create("MetorRain.plist");
    if (emitter) {
        // 创建批处理节点并使用粒子纹理来初始化
        auto batchNode = ParticleBatchNode::createWithTexture(emitter->getTexture());

        emitter->setPosition(Vec2(x, y));

        // 将粒子系统添加到批处理节点中
        batchNode->addChild(emitter);
        emitter->setDuration(-1);

        // 将批处理节点添加到您的场景或其他节点中
        addChild(batchNode);
    }
}

void particleNormalAttack::onenter(int x, int y)
{
    auto emitter = ParticleSystemQuad::create("normalAttack.plist");
    if (emitter) {
        // 创建批处理节点并使用粒子纹理来初始化
        auto batchNode = ParticleBatchNode::createWithTexture(emitter->getTexture());

        emitter->setPosition(Vec2(x, y));

        // 将粒子系统添加到批处理节点中
        batchNode->addChild(emitter);
        emitter->setDuration(-1);

        // 将批处理节点添加到您的场景或其他节点中
        addChild(batchNode);
    }
}

void particleSend::onenter(int x, int y)
{
    auto emitter = ParticleSystemQuad::create("Send.plist");
    if (emitter) {
        // 创建批处理节点并使用粒子纹理来初始化
        auto batchNode = ParticleBatchNode::createWithTexture(emitter->getTexture());

        emitter->setPosition(Vec2(x, y));

        // 将粒子系统添加到批处理节点中
        batchNode->addChild(emitter);
        emitter->setDuration(-1);

        // 将批处理节点添加到您的场景或其他节点中
        addChild(batchNode);
    }
}