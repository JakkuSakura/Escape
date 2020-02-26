#include "serialization.h"
#include <vector>
#include <map>

using namespace std;
namespace {
    struct WrapperBase {
        virtual ~WrapperBase() {}

    };

    template<typename T>
    struct Wrapper : public WrapperBase {
        Wrapper(const Wrapper<T> &o) : data(o.data) {}

        Wrapper(const T &data) : data(data) {}

        T data;
    };
}
using namespace Escape;
namespace boost {
    namespace serialization {

        template<typename Archive>
        void serialize(Archive &ar, vector<WrapperBase *> &vec, unsigned int version) {
            int size = vec.size();
            ar & BOOST_SERIALIZATION_NVP(size);
            for (int i = 0; i < size; ++i) {
                auto bs = vec[i];

#define SAVE_TO_STRING(type) {auto *p = dynamic_cast<Wrapper<type> *>(bs); \
            if (p) \
            { \
                 const auto &type = p->data; \
                ar & BOOST_SERIALIZATION_NVP(type); \
            }}
#define SAVE_TO_STRING2(type) SAVE_TO_STRING(type)
                FOREACH_COMPONENT_TYPE(SAVE_TO_STRING2);

            }
        }

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
                for (auto &pair : world) {
                    int entity = pair.first;
                    auto &components = pair.second;
                    ar & BOOST_SERIALIZATION_NVP(entity);
                    ar & BOOST_SERIALIZATION_NVP(components);

                }
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
