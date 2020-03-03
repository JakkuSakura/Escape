//
// Created by jack on 20-2-29.
//

#ifndef ESCAPE_AI_H
#define ESCAPE_AI_H

#include <map>
#include "MyECS.h"

namespace Escape {
    class AgentControl;

    class AISystem : public ECSSystem {
        std::map<entt::entity, AgentControl *> AIs;
    public:
        AISystem();

        ~AISystem();

        void initialize() override;

        void update(float delta) override;

        entt::entity allocate();

        void insert(entt::entity ent, AgentControl *agt);
    };

}


#endif //ESCAPE_AI_H
