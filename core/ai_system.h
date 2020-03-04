//
// Created by jack on 20-2-29.
//

#ifndef ESCAPE_AI_SYSTEM_H
#define ESCAPE_AI_SYSTEM_H

#include <map>
#include "MyECS.h"
#include "control.h"

namespace Escape {

    class AISystem;

    class AgentControl : public Controller {
        friend AISystem;
        entt::entity id = entt::null;

        void setEntityID(entt::entity id_) {
            this->id = id_;
        }

    public:
        ControlSystem *control;

        void init(ControlSystem *c) override {
            control = c;
        }

        entt::entity getEntityID() {
            return id;
        }
    };

    class AISystem : public ECSSystem {
        std::map<entt::entity, AgentControl *> AIs;
    public:
        AISystem();

        ~AISystem();

        void update(float delta) override;

        entt::entity allocate();

        void insert(entt::entity ent, AgentControl *agt);
    };

}


#endif //ESCAPE_AI_SYSTEM_H
