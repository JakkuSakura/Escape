#pragma clang diagnostic push
#pragma ide diagnostic ignored "hicpp-explicit-conversions"
#if !defined(COMPONENTS_H)
#define COMPONENTS_H

#include "engine/utils.h"
#include "MyECS.h"
#include <glm/glm.hpp>
#include <boost/serialization/nvp.hpp>

namespace Escape {
    struct Name : public std::string {
        FORWARD_CONSTRUCTORS(Name, std::string);

        template<class Archive>
        void serialize(Archive &ar, const unsigned int version) {
            auto &name = unwrap();
            ar & BOOST_SERIALIZATION_NVP(name);
        }
    };

    struct Position : public glm::vec2 {
        FORWARD_CONSTRUCTORS(Position, glm::vec2);

        template<class Archive>
        void serialize(Archive &ar, const unsigned int version) {
            ar & BOOST_SERIALIZATION_NVP(x);
            ar & BOOST_SERIALIZATION_NVP(y);
        }
    };

    struct Velocity : public glm::vec2 {
        FORWARD_CONSTRUCTORS(Velocity, glm::vec2);

        template<class Archive>
        void serialize(Archive &ar, const unsigned int version) {
            ar & BOOST_SERIALIZATION_NVP(x);
            ar & BOOST_SERIALIZATION_NVP(y);
        }
    };

    struct Impulse : public glm::vec2 {
        FORWARD_CONSTRUCTORS(Impulse, glm::vec2);

        template<class Archive>
        void serialize(Archive &ar, const unsigned int version) {
            ar & BOOST_SERIALIZATION_NVP(x);
            ar & BOOST_SERIALIZATION_NVP(y);
        }
    };

    struct Collision {
        entt::entity hit_with;
    };
    struct CollisionResults {
        std::vector<Collision> results;
    };


    struct Hitbox {
        float radius;

        template<class Archive>
        void serialize(Archive &ar, const unsigned int version) {
            ar & BOOST_SERIALIZATION_NVP(radius);
        }
    };

    struct Rotation {
        float radian;

        template<class Archive>
        void serialize(Archive &ar, const unsigned int version) {
            ar & BOOST_SERIALIZATION_NVP(radian);
        }
    };

    struct AgentData {
        int id;
        int player;

        template<class Archive>
        void serialize(Archive &ar, const unsigned int version) {
            ar & BOOST_SERIALIZATION_NVP(id);
            ar & BOOST_SERIALIZATION_NVP(player);
        }
    };

    struct TimeServerInfo {
        size_t tick;

        template<class Archive>
        void serialize(Archive &ar, const unsigned int version) {
            ar & BOOST_SERIALIZATION_NVP(tick);
        }
    };

    struct Lifespan {
        clock_type begin;
        clock_type end;

        template<typename Archive>
        void serialize(Archive &ar, const unsigned int version) {
            ar & BOOST_SERIALIZATION_NVP(begin);
            ar & BOOST_SERIALIZATION_NVP(end);
        }

    };

    struct Health {
        float health;
        float max_health;

        Health(const Health &h) = default;

        Health(float h = 100) {
            health = h;
            max_health = h;
        };

        template<typename Archive>
        void serialize(Archive &ar, const unsigned int version) {
            ar & BOOST_SERIALIZATION_NVP(health);
            ar & BOOST_SERIALIZATION_NVP(max_health);
        }
    };

    enum class BulletType {
        HANDGUN_BULLET,
        SHOTGUN_SHELL,
        SMG_BULLET,
        RIFLE_BULLET
    };

    struct BulletData {
        int firer_id;
        BulletType type;
        float damage;
        float density;
        float radius;

        template<typename Archive>
        void serialize(Archive &ar, const unsigned int version) {
            ar & BOOST_SERIALIZATION_NVP(firer_id);
            ar & BOOST_SERIALIZATION_NVP(type);
            ar & BOOST_SERIALIZATION_NVP(damage);
            ar & BOOST_SERIALIZATION_NVP(density);
            ar & BOOST_SERIALIZATION_NVP(radius);
        }

    };

    enum class WeaponType {
        HANDGUN,
        SHOTGUN,
        SMG,
        RIFLE
    };

    struct WeaponPrototype {
        WeaponType type;
        BulletType bullet_type;
        float cd;
        float accuracy;
        float bullet_number;
        float bullet_damage;
        float bullet_speed;
        float gun_length;

        template<typename Archive>
        void serialize(Archive &ar, const unsigned int version) {
            ar & BOOST_SERIALIZATION_NVP(type);
            ar & BOOST_SERIALIZATION_NVP(bullet_type);
            ar & BOOST_SERIALIZATION_NVP(cd);
            ar & BOOST_SERIALIZATION_NVP(accuracy);
            ar & BOOST_SERIALIZATION_NVP(bullet_number);
            ar & BOOST_SERIALIZATION_NVP(bullet_damage);
            ar & BOOST_SERIALIZATION_NVP(bullet_speed);
            ar & BOOST_SERIALIZATION_NVP(gun_length);
        }
    };

    struct Weapon {
        WeaponType weapon;
        float last;
        float next;

        template<typename Archive>
        void serialize(Archive &ar, const unsigned int version) {
            ar & BOOST_SERIALIZATION_NVP(weapon);
            ar & BOOST_SERIALIZATION_NVP(last);
            ar & BOOST_SERIALIZATION_NVP(next);
        }
    };

    enum class TerrainType {
        BOX,
        CIRCLE
    };

    struct TerrainData {
        TerrainType type;
        float arguments[16];

        template<class Archive>
        void serialize(Archive &ar, const unsigned int version) {
            ar & BOOST_SERIALIZATION_NVP(type);
            ar & BOOST_SERIALIZATION_NVP(arguments);
        }

    };

    // Controls
    // These shouldn't be saved nor assigned as component at all
    template<typename T>
    struct Control {
        Control(int agent_id, const T &t) : agent_id(agent_id), data(t), processed(false) {}

        int agent_id;
        bool processed;
        T data;
    };

    struct Shooting {
        int agent_id;
        float angle;
        bool processed;
    };

    struct ChaneWeapon {
        int agent_id;
        WeaponType weapon;
        bool processed;
    };

} // namespace Escape
#define COMPONENT_LIST \
    Escape::Position,          \
    Escape::Name,              \
    Escape::Rotation,          \
    Escape::Velocity,          \
    Escape::Health,            \
    Escape::Weapon,            \
    Escape::WeaponPrototype,   \
    Escape::Hitbox,            \
    Escape::BulletData,        \
    Escape::Lifespan,          \
    Escape::TimeServerInfo,    \
    Escape::AgentData,           \
    Escape::TerrainData

#define FOREACH_COMPONENT_TYPE(func) \
    func(Position);          \
    func(Name);              \
    func(Rotation);          \
    func(Velocity);          \
    func(Health);            \
    func(Weapon);            \
    func(WeaponPrototype);   \
    func(Hitbox);            \
    func(BulletData);        \
    func(Lifespan);          \
    func(TimeServerInfo);    \
    func(AgentData);           \
    func(TerrainData);

#endif // COMPONENTS_H

#pragma clang diagnostic pop