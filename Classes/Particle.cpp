#include"Particle.h"

/*auto emitter = ParticleSystemQuad::create("Send.plist");
    if (emitter) {
        // ����������ڵ㲢ʹ��������������ʼ��
        auto batchNode = ParticleBatchNode::createWithTexture(emitter->getTexture());
        Size visibleSize = Director::getInstance()->getVisibleSize();
        emitter->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
        // ������ϵͳ��ӵ�������ڵ���
        batchNode->addChild(particleSystem);
        emitter->setDuration(-1);
        // ��������ڵ���ӵ����ĳ����������ڵ���
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
        // ����������ڵ㲢ʹ��������������ʼ��
        auto batchNode = ParticleBatchNode::createWithTexture(emitter->getTexture());

        emitter->setPosition(Vec2(x,y));

        // ������ϵͳ��ӵ�������ڵ���
        batchNode->addChild(emitter);
        emitter->setDuration(-1);

        // ��������ڵ���ӵ����ĳ����������ڵ���
        addChild(batchNode);
    }
}
void particleExplosion::onenter(int x, int y)
{
    auto emitter = ParticleSystemQuad::create("explosion.plist");
    if (emitter) {
        // ����������ڵ㲢ʹ��������������ʼ��
        auto batchNode = ParticleBatchNode::createWithTexture(emitter->getTexture());

        emitter->setPosition(Vec2(x, y));

        // ������ϵͳ��ӵ�������ڵ���
        batchNode->addChild(emitter);
        emitter->setDuration(-1);

        // ��������ڵ���ӵ����ĳ����������ڵ���
        addChild(batchNode);
    }
}

void particleFire::onenter(int x, int y)
{
    auto emitter = ParticleSystemQuad::create("fire.plist");
    if (emitter) {
        // ����������ڵ㲢ʹ��������������ʼ��
        auto batchNode = ParticleBatchNode::createWithTexture(emitter->getTexture());

        emitter->setPosition(Vec2(x, y));

        // ������ϵͳ��ӵ�������ڵ���
        batchNode->addChild(emitter);
        emitter->setDuration(-1);

        // ��������ڵ���ӵ����ĳ����������ڵ���
        addChild(batchNode);
    }
}

void particleMetorRain::onenter(int x, int y)
{
    auto emitter = ParticleSystemQuad::create("MetorRain.plist");
    if (emitter) {
        // ����������ڵ㲢ʹ��������������ʼ��
        auto batchNode = ParticleBatchNode::createWithTexture(emitter->getTexture());

        emitter->setPosition(Vec2(x, y));

        // ������ϵͳ��ӵ�������ڵ���
        batchNode->addChild(emitter);
        emitter->setDuration(-1);

        // ��������ڵ���ӵ����ĳ����������ڵ���
        addChild(batchNode);
    }
}

void particleNormalAttack::onenter(int x, int y)
{
    auto emitter = ParticleSystemQuad::create("normalAttack.plist");
    if (emitter) {
        // ����������ڵ㲢ʹ��������������ʼ��
        auto batchNode = ParticleBatchNode::createWithTexture(emitter->getTexture());

        emitter->setPosition(Vec2(x, y));

        // ������ϵͳ��ӵ�������ڵ���
        batchNode->addChild(emitter);
        emitter->setDuration(-1);

        // ��������ڵ���ӵ����ĳ����������ڵ���
        addChild(batchNode);
    }
}

void particleSend::onenter(int x, int y)
{
    auto emitter = ParticleSystemQuad::create("Send.plist");
    if (emitter) {
        // ����������ڵ㲢ʹ��������������ʼ��
        auto batchNode = ParticleBatchNode::createWithTexture(emitter->getTexture());

        emitter->setPosition(Vec2(x, y));

        // ������ϵͳ��ӵ�������ڵ���
        batchNode->addChild(emitter);
        emitter->setDuration(-1);

        // ��������ڵ���ӵ����ĳ����������ڵ���
        addChild(batchNode);
    }
}