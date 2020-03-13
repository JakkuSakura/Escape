//
// Created by jack on 20-2-25.
//

#ifndef ESCAPE_LOGIC_H
#define ESCAPE_LOGIC_H

#include "MyECS.h"
#include <map>
#include <string>

namespace Escape {
    class Logic : public SystemManager {
        void addSystems();

    public:
        using SystemManager::world;

        Logic();

        Logic(World *world);

        // Take over the ownership of world
        void setWorld(World *world);
    };

}

#endif //ESCAPE_LOGIC_H
