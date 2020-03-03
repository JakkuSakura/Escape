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

using namespace std;
using namespace Escape;

ThorsAnvil_MakeTrait(Name, name);
ThorsAnvil_MakeTrait(vec2, x, y);
ThorsAnvil_ExpandTrait(vec2, Position);
ThorsAnvil_ExpandTrait(vec2, Velocity);
ThorsAnvil_ExpandTrait(vec2, Impulse);
ThorsAnvil_MakeTrait(Hitbox, radius);
ThorsAnvil_MakeTrait(Rotation, radian);
ThorsAnvil_MakeTrait(AgentData, player);
ThorsAnvil_MakeTrait(TimeServerInfo, tick);
ThorsAnvil_MakeTrait(Lifespan, begin, end);
ThorsAnvil_MakeTrait(Health, max_health);
ThorsAnvil_MakeTrait(BulletData, firer_id, type, damage, density, radius);
ThorsAnvil_MakeTrait(WeaponPrototype, type, bullet_type, cd, accuracy, bullet_number, bullet_damage, bullet_speed,
                     gun_length);
ThorsAnvil_MakeTrait(Weapon, weapon, last, next);
ThorsAnvil_MakeTrait(TerrainData, type, argument_1, argument_2, argument_3, argument_4);

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

ThorsAnvil_MakeEnum(TerrainType, BOX, CIRCLE);
ThorsAnvil_ExpandTrait(MapInfo::s_s_pair, MapInfo);

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

string to_str(int i) {
    static char buf[30];
    sprintf(buf, "%d", i);
    return buf;
}

int to_int(const string &s, const char *error_info = "Must be an integer: ") {
    int key_ = -1;
    if (sscanf(s.c_str(), "%d", &key_) == EOF)
        throw runtime_error(error_info + s);
    return key_;
}

class entt_oarchive {
    map<string, vector<WrapperBase *>> world;
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
        os << jsonExport(world);
    }

    // output
    void operator()(int size) {
        stage += 1;
    }

    void operator()(const entt::entity &entity) {
    }

    template<typename T>
    void operator()(entt::entity entity, const T &data) {
        world[to_str(entt::to_integral(entity))].push_back(new Wrapper<T>(&data));
    }
};

class entt_iarchive {
    int stage;
    istream &is;
    map<string, vector<WrapperBase *>> world;
    map<string, vector<WrapperBase *>>::iterator entity_iter;
    vector<type_index> components_order;
    vector<type_index>::iterator component_iter;
    map<type_index, vector<pair<ENTT_ID_TYPE, WrapperBase *>>> pairs;
    vector<pair<ENTT_ID_TYPE, WrapperBase *>>::iterator pair_iter2;

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
        is >> jsonImport(world);
    }

    template<typename T>
    void single_component() {
        components_order.emplace_back(typeid(T));
        for (auto &pair : world) {
            for (auto *comp : pair.second) {
                Wrapper<T> *p = dynamic_cast<Wrapper<T> *>(comp);
                if (p) {
                    pairs[typeid(T)].push_back(make_pair(to_int(pair.first), p));
                }
            }
        }
    }

    template<typename... components>
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
        entity = (entt::entity) to_int(entity_iter->first);
        ++entity_iter;
    }

    template<typename T>
    void operator()(entt::entity &entity, T &data) {
        assert(pair_iter2 != pairs[*(component_iter - 1)].end());
        entity = (entt::entity) pair_iter2->first;
        data = **dynamic_cast<Wrapper<T> *>(pair_iter2->second);
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
