#ifndef CORE_COMMON_SHARED_PTR_H
#define CORE_COMMON_SHARED_PTR_H

#include <string>

namespace amit::common
{
    template <typename T>
    class SharedPointer
    {
    public:
        SharedPointer();
        SharedPointer(T*, std::string);
        bool deleteObject();
        SharedPointer(const SharedPointer<T>&);
        SharedPointer<T>& operator=(SharedPointer<T>&);
        // bool IsSameType(SharedPointer<RTTI> i_ptr);
        bool isEqual(const SharedPointer<T>&) const;
        // SharedPointer(SharedPointer<T> *);
        T& operator*();
        T* operator->() const;
        ~SharedPointer();
        bool isNull() const;
        // SharedPointer<RTTI> getRawPointer() const;

        template <typename T2>
        SharedPointer<T2> CastSharedPointer();

        T*            m_WrappingObject;
        unsigned int* m_referenceCount;
        std::string   m_typeName;
    };  // SharedPointer
}  // namespace amit::common

#include "shared_pointer.inl"

#endif  // SHARED_PTR_H