#include "serialization.h"
#include <vector>
#include <map>
#include <glm/glm.hpp>
#include <utility>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <fstream>
#include <string>
#include "components.h"
#include <sstream>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <boost/serialization/export.hpp>

using namespace std;
using namespace Escape;

struct WrapperBase {
    virtual ~WrapperBase() {}

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

    T data;
    template<typename Archive>
    void serialize(Archive &ar, unsigned int version) {
        auto &base = boost::serialization::base_object<WrapperBase>(*this);
//        ar & BOOST_SERIALIZATION_NVP(base);
        ar & BOOST_SERIALIZATION_NVP(data);
    }
};
FOREACH_COMPONENT_TYPE(REGISTER2);

using namespace Escape;
namespace boost {
    namespace serialization {
        template<typename Archive>
        class entt_archive {
            Archive &ar;
            map<ENTT_ID_TYPE, vector<WrapperBase *>> world;
        public:
            entt_archive(Archive &ar) : ar(ar) {

            }

            ~entt_archive() {
                for (auto &ent : world) {
                    for (auto &comp : ent.second) {
                        delete comp;
                    }
                }
            }

            void sync() {
                ar & BOOST_SERIALIZATION_NVP(world);
            }

            // output
            void operator()(int size) {
//                ar & BOOST_SERIALIZATION_NVP(size);
            }

            void operator()(const entt::entity &entity) {
//                ar & BOOST_SERIALIZATION_NVP(entity);
            }


            template<typename T>
            void operator()(entt::entity entity, const T &data) {
                world[entt::to_integral(entity)].push_back(new Wrapper<T>(data));
            }

            // input
            void operator()(int &size) {
                ar & BOOST_SERIALIZATION_NVP(size);
            }

            void operator()(entt::entity &entity) {
                ar & BOOST_SERIALIZATION_NVP(entity);
            }

            template<typename T>
            void operator()(entt::entity &entity, T &data) {
                ar & BOOST_SERIALIZATION_NVP(entity);
                ar & BOOST_SERIALIZATION_NVP(data);
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
            entt_archive output(ar);
            auto snapshot = world.snapshot();
            snapshot.component<COMPONENT_LIST>(output);
            output.sync();
        }

        template<class Archive>
        void load(Archive &ar, entt::registry &p, unsigned int version) {
            p.clear();

            entt_archive input(ar);
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
//    std::ifstream stream(filename);
//    boost::archive::xml_iarchive ia(stream);
//    ia >> BOOST_SERIALIZATION_NVP(world);
}
