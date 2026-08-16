#ifndef CORE_COMMON_TO_STR_INL
#define CORE_COMMON_TO_STR_INL

#include "to_str.h"

template <>
inline char* amit::common::ToStr<int>(const int& data)
{
    char* ret_char_array                          = kCharArrayStorageFor32BitData;
    ret_char_array[kMaxCharArraySizeFor32Bit - 1] = '\0';
    uint8_t begin                                 = kMaxCharArraySizeFor32Bit - 2;
    int     copied_data                           = data < 0 ? (0 - data) : data;

    if (copied_data > 0)
    {
        while (copied_data)
        {
            ret_char_array[begin] = kDecimalInChars[copied_data % 10];
            copied_data /= 10;
            begin--;
        }

        if (data < 0)
        {
            ret_char_array[begin] = '-';
        }
        else
        {
            begin++;
        }
    }
    else
    {
        ret_char_array[begin] = kDecimalInChars[0];
    }

    char* ret_ptr = ret_char_array + begin;
    return ret_ptr;
}


#endif