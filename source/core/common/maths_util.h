#ifndef CORE_COMMON_MATHS_UTIL_H
#define CORE_COMMON_MATHS_UTIL_H

namespace amit::common
{
    namespace maths
    {
        int GetHCF(int a, int b)
        {
            int remainder = a % b;
            if (remainder == 0)
            {
                return b;
            }

            a = b;
            b = remainder;
            return GetHCF(a, b);
        }
    }  // namespace maths
}  // namespace amit::common
#endif