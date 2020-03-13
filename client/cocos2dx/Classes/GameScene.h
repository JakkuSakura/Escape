#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include "cocos2d.h"
#include "logic.h"
#include "my_escape.h"

class InputState
{
public:
    static const int MAX_KEY_NUM = 4096;
    bool keys[MAX_KEY_NUM] = {0};
    float mouse_x = 0, mouse_y = 0;
    bool mouse[4]={0};
};

class GameScene : public cocos2d::Scene {
public:
    InputState input_state;
    Escape::MyEscape *myEscape;
    static cocos2d::Scene *createScene();
    virtual bool init();
    float zoomin_ratio = 10;
    void menuCloseCallback(cocos2d::Ref *pSender);
    void update(float delta) override;
    virtual ~GameScene() override;

    CREATE_FUNC(GameScene);

    Position pickUp(float x, float y);

private:
    cocos2d::Sprite *newPlayer(float x, float y, float r);
    cocos2d::Sprite *newTerrain(TerrainType type, float x, float y, float w, float h);

    cocos2d::Sprite *newBullet(float x, float y, float r);

};

#endif // GAME_SCENE_H
