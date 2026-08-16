#ifndef CORE_COMMON_DEFS_H
#define CORE_COMMON_DEFS_H

#define CORE_STRINGIZE_MACRO_(X) #X
#define CORE_STRINGIZE_MACRO(X) CORE_STRINGIZE_MACRO_(X)

#ifndef CORE_SHADER_DIR
#define CORE_SHADER_DIR ""
#endif

namespace amit::common
{
    template <class T>
    concept IntType = std::is_integral_v<T>;

    template <class T>
    concept UnsignedType = std::is_signed_v<T>;

    template <class T>
    concept UnsignedIntType = IntType<T> && !UnsignedType<T>;

    template <class T>
    concept FloatType = std::is_floating_point_v<T>;

}  // namespace amit::common

#endif