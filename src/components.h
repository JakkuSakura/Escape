#pragma clang diagnostic push
#pragma ide diagnostic ignored "hicpp-explicit-conversions"
#if !defined(COMPONENTS_H)
#define COMPONENTS_H

#include "engine/utils.h"
#include "MyECS.h"
#include <glm/glm.hpp>

namespace Escape {
    struct Name : public std::string {
        FORWARD_CONSTRUCTORS(Name, std::string);

        template<class Archive>
        void serialize(Archive &ar, const unsigned int version) {
            ar & unwrap();
        }
    };


    struct Position : public glm::vec2 {
        FORWARD_CONSTRUCTORS(Position, glm::vec2);

        template<class Archive>
        void serialize(Archive &ar, const unsigned int version) {
            ar & unwrap();
        }
    };

    struct Velocity : public glm::vec2 {
        FORWARD_CONSTRUCTORS(Velocity, glm::vec2);

        template<class Archive>
        void serialize(Archive &ar, const unsigned int version) {
            ar & unwrap();
        }
    };

    struct Hitbox {
        float radius;

        template<class Archive>
        void serialize(Archive &ar, const unsigned int version) {
            ar & radius;
        }
    };

    struct Rotation {
        float radian;

        template<class Archive>
        void serialize(Archive &ar, const unsigned int version) {
            ar & radian;
        }
    };

    struct AgentData {
        int id;
        int player;

        template<class Archive>
        void serialize(Archive &ar, const unsigned int version) {
            ar & id;
            ar & player;
        }
    };

    struct TimeServerInfo {
        size_t tick;

        template<class Archive>
        void serialize(Archive &ar, const unsigned int version) {
            ar & tick;
        }
    };

    struct Lifespan {
        clock_type begin;
        clock_type end;

        template<typename Archive>
        void serialize(Archive &ar, const unsigned int version) {
            ar & begin & end;
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
            ar & health & max_health;
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
        bool hit;

        template<typename Archive>
        void serialize(Archive &ar, const unsigned int version) {
            ar & firer_id & type & damage & hit;
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
        float peice_number;
        float bullet_damage;
        float bullet_speed;
        float gun_length;

        template<typename Archive>
        void serialize(Archive &ar, const unsigned int version) {
            ar & type;
            ar & bullet_type;
            ar & cd;
            ar & accuracy;
            ar & peice_number;
            ar & bullet_damage;
            ar & bullet_speed;
            ar & gun_length;
        }
    };

    struct Weapon {
        WeaponType weapon;
        float last;
        float next;

        template<typename Archive>
        void serialize(Archive &ar, const unsigned int version) {
            ar & weapon;
            ar & last;
            ar & next;
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
            ar & type;
            ar & arguments;
        }

    };

    // Controls
    // These shouldn't be saved nor assigned as component at all
    struct Impulse {
        int agent_id;
        float angle;
        float impulse;
        bool processed;
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
#define FOREACH_COMPONENT_TYPE(func) \
    func(Escape::Position);          \
    func(Escape::Name);              \
    func(Escape::Rotation);          \
    func(Escape::Velocity);          \
    func(Escape::Health);            \
    func(Escape::Weapon);            \
    func(Escape::WeaponPrototype);   \
    func(Escape::Hitbox);            \
    func(Escape::BulletData);        \
    func(Escape::Lifespan);          \
    func(Escape::TimeServerInfo);    \
    func(Escape::AgentData);           \
    func(Escape::TerrainData);

#endif // COMPONENTS_H

#pragma clang diagnostic pop