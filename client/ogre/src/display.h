//
// Created by jack on 20-2-25.
//

#ifndef ESCAPE_DISPLAY_H
#define ESCAPE_DISPLAY_H

#include "window_ogre.h"
#include <OgreRenderSystem.h>
#include "MyECS.h"
#include "engine/utils.h"
#include "control.h"
#include "agent.h"

namespace Escape {

    class DisplayOgre : public WindowOgre {
        World *world;
        TimeServer *timeserver;
        Ogre::SceneNode *rects;

    public:
        DisplayOgre();

        ~DisplayOgre() = default;

        std::pair<Ogre::SceneNode *, Ogre::Entity *> newBox(float cx, float cy, float width, float height);

        std::pair<Ogre::SceneNode *, Ogre::Entity *> newCircle(float cx, float cy, float radius);

        Ogre::Vector3 pickUp(unsigned int absoluteX, unsigned int absoluteY);

        void initialize() override;

        virtual void processInput() override;

        void render() override;
        void setCenter(float x, float y);

        virtual void postProcess() override;

        void windowResized(int width, int height) override;
    };
} // namespace Escape



#endif //ESCAPE_DISPLAY_H
