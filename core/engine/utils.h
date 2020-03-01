#if !defined(UTILS_H)
#define UTILS_H

#include <iostream>

template<typename T>
inline void show_mat(const T &mat) {
    for (size_t i = 0; i < mat.length(); i++) {
        for (size_t j = 0; j < mat[i].length(); j++) {
            std::cerr << mat[i][j] << " ";
        }

        std::cerr << std::endl;
    }
    std::cerr << std::endl;
}

template<typename T>
inline void show_vec(const T &vec) {
    for (size_t j = 0; j < vec.length(); j++) {
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

// The same as reinterpret_cast<>() but it gets rid of most limitations excluding const and size
template<typename D, typename S>
constexpr D &as(S &src) {
    static_assert(sizeof(S) == sizeof(D),
                  "The argument should have the same memory structure");

    return *static_cast<D *>(static_cast<void *>(&src));
}

template<typename D, typename S>
constexpr const D &as(const S &src) {
    static_assert(sizeof(S) == sizeof(D),
                  "The argument should have the same memory structure");

    return *static_cast<const D *>(static_cast<const void *>(&src));
}
template<typename D, typename S>
constexpr D &as_f(S &src) {
    return *static_cast<D *>(static_cast<void *>(&src));
}

template<typename D, typename S>
constexpr const D &as_f(const S &src) {
    return *static_cast<const D *>(static_cast<const void *>(&src));
}

class NewTypeBase {
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

struct VirtualBase {
    virtual ~VirtualBase() {};
};
#endif // UTILS_H
