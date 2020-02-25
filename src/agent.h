#if !defined(AGENT_H)
#define AGENT_H
#include "MyECS.h"
#include "weapons.h"
#include "components.h"
namespace Escape
{
class AgentSystem : public ECSSystem
{
    static size_t index;
public:
    static Entity createAgent(World *world, const Position &pos);
    Entity createAgent(const Position &pos);

    void update(clock_type delta) override;
};
} // namespace Escape

#endif // AGENT_H
