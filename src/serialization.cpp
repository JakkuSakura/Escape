/*
 * This file is deemed to be ugly
 * Consider refactor it in the future
 * But not recently for it's not the main module
 * Jack Quinn Feb 27, 2020
 */
#include "serialization.h"
#include <vector>
#include <map>
#include <glm/glm.hpp>
#include <utility>

#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/map.hpp>
#include <fstream>
#include <string>
#include "components.h"
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <iostream>

#include <boost/serialization/export.hpp>

using namespace std;
using namespace Escape;

struct WrapperBase {
    virtual ~WrapperBase() {}
    virtual void *getData() = 0;
    template<typename Archive>
    void serialize(Archive &ar, unsigned int version) {}
};

BOOST_SERIALIZATION_ASSUME_ABSTRACT(WrapperBase);

#define REGISTER(type)  BOOST_CLASS_EXPORT(type)
#define REGISTER2(type) REGISTER(Wrapper<type>)

template<typename T>
struct Wrapper : public WrapperBase {
    Wrapper(const Wrapper<T> &o) : data(o.data) {}

    Wrapper() : data() {}

    Wrapper(const T &data) : data(data) {}
    void * getData() override {
        return &data;
    }
    T data;
#pragma clang diagnostic push
#pragma ide diagnostic ignored "HidingNonVirtualFunction"

    template<typename Archive>
    void serialize(Archive &ar, unsigned int version) {
        auto &base = boost::serialization::base_object<WrapperBase>(*this);
        data.serialize(ar, version);
    }

#pragma clang diagnostic pop
};
FOREACH_COMPONENT_TYPE(REGISTER2);

using namespace Escape;
namespace boost {
    namespace serialization {
        template<typename Archive>
        void
        serialize(Archive &ar, std::pair<const unsigned int, std::vector<WrapperBase *> > &pair, unsigned int version) {
            auto &entity_id = const_cast<unsigned int &>(pair.first);
            auto &components = pair.second;
            ar & BOOST_SERIALIZATION_NVP(entity_id);
            ar & BOOST_SERIALIZATION_NVP(components);
        }

        template<typename Archive>
        class entt_oarchive {
            Archive &ar;
            map<ENTT_ID_TYPE, vector<WrapperBase *>> world;
            int stage;
            map<ENTT_ID_TYPE, vector<WrapperBase *>>::iterator iterator;
        public:
            entt_oarchive(Archive &ar) : ar(ar), stage(0) {

            }

            ~entt_oarchive() {
                for (auto &ent : world) {
                    for (auto &comp : ent.second) {
                        delete comp;
                    }
                }
            }

            void write_to_file() {
                ar & BOOST_SERIALIZATION_NVP(world);
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

        template<typename Archive>
        class entt_iarchive {
            int stage;
            Archive &ar;
            map<ENTT_ID_TYPE, vector<WrapperBase *>> world;
            map<ENTT_ID_TYPE, vector<WrapperBase *>>::iterator iterator;
        public:
            entt_iarchive(Archive &ar) : ar(ar), stage(0) {

            }
            void sync() {
                ar & BOOST_SERIALIZATION_NVP(world);
            }
            // input
            void operator()(entt::entt_traits<unsigned int>::entity_type &size) {
                stage += 1;
                if (stage == 1) // entities alive
                {
                    size = world.size();
                    iterator = world.begin();
                } else { // components
                    // size = count
                    // FIXME what am I supposed to do ???
                    assert(false);
                }
            }

            void operator()(entt::entity &entity) {
                entity = (entt::entity) iterator->first;
            }
            template<typename T>
            void operator()(entt::entity &entity, T &data) {
                auto &ent = world[entt::to_integral(entity)];
                for (int i = 0; i < ent.size(); ++i) {
#define ASSIGN(type) \
                    { \
                        auto *p = dynamic_cast<Wrapper<type> *>(ent[i]); \
                        if(p) \
                        { \
                            data = *(T *)p->getData(); \
                        } \
                    }
                    FOREACH_COMPONENT_TYPE(ASSIGN);
                }
            }
        };


        template<typename Archive>
        inline void serialize(Archive &ar, glm::vec2 &p, const unsigned int version) {
            int x = p.x, y = p.y;
            ar & BOOST_SERIALIZATION_NVP(x);
            ar & BOOST_SERIALIZATION_NVP(y);
        }


        template<class Archive>
        void save(Archive &ar, const entt::registry &p, unsigned int version) {
            auto &world = const_cast<entt::registry &>(p);
            entt_oarchive output(ar);
            auto snapshot = world.snapshot();
            snapshot.component<COMPONENT_LIST>(output);
            output.write_to_file();
        }

        template<class Archive>
        void load(Archive &ar, entt::registry &p, unsigned int version) {
            p.clear();
            entt_iarchive input(ar);
            input.sync();
            auto snapshot = p.loader();
            snapshot.entities(input);
            snapshot.component<COMPONENT_LIST>(input);
            snapshot.orphans();
        }

        template<typename Archive>
        inline void serialize(Archive &ar, entt::registry &p, const unsigned int version) {
            split_free(ar, p, version);
        }

    } // namespace serialization
} // namespace boost
Escape::SerializationHelper::SerializationHelper(std::string name) : filename(std::move(name)) {
}

void Escape::SerializationHelper::serialize(const Escape::World &world) {
    std::ofstream stream(filename);
    boost::archive::xml_oarchive oa(stream);
    oa << BOOST_SERIALIZATION_NVP(world);
}

void Escape::SerializationHelper::deserialize(Escape::World &world) {
    std::ifstream stream(filename);
    boost::archive::xml_iarchive ia(stream);
    ia >> BOOST_SERIALIZATION_NVP(world);
}
