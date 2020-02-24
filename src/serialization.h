#if !defined(SERIALIZATION_H)
#define SERIALIZATION_H
#include <glm/glm.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <fstream>
#include <string>
#include <type_traits>
#include "engine/MyECS.h"
#include "components.h"
#include <sstream>
namespace Escape
{
class SerializationHelper
{
public:
    std::string filename;
    SerializationHelper(const std::string &name) : filename(name)
    {
    }

    template <typename T>
    void serialize(const T &obj)
    {
        std::ofstream stream(filename);
        boost::archive::text_oarchive oa(stream);
        oa << obj;
    }

    template <typename T>
    void serialize_ptr(const T *obj)
    {
        serialize(*obj);
    }

    template <typename T>
    T &deserialize(T &obj)
    {
        std::ifstream stream(filename);
        boost::archive::text_iarchive ia(stream);
        ia >> obj;
        return obj;
    }
    template <typename T>
    T *deserialize_ptr(T *obj)
    {
        return &deserialize(*obj);
    }
};

} // namespace Escape

namespace boost
{
namespace serialization
{

template <typename Archive>
inline void serialize(Archive &ar, Escape::Name &p, const unsigned int version)
{
    ar &p.unwrap();
}

template <typename Archive>
inline void serialize(Archive &ar, Escape::Position &p, const unsigned int version)
{
    ar &p.x &p.y;
}
template <typename Archive>
inline void serialize(Archive &ar, Escape::Velocity &p, const unsigned int version)
{
    ar &p.x &p.y;
}

template <typename Archive>
inline void serialize(Archive &ar, Escape::Hitbox &p, const unsigned int version)
{
    ar &p.radius;
}

template <typename Archive>
inline void serialize(Archive &ar, Escape::Lifespan &p, const unsigned int version)
{
    ar &p.begin &p.end;
}

template <typename Archive>
inline void serialize(Archive &ar, Escape::Health &p, const unsigned int version)
{
    ar &p.health &p.max_health;
}

template <typename Archive>
inline void serialize(Archive &ar, Escape::BulletData &p, const unsigned int version)
{
    ar &p.firer_id &p.type &p.damage &p.hit;
}

template <typename Archive>
inline void serialize(Archive &ar, Escape::WeaponPrototype &p, const unsigned int version)
{
    ar &p.type;
    ar &p.bullet_type;
    ar &p.cd;
    ar &p.accuracy;
    ar &p.peice_number;
    ar &p.bullet_damage;
    ar &p.bullet_speed;
}
template <typename Archive>
inline void serialize(Archive &ar, Escape::Weapon &p, const unsigned int version)
{
    ar &p.weapon;
    ar &p.last;
    ar &p.next;
}
template <typename T>
struct ComponentContainerHacked : public ECS::Internal::BaseComponentContainer
{
public:
    ComponentContainerHacked() {}
    ComponentContainerHacked(const T &data) : data(data) {}

    T data;

    virtual void destroy(ECS::World *world)
    {
    }

    virtual void removed(ECS::Entity *ent)
    {
    }
};
class EntityHacked
{
public:
    typedef std::unordered_map<ECS::TypeIndex, ECS::Internal::BaseComponentContainer *> map;
    typedef std::pair<std::type_index, ECS::Internal::BaseComponentContainer *> pair;
    map components;
    ECS::World *world;

    size_t id;
    bool bPendingDestroy = false;
};
template <typename Archive>
inline void serialize(Archive &ar, EntityHacked::pair &p, const unsigned int version)
{

    split_free(ar, p, version);
}

template <class Archive>
void save(Archive &ar, const EntityHacked::pair &pair, unsigned int version)
{
    ar &pair.first;
#define DISPATCH(type)                                                           \
    if (typeid(*pair.second) == typeid(ECS::Internal::ComponentContainer<type>)) \
    ar &reinterpret_cast<ComponentContainerHacked<type> *>(pair.second)->data

    DISPATCH(Escape::Position);
    else DISPATCH(Escape::Name);
    else DISPATCH(Escape::Velocity);
    else DISPATCH(Escape::Health);
    else DISPATCH(Escape::Weapon);
    else DISPATCH(Escape::WeaponPrototype);
    else DISPATCH(Escape::Hitbox);
    else DISPATCH(Escape::BulletData);
    else DISPATCH(Escape::Lifespan);

#undef DISPATCH
}

template <class Archive>
void load(Archive &ar, EntityHacked::pair &pair, unsigned int version)
{
    ar &pair.first;
#define DISPATCH(type)                                           \
    if (pair.first == ECS::TypeIndex(typeid(type)))              \
                                                                 \
    pair.second = new ECS::Internal::ComponentContainer<type>(), \
    ar &reinterpret_cast<ComponentContainerHacked<type> *>(pair.second)->data

    DISPATCH(Escape::Position);
    else DISPATCH(Escape::Name);
    else DISPATCH(Escape::Velocity);
    else DISPATCH(Escape::Health);
    else DISPATCH(Escape::Weapon);
    else DISPATCH(Escape::WeaponPrototype);
    else DISPATCH(Escape::Hitbox);
    else DISPATCH(Escape::BulletData);
    else DISPATCH(Escape::Lifespan);
    else throw std::runtime_error("Cannot read");
#undef DISPATCH
}

template <typename Archive>
inline void serialize(Archive &ar, EntityHacked &p, const unsigned int version)
{

    split_free(ar, p, version);
}

template <class Archive>
void save(Archive &ar, const EntityHacked &p, unsigned int version)
{
    ar &p.id;
    ar &p.bPendingDestroy;
    ar &p.components.size();
    for (const EntityHacked::pair &pair : p.components)
    {
        ar &pair;
    }
}
template <class Archive>
void load(Archive &ar, EntityHacked &p, unsigned int version)
{
    ar &p.id;
    ar &p.bPendingDestroy;
    size_t size;
    ar &size;
    for (size_t i = 0; i < size; i++)
    {
        auto pair = std::make_pair<std::type_index, ECS::Internal::BaseComponentContainer *>(std::type_index(typeid(0)), 0);
        ar &pair;
        p.components.insert(pair);
    }
}
template <typename Archive>
inline void serialize(Archive &ar, ECS::Entity &p, const unsigned int version)
{
    EntityHacked *hacked = (EntityHacked *)&p;
    ar &*hacked;
}

template <class Archive>
void save(Archive &ar, const ECS::TypeIndex &p, unsigned int version)
{
    ar &Escape::ComponentRegister::getInstance().getName(p);
}
template <class Archive>
void load(Archive &ar, ECS::TypeIndex &p, unsigned int version)
{
    std::string name;
    ar &name;
    p = Escape::ComponentRegister::getInstance().getTypeInfo(name);
}

template <typename Archive>
inline void serialize(Archive &ar, ECS::TypeIndex &p, const unsigned int version)
{
    split_free(ar, p, version);
}

template <class Archive>
void save(Archive &ar, const ECS::World &p, unsigned int version)
{
    ECS::World &world = const_cast<ECS::World &>(p);
    ar & world.getCount();
    world.all([&](ECS::Entity *ent)
    {
        ar & *ent;
    });

}
template <class Archive>
void load(Archive &ar, ECS::World &p, unsigned int version)
{
    p.reset();
    size_t size;
    ar & size;
    for (size_t i = 0; i < size; i++)
    {
        ECS::Entity *ent = p.create();
        ar & *ent;
    }
}

template <typename Archive>
inline void serialize(Archive &ar, ECS::World &p, const unsigned int version)
{
    split_free(ar, p, version);
}

} // namespace serialization
} // namespace boost

#endif // SERIALIZATION_H
