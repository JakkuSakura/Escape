#include "GameScene.h"
#include "components.h"
#include "Settings.h"

using namespace cocos2d;
using namespace Escape;


Scene *GameScene::createScene() {
    return GameScene::create();
}

bool GameScene::init() {
    if (!Scene::init())
        return false;

    myEscape = new MyEscape(settings["map"]);

    this->scheduleUpdate();
    this->getDefaultCamera()->setPositionZ(100);
    return true;
}


void GameScene::menuCloseCallback(Ref *pSender) {
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();
}

GameScene::~GameScene() {
    delete myEscape;
}

void GameScene::update(float delta) {
    myEscape->update(delta);
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    World *world = myEscape->getWorld();
    this->removeAllChildren();
    auto player = AgentSystem::getPlayer(world, 1);
    if (player != entt::null) {
        auto pos = world->get<Position>(player);
        this->getDefaultCamera()->lookAt(Vec3(pos.x, pos.y, 0), Vec3(0, 1, 0));
        this->getDefaultCamera()->setPosition(pos.x, pos.y);
    }

    world->view<AgentData, Position, Health, CircleShape>().each(
            [&](auto ent, auto &agt, auto &pos, auto &health, auto &cir) {
                auto player = newPlayer(pos.x, pos.y, cir.radius);
                if (world->has<Rotation>(ent)) {
                    Rotation rotation = world->get<Rotation>(ent);
                    player->setRotation(rotation.radian);
                }
            });
    world->view<TerrainData, Position, RectangleShape>().each([&](auto ent, auto &ter, auto &pos, auto &rect) {
        auto wall = newTerrain(ter.type, pos.x, pos.y, rect.width, rect.height);
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
// TODO align the center of everything
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

cocos2d::Sprite *GameScene::newTerrain(TerrainType type, float x, float y, float w, float h) {
    int x1, x2, y1, y2;
    std::string file;
    // TODO optimize this part. cache
    MapConverter converter(settings["map"]);
    std::string type_;
    if(type == TerrainType::WALL)
        type_ = "Wall";
    else if(type == TerrainType::WATER)
        type_ = "Water";
    else if(type == TerrainType::BARRAIR)
        type_ = "Barrair";


    converter.coordinate(std::move(type_), file, x1, y1, x2, y2);

    auto sprite = Sprite::create(settings["map"] + "/" + file, Rect(x1, y1, x2 - x1, y2 - y1));
    sprite->setPosition(x, y);
    sprite->setScale(w / 32, h / 32);
    this->addChild(sprite);
    return sprite;
}
