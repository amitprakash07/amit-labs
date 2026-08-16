#ifndef CORE_COMMON_TIME_HELPER_H
#define CORE_COMMON_TIME_HELPER_H

#include <stdint.h>

namespace amit::common
{
    namespace time
    {
        inline uint64_t convertToMilliSeconds(const uint64_t i_seconds);
        inline uint64_t convertToSeconds(const uint64_t i_milliSeconds);
        inline uint32_t getTimeDiff(const uint64_t, const uint64_t);
        inline uint32_t getTickDifference(uint64_t, uint64_t);
    }  // namespace time
}  // namespace amit::common

#include "time_helper.inl"

#endif