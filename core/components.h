#if !defined(COMPONENTS_H)
#define COMPONENTS_H

#include <map>
#include <deque>
#include "engine/utils.h"
#include "MyECS.h"

struct Name {
    std::string name;

    Name() = default;

    Name(std::string &&s) : name(std::move(s)) {}
};


struct vec2 {
    float x, y;

    constexpr vec2(float x, float y) : x(x), y(y) {}

    constexpr vec2() : x(0), y(0) {}

    constexpr vec2(const vec2 &p) = default;

    constexpr vec2 &operator=(vec2 p) {
        as<double>(*this) = as<double>(p);
        return *this;
    }

    constexpr vec2 &operator+=(vec2 b) {
        x += b.x;
        y += b.y;
        return *this;
    }

    constexpr friend vec2 operator+(vec2 a, vec2 b) {
        return a += b;
    }

    constexpr vec2 &operator*=(float b) {
        x *= b;
        y *= b;
        return *this;
    }

    constexpr friend vec2 operator*(vec2 a, float b) {
        return a *= b;
    }

    constexpr friend vec2 operator*(float a, vec2 b) {
        return b *= a;
    }

    constexpr vec2 &operator/=(float b) {
        *this *= 1 / b;
        return *this;
    }

    constexpr friend vec2 operator/(vec2 a, float b) {
        return a /= b;
    }
};


struct Position : public vec2 {
    FORWARD_CONSTRUCTORS(Position, vec2);
};

struct Velocity : public vec2 {
    FORWARD_CONSTRUCTORS(Velocity, vec2);
};



struct CircleShape {
    float radius;
};

struct RectangleShape {
    float width;
    float height;
};


struct Rotation {
    Rotation(float rad = 0) : radian(rad) {}
    float radian;
};


struct AgentData {
    int player; // This number differs from entity id
    int group;
    std::string ai;
};


struct TimeServerInfo {
    size_t tick;
};


struct Lifespan {
    float begin;
    float end;
};


struct Health {
    float health;
    float max_health;

    Health(const Health &h) = default;

    Health(float h = 100) {
        health = h;
        max_health = h;
    };

};


enum class BulletType {
    HANDGUN_BULLET,
    SHOTGUN_SHELL,
    SMG_BULLET,
    RIFLE_BULLET
};

struct BulletData {
    unsigned int firer_id;
    BulletType type;
    float damage;
    float density;
    float radius;
    bool hit;
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
};


struct Weapon {
    WeaponType weapon;
    float last;
    float next;
};


enum class TerrainType {
    AIR,
    WALL,
    WATER,
    BARRAIR,
};

struct TerrainData {
    TerrainType type;
};

struct MapInfo : public std::map<std::string, std::string> {
    using s_s_pair = std::map<std::string, std::string>;
    FORWARD_CONSTRUCTORS(MapInfo, s_s_pair);
};

struct Collidable {
    bool flag;
    // This will not to be checked. It will be always true whatever the value is.
};

//struct

#define COMPONENT_LIST \
    Position,          \
    Name,              \
    Rotation,          \
    Velocity,          \
    Health,            \
    Weapon,            \
    WeaponPrototype,   \
    CircleShape,       \
    RectangleShape,    \
    BulletData,        \
    Lifespan,          \
    TimeServerInfo,    \
    AgentData,         \
    TerrainData,       \
    MapInfo,           \
    Collidable

#define FOREACH_COMPONENT_TYPE(func) \
    func(Position);          \
    func(Name);              \
    func(Rotation);          \
    func(Velocity);          \
    func(Health);            \
    func(Weapon);            \
    func(WeaponPrototype);   \
    func(CircleShape);       \
    func(RectangleShape);    \
    func(BulletData);        \
    func(Lifespan);          \
    func(TimeServerInfo);    \
    func(AgentData);         \
    func(TerrainData);       \
    func(MapInfo);           \
    func(Collidable);        \

#endif // COMPONENTS_H
