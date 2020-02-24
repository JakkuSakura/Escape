#if !defined(SERIALIZATION_H)
#define SERIALIZATION_H
#include <glm/glm.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/unordered_map.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <string>
#include <type_traits>
#include "engine/MyECS.h"
#include "components.h"
#include <sstream>
namespace Escape
{
class SerializationHelper
{
    boost::archive::text_oarchive oa;
    boost::archive::text_iarchive ia;

public:
    std::stringstream stream;
    SerializationHelper() : oa(stream), ia(stream)
    {
    }
    template <typename T>
    void serialize(const T &obj)
    {
        oa << obj;
    }
    template <typename T>
    T deserialize()
    {
        T obj;
        ia >> obj;
        return obj;
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
class EntityHacked
{
public:
    std::unordered_map<ECS::TypeIndex, ECS::Internal::BaseComponentContainer *> components;
    ECS::World *world;

    size_t id;
    bool bPendingDestroy = false;
};

template <typename Archive>
inline void serialize(Archive &ar, ECS::Entity &p, const unsigned int version)
{
    EntityHacked *hacked = (EntityHacked *)&p;
    ar & hacked->id;
    ar & hacked->bPendingDestroy;
    ar & hacked->components;
}

template <typename Archive>
inline void serialize(Archive &ar, ECS::TypeIndex &p, const unsigned int version)
{
    // ar & p->;
}

} // namespace serialization
} // namespace boost
#endif // SERIALIZATION_H
