#ifndef CORE_COMMON_QUEUE_H
#define CORE_COMMON_QUEUE_H

#include <cstdint>

namespace amit::common
{
    template <class DataType, uint64_t initial_size = 100>
    class Queue
    {
    public:
        Queue();
        void        Push(const DataType& data);
        void        Pop();
        DataType    Front() const;
        DataType    Back() const;
        size_t      Size() const;
        bool        Empty() const;
        std::string Print() const;

        static void UnitTest();

    private:
        uint64_t  back_;
        uint64_t  front_;
        bool      empty_;
        DataType* storage_;
        uint64_t  max_container_element_count_;
    };

}  // namespace amit::common

#include "queue.inl"

#endif
