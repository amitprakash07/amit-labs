#ifndef CORE_COMMON_STACK_INL
#define CORE_COMMON_STACK_INL

#include <iostream>
#include <stdexcept>

#include "stack.h"
#include "to_str.h"

template <class DataType, uint32_t initial_size>
amit::common::Stack<DataType, initial_size>::Stack()
{
    if (initial_size < UINT32_MAX)
    {
        storage_ = new (std::nothrow) DataType(initial_size);
        if (storage_)
        {
            memset(storage_, 0, sizeof(DataType) * initial_size);
            max_container_element_count_ = initial_size;
            top_index_                   = 0;
            empty_                       = true;
        }
    }
}

template <class DataType, uint32_t initial_size>
void amit::common::Stack<DataType, initial_size>::Push(DataType data)
{
    if (!empty_)
    {
        if ((top_index_ + 1) >= max_container_element_count_)
        {
            ResizeAndMove((top_index_ + 1) * 2);
        }
    }

    if (storage_ && max_container_element_count_ > top_index_)
    {
        if (!empty_)
        {
            top_index_++;
        }
        else
        {
            empty_ = false;
        }

        storage_[top_index_] = data;
    }
}

template <class DataType, uint32_t initial_size>
void amit::common::Stack<DataType, initial_size>::Pop()
{
    if (!empty_)
    {
        top_index_--;
        if (top_index_ == 0)
        {
            empty_ = true;
        }
    }
}

template <class DataType, uint32_t initial_size>
DataType amit::common::Stack<DataType, initial_size>::Top()
{
    if (!empty_)
    {
        return storage_[top_index_];
    }

    throw std::invalid_argument("Empty Stack");
}

template <class DataType, uint32_t initial_size>
uint32_t amit::common::Stack<DataType, initial_size>::Size() const
{
    if (empty_)
    {
        return 0;
    }

    return top_index_ + 1;
}

template <class DataType, uint32_t initial_size>
bool amit::common::Stack<DataType, initial_size>::Empty() const
{
    return empty_;
}

template <class DataType, uint32_t initial_size>
std::string amit::common::Stack<DataType, initial_size>::Print() const
{
    std::string ret_string = {};
    int         iter       = top_index_;
    while (iter >= 0)
    {
        ret_string += ToStr(storage_[iter]);
        ret_string += "\n";
        iter--;
    }

    return ret_string;
}

template <class DataType, uint32_t initial_size>
void amit::common::Stack<DataType, initial_size>::UnitTest()
{
    Stack<int> stack1;
    for (int i = 0; i < 100; i = i + 10)
    {
        stack1.Push(i);
    }

    std::cout << stack1.Print();

    for (int i = 0; i < 100; i = i + 10)
    {
        stack1.Pop();
        std::cout << stack1.Print();
    }
}

template <class DataType, uint32_t initial_size>
void amit::common::Stack<DataType, initial_size>::ResizeAndMove(uint32_t new_size)
{
    DataType* new_storage = new (std::nothrow) DataType[new_size];
    if (new_storage)
    {
        memset(new_storage, 0, sizeof(DataType) * new_size);
        memcpy(new_storage, storage_, sizeof(DataType) * max_container_element_count_);
        delete[] storage_;
        storage_                     = new_storage;
        max_container_element_count_ = new_size;
    }
}

#endif