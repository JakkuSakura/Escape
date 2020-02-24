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

#define NEW_TYPE(NewName, BaseName) struct NewName final : public BaseName\
{ \
    template<typename ... Args> \
    NewName (Args&& ... args) : BaseName (std::forward<Args>(args) ...) {} \
    BaseName &unwrap() { \
        return *reinterpret_cast<BaseName *>(this); \
    }\
    const BaseName &unwrap() const { \
        return *reinterpret_cast<const BaseName *>(this); \
    }\
}
#endif // UTILS_H
