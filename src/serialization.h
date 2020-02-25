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
#include "MyECS.h"
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
inline void serialize(Archive &ar, Escape::ID &p, const unsigned int version)
{
    ar &p.id;
}

template <typename Archive>
inline void serialize(Archive &ar, Escape::TimeServerInfo &p, const unsigned int version)
{
    ar &p.tick;
}

template <typename Archive>
inline void serialize(Archive &ar, Escape::Control &p, const unsigned int version)
{
    ar &p.player;
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

template <class Archive>
void save(Archive &ar, const entt::entity &p, unsigned int version, entt::registry &reg)
{
    int count = 0;
    reg.visit(p, [&](auto component) {
        count += 1;
    });
    ar &count;
#define DISPATCHALL() FOREACH_COMPONENT_TYPE(DISPATCH)


#define DISPATCH(type)          \
    if (reg.has<type>(p))       \
    {                           \
        ar &std::string(#type); \
        ar &reg.get<type>(p);   \
    }

    DISPATCHALL();

#undef DISPATCH
}
template <class Archive>
void load(Archive &ar, entt::entity &p, unsigned int version, entt::registry &reg)
{
    int count;
    ar &count;
    for (int i = 0; i < count; ++i)
    {
        std::string c_type;
        ar &c_type;
#define DISPATCH(type)           \
    if (c_type == #type)         \
    {                            \
        type comp;               \
        ar &comp;                \
        reg.assign<type>(p, comp); \
    }
    
    DISPATCHALL();

#undef DISPATCH
    }
}

// template <class Archive>
// void save(Archive &ar, const std::type_index &p, unsigned int version)
// {
//     ar &Escape::ComponentRegister::getInstance().getName(p);
// }
// template <class Archive>
// void load(Archive &ar, std::type_index &p, unsigned int version)
// {
//     std::string name;
//     ar &name;
//     p = Escape::ComponentRegister::getInstance().getTypeInfo(name);
// }

// template <typename Archive>
// inline void serialize(Archive &ar, std::type_index &p, const unsigned int version)
// {
//     split_free(ar, p, version);
// }

template <class Archive>
void save(Archive &ar, const entt::registry &p, unsigned int version)
{
    auto &world = const_cast<entt::registry &>(p);
    ar &world.alive();
    world.each([&](entt::entity ent) {
        save(ar, ent, version, world);
    });
}
template <class Archive>
void load(Archive &ar, entt::registry &p, unsigned int version)
{
    p.clear();
    size_t size;
    ar &size;
    for (size_t i = 0; i < size; i++)
    {
        entt::entity ent = p.create();
        load(ar, ent, version, p);
    }
}

template <typename Archive>
inline void serialize(Archive &ar, entt::registry &p, const unsigned int version)
{
    split_free(ar, p, version);
}

} // namespace serialization
} // namespace boost

#endif // SERIALIZATION_H
