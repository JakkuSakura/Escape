#if !defined(SERIALIZATION_H)
#define SERIALIZATION_H

#include <glm/glm.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>
#include <fstream>
#include <string>
#include "MyECS.h"
#include "components.h"
#include <sstream>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <iostream>
#include <iomanip>
#include <sstream>

namespace Escape {
    class SerializationHelper {
        std::string filename;
    public:

        SerializationHelper(const std::string &name) : filename(name) {
        }

        template<typename T>
        void serialize(const T &obj) {
            std::ofstream stream(filename);
            boost::archive::xml_oarchive oa(stream);
            oa << BOOST_SERIALIZATION_NVP(obj);
        }

        template<typename T>
        void serialize_ptr(const T *obj) {
            serialize(*obj);
        }

        template<typename T>
        T &deserialize(T &obj) {
            std::ifstream stream(filename);
            boost::archive::xml_iarchive ia(stream);
            ia >> BOOST_SERIALIZATION_NVP(obj);
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
        class entt_archive {
            Archive &ar;
        public:
            entt_archive(Archive &ar) : ar(ar) {

            }

            void operator()(int &size) {
                ar & BOOST_SERIALIZATION_NVP(size);
            }

            void operator()(int size) {
                ar & BOOST_SERIALIZATION_NVP(size);
            }

            template<typename T>
            void operator()(T &entity) {
                ar & BOOST_SERIALIZATION_NVP(entity);
            }

            template<typename Te, typename Td>
            void operator()(Te &ent, Td &data) {
                ar & BOOST_SERIALIZATION_NVP(ent);
                ar & BOOST_SERIALIZATION_NVP(data);
            }

            template<typename Te, typename Td>
            void operator()(const Te &ent, const Td &data) {
                ar & BOOST_SERIALIZATION_NVP(ent);
                ar & BOOST_SERIALIZATION_NVP(data);
            }
        };


        template<typename Archive>
        inline void serialize(Archive &ar, glm::vec2 &p, const unsigned int version) {
            ar & BOOST_SERIALIZATION_NVP(p.x);
            ar & BOOST_SERIALIZATION_NVP(p.y);
        }


        template<class Archive>
        void save(Archive &ar, const entt::registry &p, unsigned int version) {
            auto &world = const_cast<entt::registry &>(p);
            entt_archive output(ar);
            auto snapshot = world.snapshot();
            snapshot.entities(output);
            snapshot.component<COMPONENT_LIST>(output);
        }

        template<class Archive>
        void load(Archive &ar, entt::registry &p, unsigned int version) {
            p.clear();
            
            entt_archive input(ar);
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

#endif // SERIALIZATION_H
