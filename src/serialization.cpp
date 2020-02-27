/*
 * This file is deemed to be ugly
 * Consider refactor it in the future
 * But not recently for it's not the main module
 * Jack Quinn Feb 27, 2020
 * TODO consider modifying xml_archive
 */
#include "serialization.h"
#include <vector>
#include <map>
#include <glm/glm.hpp>
#include <utility>
#include <fstream>
#include <string>
#include "components.h"
#include <iostream>
#include <typeindex>
#include <ThorSerialize/Traits.h>
#include <ThorSerialize/JsonThor.h>
#include <ThorSerialize/SerUtil.h>

using namespace std;
using namespace Escape;

ThorsAnvil_MakeTrait(Name, name);
ThorsAnvil_MakeTrait(vec2, x, y);
ThorsAnvil_ExpandTrait(vec2, Position);
ThorsAnvil_ExpandTrait(vec2, Velocity);
ThorsAnvil_ExpandTrait(vec2, Impulse);
ThorsAnvil_MakeTrait(Hitbox, radius);
ThorsAnvil_MakeTrait(Rotation, radian);
ThorsAnvil_MakeTrait(AgentData, id, player);
ThorsAnvil_MakeTrait(TimeServerInfo, tick);
ThorsAnvil_MakeTrait(Lifespan, begin, end);
ThorsAnvil_MakeTrait(Health, max_health);
ThorsAnvil_MakeTrait(BulletData, firer_id, type, damage, density, radius);
ThorsAnvil_MakeTrait(WeaponPrototype, type, bullet_type, cd, accuracy, bullet_number, bullet_damage, bullet_speed,
                     gun_length);
ThorsAnvil_MakeTrait(Weapon, weapon, last, next);
ThorsAnvil_MakeTrait(TerrainData, type, argument_1, argument_2, argument_3, argument_4);


struct WrapperBase {
    virtual ~WrapperBase() {};

    virtual void *getData() { return 0; };

    ThorsAnvil_PolyMorphicSerializer(WrapperBase);
};

template<typename T>
struct Wrapper : public WrapperBase {
    Wrapper() : data() {}

    Wrapper(const T &d) : data(d) {}

    T data;

    void *getData() {
        return &data;
    }

//    ThorsAnvil_PolyMorphicSerializer(Wrapper<T>);

};
ThorsAnvil_MakeTrait(WrapperBase);


#define REGISTER(type)  ThorsAnvil_ExpandTrait(WrapperBase, Wrapper<type>, data);
#define REGISTER2(type) REGISTER(type)

FOREACH_COMPONENT_TYPE(REGISTER2);

using namespace Escape;
using ThorsAnvil::Serialize::jsonExport;
using ThorsAnvil::Serialize::jsonImport;

class entt_oarchive {
    map<ENTT_ID_TYPE, vector<WrapperBase *>> world;
    ostream &os;
    int stage;
public:
    entt_oarchive(ostream &os) : os(os), stage(0) {

    }

    ~entt_oarchive() {
        for (auto &ent : world) {
            for (auto &comp : ent.second) {
                delete comp;
            }
        }
    }

    void write_to_stream() {
//        os << jsonExport(world);
    }

    // output
    void operator()(int size) {
        stage += 1;
    }

    void operator()(const entt::entity &entity) {

    }


    template<typename T>
    void operator()(entt::entity entity, const T &data) {
        world[entt::to_integral(entity)].push_back(new Wrapper<T>(data));
    }

};


class entt_iarchive {
    int stage;
    istream &is;
    map<ENTT_ID_TYPE, vector<WrapperBase *>> world;
    map<ENTT_ID_TYPE, vector<WrapperBase *>>::iterator entity_iter;
    vector<type_index> components_order;
    vector<type_index>::iterator component_iter;
    map<type_index, vector<pair<ENTT_ID_TYPE, void *>>> pairs;
    vector<pair<ENTT_ID_TYPE, void *>>::iterator pair_iter2;

public:
    entt_iarchive(istream &is) : is(is), stage(0) {
    }

    ~entt_iarchive() {
        for (auto &ent : world) {
            for (auto &comp : ent.second) {
                delete comp;
            }
        }
    }


    void read_from_stream() {
//        is >> jsonImport(world);
    }

    template<typename T>
    void single_component() {
        components_order.emplace_back(typeid(T));
        for (auto &pair : world) {
            for (auto *comp : pair.second) {
                if (typeid(Wrapper<T>) == typeid(*comp)) {
                    pairs[typeid(T)].push_back(make_pair(pair.first, comp->getData()));
                }
            }
        }
    }

    template<typename ... components>
    void set_orders() {
        (single_component<components>(), ...);
        component_iter = components_order.begin();
    }

    // input
    void operator()(entt::entt_traits<unsigned int>::entity_type &size) {
        stage += 1;
        if (stage == 1) // entities alive
        {
            size = world.size();
        } else { // components
            size = pairs[*component_iter].size();
            pair_iter2 = pairs[*component_iter].begin();
            ++component_iter;
        }
        entity_iter = world.begin();
    }

    void operator()(entt::entity &entity) {
        entity = (entt::entity) entity_iter->first;
        ++entity_iter;
    }

    template<typename T>
    void operator()(entt::entity &entity, T &data) {
        entity = (entt::entity) pair_iter2->first;
        data = *(T *) pair_iter2->second;
        ++pair_iter2;
    }
};


Escape::SerializationHelper::SerializationHelper(std::string name) : filename(std::move(name)) {
}

void Escape::SerializationHelper::serialize(const Escape::World &world) {
    std::ofstream stream(filename);
    entt_oarchive output(stream);
    auto snapshot = const_cast<entt::registry &>(world).snapshot();
    snapshot.component<COMPONENT_LIST>(output);
    output.write_to_stream();

}

void Escape::SerializationHelper::deserialize(Escape::World &world) {
    world.clear();
    std::ifstream stream(filename);
    entt_iarchive input(stream);
    input.read_from_stream();
    input.set_orders<COMPONENT_LIST>();
    auto snapshot = world.loader();
    snapshot.entities(input);
    snapshot.component<COMPONENT_LIST>(input);
    snapshot.orphans();
}
