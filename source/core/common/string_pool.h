#ifndef CORE_COMMON_STRING_POOL_H
#define CORE_COMMON_STRING_POOL_H

#include <vector>

#include "additional_types.h"
#include "rtti.h"

namespace amit::common
{
    class StringPool : public RTTI
    {
    public:
        static SharedPointer<StringPool> Instance();
        char*                            FindString(const char*);
        std::string                      GetType() const override
        {
            return "";
        }
        bool IsSameType(SharedPointer<RTTI>, std::string) const override
        {
            return true;
        }
        ~StringPool();

    private:
        static SharedPointer<StringPool> string_pool_instance_;
        std::vector<typedefs::String>    string_list_;
        char*                            AddString(const char*);
        StringPool();
        StringPool(StringPool&);

    };  // end-StringPool
}  // namespace amit::common

#endif