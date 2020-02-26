#if !defined(SERIALIZATION_H)
#define SERIALIZATION_H

#include <glm/glm.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <fstream>
#include <string>
#include <type_traits>
#include "MyECS.h"
#include "components.h"
#include <sstream>

namespace Escape {
    class SerializationHelper {
    public:
        std::string filename;

        SerializationHelper(const std::string &name) : filename(name) {
        }

        template<typename T>
        void serialize(const T &obj) {
            std::ofstream stream(filename);
            boost::archive::text_oarchive oa(stream);
            oa << obj;
        }

        template<typename T>
        void serialize_ptr(const T *obj) {
            serialize(*obj);
        }

        template<typename T>
        T &deserialize(T &obj) {
            std::ifstream stream(filename);
            boost::archive::text_iarchive ia(stream);
            ia >> obj;
            return obj;
        }

        template<typename T>
        T *deserialize_ptr(T *obj) {
            return &deserialize(*obj);
        }
    };

} // namespace Escape

namespace boost {
    namespace serialization {

        template<typename Archive>
        inline void serialize(Archive &ar, glm::vec2 &p, const unsigned int version) {
            ar & p.x & p.y;
        }

        template<class Archive>
        void save(Archive &ar, const entt::entity &p, unsigned int version, entt::registry &reg) {
            int count = 0;
            reg.visit(p, [&](auto component) {
                count += 1;
            });
            ar & count;
#define DISPATCHALL() FOREACH_COMPONENT_TYPE(DISPATCH)


#define DISPATCH(type)          \
    if (reg.has<type>(p))       \
    {                           \
        ar &std::string(#type); \
        ar &reg.get<type>(p);   \
    }

            DISPATCHALL();

#undef DISPATCH
        }

        template<class Archive>
        void load(Archive &ar, entt::entity &p, unsigned int version, entt::registry &reg) {
            int count;
            ar & count;
            for (int i = 0; i < count; ++i) {
                std::string c_type;
                ar & c_type;
#define DISPATCH(type)           \
    if (c_type == #type)         \
    {                            \
        type comp;               \
        ar &comp;                \
        reg.assign<type>(p, comp); \
    }

                DISPATCHALL();

#undef DISPATCH
            }
        }

// template <class Archive>
// void save(Archive &ar, const std::type_index &p, unsigned int version)
// {
//     ar &Escape::ComponentRegister::getInstance().getName(p);
// }
// template <class Archive>
// void load(Archive &ar, std::type_index &p, unsigned int version)
// {
//     std::string name;
//     ar &name;
//     p = Escape::ComponentRegister::getInstance().getTypeInfo(name);
// }

// template <typename Archive>
// inline void serialize(Archive &ar, std::type_index &p, const unsigned int version)
// {
//     split_free(ar, p, version);
// }

        template<class Archive>
        void save(Archive &ar, const entt::registry &p, unsigned int version) {
            auto &world = const_cast<entt::registry &>(p);
            ar & world.alive();
            world.each([&](entt::entity ent) {
                save(ar, ent, version, world);
            });
        }

        template<class Archive>
        void load(Archive &ar, entt::registry &p, unsigned int version) {
            p.clear();
            size_t size;
            ar & size;
            for (size_t i = 0; i < size; i++) {
                entt::entity ent = p.create();
                load(ar, ent, version, p);
            }
        }

        template<typename Archive>
        inline void serialize(Archive &ar, entt::registry &p, const unsigned int version) {
            split_free(ar, p, version);
        }

    } // namespace serialization
} // namespace boost

#endif // SERIALIZATION_H
