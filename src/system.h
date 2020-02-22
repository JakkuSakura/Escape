#if !defined(SYSTEM_H)
#define SYSTEM_H
#include <vector>
#include <cassert>
#include <functional>
namespace Escape
{
/**
 * This system supports subsystems
 * 
 */
class System
{
protected:
    std::vector<System *> subsystems;
    System *parent;

public:
    virtual void configure()
    {

    }
    virtual void unconfigure()
    {

    }
    virtual void update(float delta)
    {
    }
    void updateAll(float delta)
    {
        foreach ([&](System *sys) { sys->update(delta); });
    }
    System(System *parent = nullptr) : parent(parent)
    {
    }
    virtual void initialize()
    {
    }
    template <class T>
    T *findSystem()
    {
        System *root = this;
        while (root->parent != nullptr)
            root = root->parent;
        T *objective = nullptr;
        root->foreach ([&](System *sys) {
            T *casted = dynamic_cast<T *>(sys);
            if (casted != nullptr)
                objective = casted;
        });
        return objective;
    }
    void foreach (std::function<void(System *)> func)
    {
        func(this);
        for (auto &&sub : subsystems)
        {
            sub->foreach (func);
        }
    }

    const std::vector<System *> &getSubSystems() const
    {
        return subsystems;
    }

    System *getParent() const
    {
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
        sub->configure();
    }
    bool removeSubSystem(System *sub)
    {
        assert(sub->parent != nullptr);
        for (size_t i = 0; i < subsystems.size(); i++)
        {
            if(subsystems[i] == sub)
            {
                subsystems.erase(subsystems.begin() + i);
                sub->unconfigure();
                sub->parent = nullptr;
                return true;
            }
        }
        return false;
    }
};
} // namespace Escape

#endif // SYSTEM_H
