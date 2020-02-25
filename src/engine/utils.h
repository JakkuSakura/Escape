#if !defined(UTILS_H)
#define UTILS_H

#include <iostream>
template <typename T>
inline void show_mat(const T &mat)
{
    for (size_t i = 0; i < mat.length(); i++)
    {
        for (size_t j = 0; j < mat[i].length(); j++)
        {
            std::cerr << mat[i][j] << " ";
        }

        std::cerr << std::endl;
    }
    std::cerr << std::endl;
}
template <typename T>
inline void show_vec(const T &vec)
{
    for (size_t j = 0; j < vec.length(); j++)
    {
        std::cerr << vec[j] << " ";
    }

    std::cerr << std::endl;
}
using clock_type = float;
#define FORWARD_CONSTRUCTORS(NewName, BaseName)                              \
    template <typename... Args>                                              \
    NewName(Args &&... args) : BaseName(std::forward<Args>(args)...) {}      \
    BaseName &unwrap()                                                       \
    {                                                                        \
        return *reinterpret_cast<BaseName *>(this);                          \
    }                                                                        \
    const BaseName &unwrap() const                                           \
    {                                                                        \
        return *reinterpret_cast<const BaseName *>(this);                    \
    }                                                                        \
    template <typename T>                                                    \
    NewName &from(T &&src)                                                   \
    {                                                                        \
        /*static_assert(std::is_pod_v<T>,                                    \
                      "The argument should be POD");  */                     \
        static_assert(sizeof(NewName) == sizeof(T),                          \
                      "The argument should have the same memory structure"); \
        return *this = std::move(*reinterpret_cast<const BaseName *>(this)); \
    }

class NewTypeBase
{
};
#define NEW_TYPE(NewName, BaseName)                               \
    struct NewName final : public BaseName, protected NewTypeBase \
    {                                                             \
        FORWARD_CONSTRUCTORS(NewName, BaseName);                  \
    }
#define TOKENPASTE(x, y) x##y
#define TOKENPASTE2(x, y) TOKENPASTE(x, y)
#define RUN(codes)                  \
    static struct                   \
    {                               \
        struct __internal           \
        {                           \
            __internal() { codes; } \
        } temp;                     \
    } TOKENPASTE2(__temp_codes_, __LINE__)

#define TRACE() std::cerr << "" << __FILE__ << ":" << __LINE__ << "(" << __func__ << ")" << std::endl;
#endif // UTILS_H
