#ifndef CORE_COMMON_UTIL_H
#define CORE_COMMON_UTIL_H

#include <sstream>
#include <vector>

namespace amit::common
{
    namespace util
    {
        std::vector<int> PrefixArray(const std::vector<int>& input);
        std::string      ToStr(const std::vector<int>& input_array);

    }  // namespace util
}  // namespace amit::common
#endif