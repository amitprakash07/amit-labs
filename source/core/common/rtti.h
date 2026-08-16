#ifndef CORE_COMMON_RTTI_H
#define CORE_COMMON_RTTI_H

#include <string>

#include "shared_pointer.h"

#define MAKE_STRING(X) #X

namespace amit::common
{
    template <typename TypeName>
    std::string ToString()
    {
        return MAKE_STRING(TypeName);
    };
    class RTTI
    {
    public:
        virtual std::string GetType() const                                    = 0;
        virtual bool        IsSameType(SharedPointer<RTTI>, std::string) const = 0;

        virtual ~RTTI() = default;
        RTTI()          = default;

        RTTI(const RTTI&)            = default;
        RTTI& operator=(const RTTI&) = default;

        RTTI(RTTI&&)            = default;
        RTTI& operator=(RTTI&&) = default;
    };

}  // namespace amit::common

#endif