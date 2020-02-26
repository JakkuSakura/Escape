//
// Created by jack on 20-2-25.
//

#ifndef ESCAPE_LOGIC_H
#define ESCAPE_LOGIC_H

#include "MyECS.h"

namespace Escape {
    class Logic : public SystemManager {
    public:
        using SystemManager::world;

        void initialize() override;

        Logic();

    };

}

#endif //ESCAPE_LOGIC_H
