#if !defined(SERIALIZATION_H)
#define SERIALIZATION_H
#include <glm/glm.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <string>
#include <type_traits>
#include "components.h"


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
} // namespace serialization
} // namespace boost
#endif // SERIALIZATION_H
