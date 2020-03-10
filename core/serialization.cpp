/*
 * This file is deemed to be ugly
 * Consider refactor it in the future
 * But not recently for it's not the main module
 * Jack Quinn Feb 27, 2020
 */
#include "serialization.h"
#include <vector>
#include <map>
#include <utility>
#include <fstream>
#include <string>
#include "components.h"
#include <typeindex>
#include <ThorSerialize/Traits.h>
#include <ThorSerialize/JsonThor.h>
#include <ThorSerialize/SerUtil.h>
#include <iostream>
#include "event_system.h"

using namespace std;
using namespace Escape;

ThorsAnvil_MakeTrait(Name, name);
ThorsAnvil_MakeTrait(vec2, x, y);
ThorsAnvil_ExpandTrait(vec2, Position);
ThorsAnvil_ExpandTrait(vec2, Velocity);
ThorsAnvil_MakeTrait(Hitbox, radius);
ThorsAnvil_MakeTrait(Rotation, radian);
ThorsAnvil_MakeTrait(AgentData, player, group, ai);
ThorsAnvil_MakeTrait(TimeServerInfo, tick);
ThorsAnvil_MakeTrait(Lifespan, begin, end);
ThorsAnvil_MakeTrait(Health, health, max_health);
ThorsAnvil_MakeTrait(BulletData, firer_id, type, damage, density, radius);
ThorsAnvil_MakeTrait(WeaponPrototype, type, bullet_type, cd, accuracy, bullet_number, bullet_damage, bullet_speed,
                     gun_length);
ThorsAnvil_MakeTrait(Weapon, weapon, last, next);
ThorsAnvil_MakeTrait(TerrainData, type, shape, argument_1, argument_2, argument_3, argument_4);

ThorsAnvil_MakeEnum(BulletType,
                    HANDGUN_BULLET,
                    SHOTGUN_SHELL,
                    SMG_BULLET,
                    RIFLE_BULLET);

ThorsAnvil_MakeEnum(WeaponType,
                    HANDGUN,
                    SHOTGUN,
                    SMG,
                    RIFLE);

ThorsAnvil_MakeEnum(TerrainType, AIR, WALL, WATER, BARRAIR);
ThorsAnvil_MakeEnum(TerrainShape, BOX, CIRCLE);
ThorsAnvil_ExpandTrait(MapInfo::s_s_pair, MapInfo);

ThorsAnvil_MakeTrait(Event, actor);


struct WrapperBase {
    WrapperBase() {};

    virtual ~WrapperBase() {};

    ThorsAnvil_PolyMorphicSerializer(WrapperBase);

    virtual const char *getType() { return polyMorphicSerializerName(); }


};

#define My_RegisterPolyMorphicType(DataType, name)                    \
    namespace ThorsAnvil                                              \
    {                                                                 \
    namespace Serialize                                               \
    {                                                                 \
    namespace                                                         \
    {                                                                 \
    ThorsAnvil_InitPolyMorphicType<DataType> THOR_UNIQUE_NAME(#name); \
    }                                                                 \
    }                                                                 \
    }

template<typename T>
struct Wrapper {
};
#define MAKE_WRAPPER(T, name)                                                                       \
    template <>                                                                                     \
    struct Wrapper<T> : public WrapperBase                                                          \
    {                                                                                               \
        using element_type = T;                                                                     \
        T *pointer;                                                                                 \
        Wrapper() { pointer = nullptr; }                                                            \
        Wrapper(T *p) {pointer = p;}                                                                \
        Wrapper(const T *p) {pointer = const_cast<T *>(p);}                                         \
        Wrapper &operator=(const Wrapper &wrapper)                                                  \
        {                                                                                           \
            pointer = wrapper.pointer;                                                              \
            return *this;                                                                           \
        }                                                                                           \
        Wrapper &operator=(std::nullptr_t nil)                                                      \
        {                                                                                           \
            return *this;                                                                           \
        }                                                                                           \
        T &operator*() const {return *pointer;}                                                     \
        T* operator->(){return pointer;}                                                            \
        virtual void printPolyMorphicObject(ThorsAnvil::Serialize::Serializer &parent,              \
                                            ThorsAnvil::Serialize::PrinterInterface &printer)       \
        {                                                                                           \
            parent.print(**this);                                                                   \
        }                                                                                           \
        virtual void parsePolyMorphicObject(ThorsAnvil::Serialize::DeSerializer &parent,            \
                                            ThorsAnvil::Serialize::ParserInterface &parser)         \
        {                                                                                           \
            T *t = new T();                                                                         \
            try {                                                                                   \
            parent.parse(*t);                                                                       \
            } catch (...) {                                                                         \
            delete t;                                                                               \
            throw;                                                                                  \
            }                                                                                       \
            pointer = t;                                                                            \
        }                                                                                           \
        virtual const char *getType() {return polyMorphicSerializerName();  }                       \
        static constexpr char const *polyMorphicSerializerName() { return #name; };                 \
    };                                                                                              \
    My_RegisterPolyMorphicType(Wrapper<T>, name);

ThorsAnvil_MakeTrait(WrapperBase);
#define MAKE_WRAPPER2(type) MAKE_WRAPPER(type, type)

FOREACH_COMPONENT_TYPE(MAKE_WRAPPER2);


namespace ThorsAnvil {
    namespace Serialize {

        template<typename T>
        class Traits<Wrapper<T>> {
        public:
            static constexpr TraitType type = TraitType::Pointer;

            static Wrapper<T> alloc() { return Wrapper<T>(new T()); }

            static void release(Wrapper<T> &p) {
                delete p.pointer;
                p.pointer = nullptr;
            }
        };

        template<>
        class DeSerializationForBlock<TraitType::Array, vector<WrapperBase *>> {
            typedef vector<WrapperBase *> T;
            DeSerializer &parent;
            ParserInterface &parser;
            std::string key;
        public:
            DeSerializationForBlock(DeSerializer &parent, ParserInterface &parser)
                    : parent(parent), parser(parser) {
                ParserInterface::ParserToken tokenType = parser.getToken();

                if (tokenType != ParserInterface::ParserToken::MapStart) {
                    throw std::runtime_error(
                            "ThorsAnvil::Serialize::DeSerializationForBlock<Map>::DeSerializationForBlock: Invalid Object Start");
                }
            }

            WrapperBase *parseObject(const std::string &className) {
                using BaseType  = WrapperBase;
                using AllocType = typename GetAllocationType<BaseType>::AllocType;
                auto *object = ConvertPointer<BaseType>::assign(
                        PolyMorphicRegistry::getNamedTypeConvertedTo<AllocType>(className));
                object->parsePolyMorphicObject(parent, parser);
                return object;
            }

            void scanObject(T &object) {
                while (hasMoreValue()) {
                    WrapperBase *obj = parseObject(key);
                    object.push_back(obj);
                }
            }

            bool hasMoreValue() {
                ParserInterface::ParserToken tokenType = parser.getToken();
                bool result = tokenType != ParserInterface::ParserToken::MapEnd;
                if (result) {
                    if (tokenType != ParserInterface::ParserToken::Key) {
                        throw std::runtime_error(
                                "ThorsAnvil::Serialize::DeSerializationForBlock<Map>::hasMoreValue: Expecting key token");
                    }
                    key = parser.getKey();
                }

                return result;
            }
        };

        template<>
        class SerializerForBlock<TraitType::Array, vector<WrapperBase *>> {
            typedef vector<WrapperBase *> T;
            Serializer &parent;
            PrinterInterface &printer;
            T const &object;
        public:
            SerializerForBlock(Serializer &parent, PrinterInterface &printer, T const &object)
                    : parent(parent), printer(printer), object(object) {
                printer.openMap();
            }

            ~SerializerForBlock() {
                printer.closeMap();
            }

            void printMembers() {
                for (WrapperBase *w : object) {
                    if (w) {
                        printer.addKey(w->getType());
                        parent.print(w);
                    }
                }
            }
        };
    }
}


using namespace Escape;
using ThorsAnvil::Serialize::jsonExport;
using ThorsAnvil::Serialize::jsonImport;


class entt_oarchive {
    map<string, vector<WrapperBase *>> world;
    World *world_;
    ostream &os;

public:
    entt_oarchive(ostream &os, World *world__) : os(os), world_(world__) {
    }

    ~entt_oarchive() {
        for (auto &ent : world) {
            for (auto &comp : ent.second) {
                delete comp;
            }
        }
    }

    void flush() {
        os << jsonExport(world);
    }

    void flush(entt::entity entity) {
        os << jsonExport(world[to_str(entt::to_integral(entity))]);
    }

    template<typename T>
    void component(entt::entity entity) {
        if (world_->has<T>(entity))
            world[to_str(entt::to_integral(entity))].push_back(new Wrapper<T>(&world_->get<T>(entity)));
    }

    template<typename ... Args>
    void components(entt::entity entity) {
        (component<Args>(entity), ... );
    }
};

class entt_iarchive {
    istream &is;
    map<string, vector<WrapperBase *>> world;
    World *world_;


public:
    entt_iarchive(istream &is, World *world__) : is(is), world_(world__) {
    }

    ~entt_iarchive() {
        for (auto &ent : world) {
            for (auto &comp : ent.second) {
                delete comp;
            }
        }
    }

    void read_from_stream() {
        is >> jsonImport(world);
    }

    template<typename T>
    void component(entt::entity entity, const vector<WrapperBase *> &components) {
        for (int i = 0; i < components.size(); ++i) {
            auto *p = dynamic_cast<Wrapper<T> *>(components[i]);
            if (p) {
                world_->assign_or_replace<T>(entity, **p);
            }
        }
    }

    template<typename ... Args>
    void components() {
        for (auto pair : world) {
            components((entt::entity) to_int(pair.first), pair.second);
        }
    }

    template<typename ... Args>
    void components(entt::entity ent, const vector<WrapperBase *> &comps) {
        if (!world_->valid(ent))
            world_->create(ent);
        (component<Args>(ent, comps), ...);

    }

};

Escape::SerializationHelper::SerializationHelper() {
}

void Escape::SerializationHelper::serialize(const Escape::World &world, std::ostream &stream) {
    entt_oarchive output(stream, const_cast<Escape::World *>(&world));
    world.each([&](entt::entity ent) {
        output.components<COMPONENT_LIST>(ent);
    });
}

void Escape::SerializationHelper::deserialize(Escape::World &world, std::istream &stream) {
    world.clear();
    entt_iarchive input(stream, &world);
    input.components<COMPONENT_LIST>();

}

void SerializationHelper::serialize(const World &world, const entt::entity ent, std::ostream &stream) {
    entt_oarchive output(stream, const_cast<Escape::World *>(&world));
    output.components<COMPONENT_LIST>(ent);
    output.flush(ent);
}


void SerializationHelper::serialize(const WeaponType &object, std::ostream &stream) {
    stream << jsonExport(object);
}

void SerializationHelper::deserialize(WeaponType &object, std::istream &stream) {
    stream >> jsonImport(object);
}

namespace Escape {
    nlohmann::json getEntityInfo(World *world, entt::entity ent) {
        SerializationHelper helper;
        std::stringstream ss;
        helper.serialize(*world, ent, ss);
        nlohmann::json js;
        ss >> js;
        return js;
    }

    void setEntityInfo(World *world, entt::entity ent, const nlohmann::json &js) {
        std::stringstream ss;
        vector<WrapperBase *> comps;
        ss << js;
        ss >> jsonImport(comps);
        entt_iarchive ia(ss, world);
        ia.components<COMPONENT_LIST>(ent, comps);

    }

}

