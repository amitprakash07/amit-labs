#ifndef CORE_COMMON_HEAP_CONTAINER_H
#define CORE_COMMON_HEAP_CONTAINER_H

#include <cstdint>
#include <vector>

#include "heap_container.h"

namespace amit::common
{

    enum class HeapType
    {
        Max,
        Min
    };

    template <typename DataType, HeapType type = HeapType::Max, size_t init_size = 50>
    class HeapContainer
    {
    public:
        HeapContainer()
        {
            data_.resize(buffer_size_);
            current_size_ = 0;
        }

        HeapContainer(std::vector<DataType> init_data);

        void     Push(DataType data);
        void     Pop();
        DataType Top() const;
        size_t   Size() const
        {
            return current_size_;
        }
        bool Empty() const
        {
            return current_size_ == 0;
        }
        std ::string ToStr() const;

        static void UnitTest();

    private:
        size_t Parent(size_t index) const
        {
            if (index == 0)
            {
                return 0;
            }

            size_t parent = index % 2 == 0 ? static_cast<int>(index) - 2 : static_cast<int>(index) - 1;
            if (parent == 0)
            {
                return 0;
            }

            return parent / 2;
        }

        size_t LeftChild(size_t index) const
        {
            return (2 * index) + 1;
        }
        size_t RightChild(size_t index) const
        {
            return (2 * index) + 2;
        }
        std::vector<DataType> data_;
        size_t                current_size_;
        size_t                buffer_size_ = init_size;
        HeapType              heap_type_   = type;
    };

}  // namespace amit::common

#include "heap_container.inl"

#endif
