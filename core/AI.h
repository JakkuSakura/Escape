//
// Created by jack on 20-2-29.
//

#ifndef ESCAPE_AI_H
#define ESCAPE_AI_H

#include "MyECS.h"
namespace Escape{
class AISystem : public System {
public:
    AISystem();
    void initialize() override;
};

}



#endif //ESCAPE_AI_H
