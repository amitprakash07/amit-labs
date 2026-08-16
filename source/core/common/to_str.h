#ifndef CORE_COMMON_TO_STR_H
#define CORE_COMMON_TO_STR_H

#include <cstdint>
#include <vector>

namespace amit::common
{
    constexpr char    kDecimalInChars[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    constexpr char    kHexInChars[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    constexpr char    kOctInChars[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8'};
    constexpr char    kBinaryInChars[]                                         = {'0', '1'};
    constexpr uint8_t kMaxCharArraySizeFor32Bit                                = 11;
    constexpr uint8_t kMaxCharArraySizeFor64Bit                                = 22;
    inline char       kCharArrayStorageFor32BitData[kMaxCharArraySizeFor32Bit] = {};
    inline char       kCharArrayStorageFor64BitData[kMaxCharArraySizeFor64Bit] = {};

    template <typename T>
    char* ToStr(const T&)
    {
        return nullptr;
    }

    template <>
    char* ToStr<int>(const int& data);

}  // namespace amit::common

#include "to_str.inl"

#endif
