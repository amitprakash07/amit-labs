#ifndef CORE_COMMON_HASHED_STRING_H
#define CORE_COMMON_HASHED_STRING_H

#include "rtti.h"
#include "shared_pointer.h"

namespace amit::common
{
    namespace utils
    {
        class StringHash : public RTTI
        {
        public:
            StringHash();
            StringHash(const char*);
            StringHash(const StringHash&);
            unsigned __int32        getHash() const;
            StringHash&             operator=(const StringHash&);
            bool                    operator!=(const StringHash&) const;
            bool                    operator==(const StringHash&) const;
            bool                    operator<(const StringHash&) const;
            static unsigned __int32 Hash(const char*);
            static unsigned __int32 Hash(const void*, size_t);
            bool                    isNil() const;
            std::string             GetType() const override
            {
                return "";
            }
            bool IsSameType(SharedPointer<RTTI>, std::string) const override
            {
                return true;
            }

        private:
            unsigned __int32 mHash;
#ifdef _DEBUG
            char* mString;
#endif
        };
    }  // namespace utils
}  // namespace amit::common

#endif