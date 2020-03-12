#include "GameScene.h"
#include "components.h"
#include "Settings.h"

using namespace cocos2d;
using namespace Escape;


class ClientController : public Escape::Controller {
    InputState *input;
    ControlSystem *controlSystem;
    int player_id;
    GameScene *display;
public:
    ClientController(GameScene *display_, InputState *input, int player_id_) : input(input),
                                                                               player_id(player_id_),
                                                                               display(display_) {

    }

    void init(ControlSystem *msg) override {
        this->controlSystem = msg;
    }

    void update(float delta) override {
        entt::entity player = controlSystem->findPlayer(player_id);
        if (player == entt::null)
            return;
        auto[pos, agt] = controlSystem->get<Position, AgentData>(player);
        if (input->mouse[(int) EventMouse::MouseButton::BUTTON_LEFT]) {
            auto click = display->pickUp(input->mouse_x, input->mouse_y);
            double x = click.x, y = click.y;
            std::cerr << "Cursor: " << x << " " << y << std::endl;
            std::cerr << "Player: " << pos.x << " " << pos.y << std::endl;
            float angle = atan2(y - pos.y, x - pos.x);

            controlSystem->dispatch(player, Shooting(angle));
        }

        Velocity vel(0, 0);
        if (input->keys[(int) EventKeyboard::KeyCode::KEY_W])
            vel.y += 1;
        if (input->keys[(int) EventKeyboard::KeyCode::KEY_S])
            vel.y += -1;
        if (input->keys[(int) EventKeyboard::KeyCode::KEY_A])
            vel.x += -1;
        if (input->keys[(int) EventKeyboard::KeyCode::KEY_D])
            vel.x += 1;
        float spd = glm::length(as<glm::vec2>(vel));
        if (spd > 0) {
            if (spd > 1) {
                vel /= spd;
            }
            vel *= AGENT_IMPULSE;
            controlSystem->dispatch<Impulse>(player, Impulse(vel.x, vel.y));
        }

        if (input->keys[(int) EventKeyboard::KeyCode::KEY_1])
            controlSystem->dispatch(player, ChangeWeapon(WeaponType::HANDGUN));

        if (input->keys[(int) EventKeyboard::KeyCode::KEY_2])
            controlSystem->dispatch(player, ChangeWeapon(WeaponType::SHOTGUN));

        if (input->keys[(int) EventKeyboard::KeyCode::KEY_3])
            controlSystem->dispatch(player, ChangeWeapon(WeaponType::SMG));

        if (input->keys[(int) EventKeyboard::KeyCode::KEY_4])
            controlSystem->dispatch(player, ChangeWeapon(WeaponType::SNIPER_RIFLE));

        if (input->keys[(int) EventKeyboard::KeyCode::KEY_5])
            controlSystem->dispatch(player, ChangeWeapon(WeaponType::FLAME_THROWER));
    }
};

Scene *GameScene::createScene() {
    return GameScene::create();
}

bool GameScene::init() {
    if (!Scene::init())
        return false;

    myEscape = new MyEscape(settings["map"]);
    this->getDefaultCamera()->setPositionZ(60);
    this->scheduleUpdate();

    {
        auto listener = EventListenerKeyboard::create();
        listener->onKeyPressed = [&](EventKeyboard::KeyCode keyCode, cocos2d::Event *event) {
            if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
                menuCloseCallback(nullptr);
            if ((int) keyCode < InputState::MAX_KEY_NUM)
                input_state.keys[(int) keyCode] = 1;
        };
        listener->onKeyReleased = [&](EventKeyboard::KeyCode keyCode, cocos2d::Event *event) {
            if ((int) keyCode < InputState::MAX_KEY_NUM)
                input_state.keys[(int) keyCode] = 0;
        };

        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    }

    {
        auto listener = EventListenerMouse::create();
        listener->onMouseDown = [&](cocos2d::Event *event) {
            EventMouse *mouseEvent = dynamic_cast<EventMouse *>(event);
            input_state.mouse[(int) mouseEvent->getMouseButton()] = true;
            input_state.mouse_x = mouseEvent->getCursorX();
            input_state.mouse_y = mouseEvent->getCursorY();
        };

        listener->onMouseMove = [&](cocos2d::Event *event) {
            EventMouse *mouseEvent = dynamic_cast<EventMouse *>(event);
            input_state.mouse_x = mouseEvent->getCursorX();
            input_state.mouse_y = mouseEvent->getCursorY();
        };


        listener->onMouseUp = [&](cocos2d::Event *event) {
            EventMouse *mouseEvent = dynamic_cast<EventMouse *>(event);
            input_state.mouse[(int) mouseEvent->getMouseButton()] = false;
            input_state.mouse_x = mouseEvent->getCursorX();
            input_state.mouse_y = mouseEvent->getCursorY();
        };

        _eventDispatcher->addEventListenerWithFixedPriority(listener, 1);

    }
    myEscape->findSystem<ControlSystem>()->addController(new ClientController(this, &input_state, 1));
    myEscape->foreach([](System *sys) {
        sys->initialize();
    });
    return true;
}

// TODO get world coordinates correctly
Position GameScene::pickUp(float x, float y) {
    auto size = Director::getInstance()->getWinSize();
    auto camera = getDefaultCamera();
    auto pos = camera->unprojectGL(Vec3(x, y, 0));
    return Position(pos.x, pos.y);
}

void GameScene::menuCloseCallback(Ref *pSender) {
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();
}

GameScene::~GameScene() {
    delete myEscape;
}

void GameScene::update(float delta) {
//    std::cerr << "Update " << delta << std::endl;
    myEscape->updateAll(delta);
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    World *world = myEscape->getWorld();
    this->removeAllChildren();
    auto player = AgentSystem::getPlayer(world, 1);
    if (player != entt::null) {
        auto pos = world->get<Position>(player);
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
    myEscape->mapConverter.coordinate((int) type, x1, y1, x2, y2);
    auto sprite = Sprite::create(myEscape->mapConverter.tileset, Rect(x1, y1, x2 - x1, y2 - y1));
    sprite->setPosition(x, y);
    sprite->setScale(w / 32, h / 32);
    this->addChild(sprite);
    return sprite;
}
