#if !defined(SERIALIZATION_H)
#define SERIALIZATION_H

#include "MyECS.h"
#include "components.h"
namespace Escape {
    class SerializationHelper {
    public:

        explicit SerializationHelper();

        void deserialize(World &world, std::istream &stream);

        void serialize(const World &world, std::ostream &stream);

        void serialize(const WeaponType &object, std::ostream &stream);

        void deserialize(WeaponType &object, std::istream &stream);

        void serialize(const World &world, entt::entity ent, std::ostream &stream);
    };

} // namespace Escape


#endif // SERIALIZATION_H
