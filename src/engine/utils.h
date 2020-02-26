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

#include <cstring>
// template <typename... Args>
// NewName(Args &&... args) : BaseName(std::forward<Args>(args)...) {}
using clock_type = float;
#define FORWARD_CONSTRUCTORS(NewName, BaseName)                              \
    using base_type__ = BaseName;                                            \
    using new_type__ = NewName;                                              \
    using base_type__::base_type__;                                          \
    NewName(const base_type__ &old) : base_type__(old)                       \
    {                                                                        \
    }                                                                        \
    constexpr base_type__ &unwrap()                                          \
    {                                                                        \
        return *static_cast<base_type__ *>((void *)this);                    \
    }                                                                        \
    constexpr const base_type__ &unwrap() const                              \
    {                                                                        \
        return *static_cast<const base_type__ *>((void *)this);              \
    }                                                                        \
    template <typename T>                                                    \
    constexpr T &to()                                                        \
    {                                                                        \
        static_assert(sizeof(NewName) == sizeof(T),                          \
                      "The argument should have the same memory structure"); \
        return *static_cast<T *>((void *)this);                              \
    }                                                                        \
    template <typename T>                                                    \
    constexpr const T &to() const                                            \
    {                                                                        \
        static_assert(sizeof(NewName) == sizeof(T),                          \
                      "The argument should have the same memory structure"); \
        return *static_cast<const T *>((void *)this);                        \
    }                                                                        \
    template <typename T>                                                    \
    constexpr NewName &from(const T &src)                                    \
    {                                                                        \
        this->to<T>() = src;                                                 \
        return *this;                                                        \
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
