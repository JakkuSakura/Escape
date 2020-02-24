#if !defined(SERIALIZATION_H)
#define SERIALIZATION_H
#include <string>

namespace Escape
{

template <typename Archive, typename T>
void serialize(Archive &ar, T &p, const unsigned int version)
{
    static_assert(std::is_base_of_v<glm::vec2, T>, "Must be derived from glm::vec2 without modification");
    ar &T.x &T.y;
}
template<typename T>
void serialize(T pod, std::string &buf)
{
    
}
template<typename T>
void serialize(T pod, std::string &buf)
{
    
}

} // namespace Escape


#endif // SERIALIZATION_H

