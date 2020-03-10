#include "GameScene.h"
#include "components.h"

using namespace cocos2d;
using namespace Escape;

Scene *GameScene::createScene() {
    return GameScene::create();
}

bool GameScene::init() {
    if (!Scene::init())
        return false;

    logic = new Logic();
    this->scheduleUpdate();
    return true;
}


void GameScene::menuCloseCallback(Ref *pSender) {
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();
}

GameScene::~GameScene() {
    delete logic;
}

void GameScene::update(float delta) {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    World *world = logic->world;
    this->removeAllChildren();

    world->view<AgentData, Position, Health, CircleShape>().each(
            [&](auto ent, auto &agt, auto &pos, auto &health, auto &cir) {
                auto player = newPlayer(pos.x, pos.y, cir.radius);
                if (world->has<Rotation>(ent)) {
                    Rotation rotation = world->get<Rotation>(ent);
                    player->setRotation(rotation.radian);
                }
            });
    world->view<TerrainData, Position>().each([&](auto ent, auto &ter, auto &pos) {
        auto wall = newBox(pos.x, pos.y, ter.argument_1, ter.argument_2);
        if (world->has<Rotation>(ent)) {
            Rotation rotation = world->get<Rotation>(ent);
            wall->setRotation(rotation.radian);
        }

    });
    world->view<BulletData, Position>().each([&](auto ent, auto &data, auto &pos) {
        auto pair = newBullet(pos.x, pos.y, data.radius);
        if (world->has<Rotation>(ent)) {
            auto rotation = world->get<Rotation>(ent);
        }
    });
}

cocos2d::Sprite *GameScene::newBullet(float x, float y, float r) {
    auto sprite = Sprite::create("bullet.png");
    sprite->setPosition(x, y);
    sprite->setScale(r / 16 /*px*/);
    this->addChild(sprite);
    return sprite;
}

cocos2d::Sprite *GameScene::newPlayer(float x, float y, float r) {
    auto sprite = Sprite::create("player.png");
    sprite->setPosition(x, y);
    sprite->setScale(r / 16 /*px*/);
    this->addChild(sprite);
    return sprite;
}

cocos2d::Sprite *GameScene::newBox(float x, float y, float w, float h) {
    auto sprite = Sprite::create("wall.png");
    sprite->setPosition(x, y);
    sprite->setScale(w / 32, h / 32);
    this->addChild(sprite);
    return sprite;
}
