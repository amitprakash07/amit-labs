#ifndef CORE_COMMON_UNIQUE_PTR_H_
#define CORE_COMMON_UNIQUE_PTR_H_

namespace amit::common
{
    template <typename T>
    class UniquePtr
    {
    public:
        UniquePtr();
        bool deleteObject();
        ~UniquePtr();
        UniquePtr(const UniquePtr&);
        UniquePtr& operator=(const UniquePtr&);
        T&         operator*();
        T*         operator->();

        UniquePtr(UniquePtr&&)            = default;
        UniquePtr& operator=(UniquePtr&&) = default;

    private:
        T* m_WrappingObject;
    };  // UniquePtr
}  // namespace amit::common
#endif  // header guard