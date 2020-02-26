#if !defined(SERIALIZATION_H)
#define SERIALIZATION_H

#include <glm/glm.hpp>
#include <utility>
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

        explicit SerializationHelper(std::string name);

        void serialize(const World &world);


        void deserialize(World &world);

    };

} // namespace Escape

#endif // SERIALIZATION_H
