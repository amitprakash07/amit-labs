#include "util.h"

namespace amit::common
{
    namespace util
    {
        std::vector<int> PrefixArray(const std::vector<int>& input)
        {
            std::vector<int> return_prefix_array;
            return_prefix_array.resize(input.size());

            return_prefix_array[0] = input[0];

            for (size_t iter = 1; iter < input.size(); iter++)
            {
                return_prefix_array[iter] = return_prefix_array[iter - 1] + input[iter];
            }

            return return_prefix_array;
        }

        std::string ToStr(const std::vector<int>& input_array)
        {
            std::stringstream ss;
            ss << "{ ";
            for (const int& val : input_array)
            {
                ss << val << ",";
            }

            ss << " }\n";

            return std::string(ss.str());
        }
    }  // namespace util
}  // namespace amit::common