#if !defined(SERIALIZATION_H)
#define SERIALIZATION_H

#include "MyECS.h"

namespace Escape {
    class SerializationHelper {
        std::string filename;
    public:

        explicit SerializationHelper(std::string name);

        void serialize(const World &world);

        void deserialize(World &world);

        void serialize(const World &world, const entt::entity ent);

    };

} // namespace Escape

#endif // SERIALIZATION_H
