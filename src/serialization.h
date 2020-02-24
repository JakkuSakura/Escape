#if !defined(SERIALIZATION_H)
#define SERIALIZATION_H
#include <glm/glm.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/iostreams/stream.hpp>
// #include <boost/iostreams/>
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
    static const int buffer_size = 4096;
    char buffer[buffer_size] = {0};

    SerializationHelper()
    {
    }
    template <typename T>
    void serialize(const T &obj)
    {
        boost::iostreams::basic_array_sink<char> sr(buffer, buffer_size);
        boost::iostreams::stream<boost::iostreams::basic_array_sink<char>> source(sr);
        boost::archive::text_oarchive oa(source);
        oa << obj;
    }

    template <typename T>
    void serialize(const T *obj)
    {
        serialize(*obj);
    }
    template <typename T>
    T &deserialize(T &obj)
    {
        std::stringstream stream(buffer);
        boost::archive::text_iarchive ia(stream);
        ia >> obj;
        return obj;
    }
    template <typename T>
    T *deserialize(T *obj)
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
    typedef std::pair<const std::type_index, ECS::Internal::BaseComponentContainer *> pair;
    map components;
    ECS::World *world;

    size_t id;
    bool bPendingDestroy = false;
};
template <typename Archive>
inline void serialize(Archive &ar, ECS::Internal::BaseComponentContainer &p, const unsigned int version)
{

#define DISPATCH(type)                                                \
    if (typeid(p) == typeid(ECS::Internal::ComponentContainer<type>)) \
    ar &reinterpret_cast<ComponentContainerHacked<type> *>(&p)->data

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

template <typename Archive>
inline void serialize(Archive &ar, EntityHacked::map &p, const unsigned int version)
{
    TRACE();
    split_free(ar, p, version);
    TRACE();
}

template <class Archive>
void save(Archive &ar, const EntityHacked::map &p, unsigned int version)
{
    TRACE();
    ar &p.size();
    for (const EntityHacked::pair &pair : p)
    {
        TRACE();
        ar &pair.first;
        TRACE();
        ar &*pair.second;
        TRACE();
    }
    TRACE();
}
template <class Archive>
void load(Archive &ar, EntityHacked::map &p, unsigned int version)
{
    TRACE();
    size_t size;
    ar &size;
    std::cerr << "size: " << size << std::endl;
    TRACE();
    for (size_t i = 0; i < size; i++)
    {
        TRACE();
        ECS::TypeIndex id(typeid(void *));
        ar &id;
#define FORWARD(type)
        if (id == ECS::TypeIndex(typeid(Escape::Position)))
        {
            ECS::Internal::BaseComponentContainer *ptr = new ECS::Internal::ComponentContainer<Escape::Position>();
            ar &*ptr;
            p.insert(std::make_pair(id, ptr));
            TRACE();
        }
        else
        {
            throw new std::runtime_error("Cannot read");
        }
    }

    TRACE();
}
template <typename Archive>
inline void serialize(Archive &ar, ECS::Entity &p, const unsigned int version)
{
    TRACE();
    EntityHacked *hacked = (EntityHacked *)&p;
    ar & hacked->id;
    std::cerr << "Id" << hacked->id << std::endl;
    TRACE();
    ar & hacked->bPendingDestroy;
    std::cerr << "PendingDestory" << hacked->bPendingDestroy << std::endl;
    TRACE();
    ar & hacked->components;
    TRACE();
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
    std::cerr << "Get name " << name << std::endl;
    p = Escape::ComponentRegister::getInstance().getTypeInfo(name);
}

template <typename Archive>
inline void serialize(Archive &ar, ECS::TypeIndex &p, const unsigned int version)
{
    split_free(ar, p, version);
}

} // namespace serialization
} // namespace boost

#endif // SERIALIZATION_H
