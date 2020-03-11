#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include "cocos2d.h"
#include "logic.h"
#include "my_escape.h"
class GameScene : public cocos2d::Scene {
public:
    Escape::MyEscape *myEscape;
    static cocos2d::Scene *createScene();
    virtual bool init();

    void menuCloseCallback(cocos2d::Ref *pSender);
    void update(float delta) override;
    virtual ~GameScene() override;
    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);
private:
    cocos2d::Sprite *newPlayer(float x, float y, float r);
    cocos2d::Sprite *newTerrain(TerrainType type, float x, float y, float w, float h);

    cocos2d::Sprite *newBullet(float x, float y, float r);
};

#endif // GAME_SCENE_H
