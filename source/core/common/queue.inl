#ifndef CORE_COMMON_QUEUE_INL
#define CORE_COMMON_QUEUE_INL

#include <iostream>
#include <stdexcept>

#include "queue.h"
#include "to_str.h"

template <class DataType, uint64_t initial_size>
amit::common::Queue<DataType, initial_size>::Queue()
{
    storage_ = new (std::nothrow) DataType[initial_size];

    if (storage_)
    {
        empty_ = true;
        back_ = front_               = 0;
        max_container_element_count_ = initial_size;
    }
}

template <class DataType, uint64_t initial_size>
void amit::common::Queue<DataType, initial_size>::Push(const DataType& data)
{
    if (!empty_)
    {
        // Check we have empty space
        if (back_ >= front_)
        {
            if ((back_ + 1) >= max_container_element_count_)
            {
                if ((static_cast<int>(front_) - 1) > 0)
                {
                    back_ = 0;
                }
                else
                {
                    // resize
                    uint64_t  new_max_element_count = max_container_element_count_ * 2;
                    DataType* new_storage           = new DataType[new_max_element_count];
                    memset(new_storage, 0, sizeof(DataType) * new_max_element_count);
                    memcpy(new_storage, storage_, sizeof(DataType) * max_container_element_count_);
                    delete[] storage_;
                    storage_                     = new_storage;
                    back_                        = max_container_element_count_;
                    max_container_element_count_ = new_max_element_count;
                }
            }
            else
            {
                back_++;
            }
        }
        else if (front_ > back_)
        {
            if ((back_ + 1) >= front_)
            {
                // resize
                uint64_t  new_max_element_count = max_container_element_count_ * 2;
                DataType* new_storage           = new DataType[new_max_element_count];
                memset(new_storage, 0, sizeof(DataType) * new_max_element_count);
                memcpy(new_storage, storage_ + front_, sizeof(DataType) * (max_container_element_count_ - front_));
                memcpy(new_storage + front_, storage_, sizeof(DataType) * (back_ + 1));
                delete[] storage_;
                storage_                     = new_storage;
                back_                        = max_container_element_count_;
                front_                       = 0;
                max_container_element_count_ = new_max_element_count;
            }
            else
            {
                back_++;
            }
        }
    }
    else
    {
        empty_ = false;
    }

    storage_[back_] = data;
}

template <class DataType, uint64_t initial_size>
void amit::common::Queue<DataType, initial_size>::Pop()
{
    if (!empty_)
    {
        if (back_ == front_)
        {
            back_ = front_ = 0;
            empty_         = true;
        }
        else if (front_ + 1 >= max_container_element_count_)
        {
            front_ = 0;
        }
        else
        {
            front_++;
        }
    }
}

template <class DataType, uint64_t initial_size>
DataType amit::common::Queue<DataType, initial_size>::Front() const
{
    if (!empty_)
    {
        return storage_[front_];
    }

    return DataType();
}

template <class DataType, uint64_t initial_size>
DataType amit::common::Queue<DataType, initial_size>::Back() const
{
    if (!empty_)
    {
        return storage_[back_];
    }

    return DataType();
}

template <class DataType, uint64_t initial_size>
size_t amit::common::Queue<DataType, initial_size>::Size() const
{
    if (!empty_)
    {
        if (back_ >= front_)
        {
            return (back_ - front_ + 1);
        }

        return max_container_element_count_ - front_ + back_ + 1;
    }

    return 0;
}

template <class DataType, uint64_t initial_size>
bool amit::common::Queue<DataType, initial_size>::Empty() const
{
    return empty_;
}

template <class DataType, uint64_t initial_size>
std::string amit::common::Queue<DataType, initial_size>::Print() const
{
    std::string ret_string = {};
    if (!empty_)
    {
        if (back_ >= front_)
        {
            size_t iter = front_;
            while (iter <= back_)
            {
                ret_string += ToStr(storage_[iter]);
                ret_string += "\n";
                iter++;
            }
        }
        else
        {
            size_t iter = front_;
            while (iter < max_container_element_count_)
            {
                ret_string += ToStr(storage_[iter]);
                ret_string += "\n";
                iter++;
            }

            iter = 0;
            while (iter <= back_)
            {
                ret_string += ToStr(storage_[iter]);
                ret_string += "\n";
                iter++;
            }
        }
    }
    return ret_string;
}

template <class DataType, uint64_t initial_size>
void amit::common::Queue<DataType, initial_size>::UnitTest()
{
    Queue<int, 5> int_queue;
    int_queue.Push(5);
    std::cout << "Stack size " << int_queue.Size();
    std::cout << int_queue.Print();
    std::cout << "Stack size " << int_queue.Size();
    std::cout << "<<<<<<>>>>>>\n";
    int_queue.Pop();
    std::cout << "Stack size " << int_queue.Size();
    std::cout << int_queue.Print();
    std::cout << "<<<<<<>>>>>>\n";

    for (size_t it = 0; it < 10; it++)
    {
        int_queue.Push(it);
    }

    std::cout << "Stack size " << int_queue.Size();
    std::cout << int_queue.Print();
    std::cout << "<<<<<<>>>>>>\n";

    for (size_t it = 0; it < 5; it++)
    {
        int_queue.Pop();
    }
    std::cout << "Stack size " << int_queue.Size();
    std::cout << int_queue.Print();
    std::cout << "<<<<<<>>>>>>\n";

    for (size_t it = 10; it < 15; it++)
    {
        int_queue.Push(it);
    }
    std::cout << "Stack size " << int_queue.Size();
    std::cout << int_queue.Print();
    std::cout << "<<<<<<>>>>>>\n";

    int_queue.Push(20);
    std::cout << "Stack size " << int_queue.Size();
    std::cout << int_queue.Print();
    std::cout << "<<<<<<>>>>>>\n";

    std::cout << "Size of the queue = " << int_queue.Size();

    std::cout << int_queue.Print();
}

#endif
