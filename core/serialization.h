#if !defined(SERIALIZATION_H)
#define SERIALIZATION_H

#include "MyECS.h"

namespace Escape {
    class SerializationHelper {
    public:

        explicit SerializationHelper();

        void deserialize(World &world, std::istream &stream);

        void serialize(const World &world, std::ostream &stream);

        void serialize(const World &world, const entt::entity ent, std::ostream &stream);
    };

} // namespace Escape

#endif // SERIALIZATION_H
