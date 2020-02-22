#if !defined(SYSTEM_H)
#define SYSTEM_H
#include <vector>
#include <cassert>
namespace Escape
{
class System
{
protected:
    std::vector<System *> subsystems;
    System *parent;

public:
    System(System *parent = nullptr) : parent(parent)
    {
    }
    System *getParent() const {
        return parent;
    }
    virtual ~System()
    {
        for (auto &&i : subsystems)
        {
            delete i;
        }
    }
    void addSubSystem(System *sub)
    {
        assert(sub->parent == nullptr);
        sub->parent = this;
        subsystems.push_back(sub);
    }
    virtual void update(float delta)
    {
        updateSubSystems(delta);
    }
    void updateSubSystems(float delta)
    {
        for (auto &&sub : subsystems)
        {
            sub->update(delta);
        }
    }
};
} // namespace Escape

#endif // SYSTEM_H
