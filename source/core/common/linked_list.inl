#ifndef CORE_COMMON_LINKED_LIST_INL
#define CORE_COMMON_LINKED_LIST_INL

#include <iostream>
#include <sstream>

// #include "linked_list.h"

template <typename DataType>
amit::common::LinkedList<DataType>::Node::Node()
    : data()
    , next(nullptr)
{
}

template <typename DataType>
typename amit::common::LinkedList<DataType>::Node* amit::common::LinkedList<DataType>::Data() const
{
    if (head_ && head_->next)
    {
        return head_->next;
    }

    return nullptr;
}

template <typename DataType>
amit::common::LinkedList<DataType>::LinkedList()
{
    head_  = new Node();
    count_ = 0;
}

template <typename DataType>
amit::common::LinkedList<DataType>::LinkedList(DataType data)
    : LinkedList()
{
    Node* new_node = new Node();
    new_node->data = data;

    head_->next = new_node;
    count_      = 1;
}

template <typename DataType>
amit::common::LinkedList<DataType>::~LinkedList<DataType>()
{
    while (head_)
    {
        Node* next_node = head_->next;
        delete head_;
        head_ = next_node;
    }

    count_ = 0;
    head_  = nullptr;
}

template <typename DataType>
bool amit::common::LinkedList<DataType>::Add(DataType data, int index)
{
    if (index != -1 && static_cast<size_t>(index) > count_)
    {
        throw std::out_of_range("Index out of range");
        return false;
    }

    Node* new_node = new Node();
    new_node->data = data;

    // index == 0 i.e. at head
    if (index == 0 || ((index == -1) && head_->next == nullptr))
    {
        new_node->next = head_->next;
        head_->next    = new_node;
    }
    else
    {
        size_t list_index_iter = 0;
        Node*  list_iter       = head_->next;

        while (list_iter && list_iter->next != nullptr)
        {
            if (list_index_iter == static_cast<size_t>(index) - 1)
            {
                new_node->next  = list_iter->next;
                list_iter->next = new_node;
                break;
            }
            list_iter = list_iter->next;
            list_index_iter++;
        }

        if (index == -1 || (static_cast<size_t>(index) - 1) == count_)
        {
            list_iter->next = new_node;
        }
    }

    count_++;
    return true;
}
template <typename DataType>
size_t amit::common::LinkedList<DataType>::Size() const
{
    return count_;
}

template <typename DataType>
DataType amit::common::LinkedList<DataType>::Remove(int index)
{
    return DataType();
}

template <typename DataType>
DataType amit::common::LinkedList<DataType>::operator[](size_t index) const
{
    if (index > count_)
    {
        throw std::out_of_range("Index out of range");
    }
    else
    {
        Node*  list_iter       = head_->next;
        size_t list_index_iter = 0;

        while (list_iter)
        {
            if (list_index_iter == index)
            {
                break;
            }
            list_iter = list_iter->next;
            list_index_iter++;
        }

        return list_iter->data;
    }
}
template <typename DataType>
DataType amit::common::LinkedList<DataType>::GetMiddle() const
{
    if (Node* middle_node = GetMiddleNode())
    {
        return middle_node->data;
    }

    return DataType();
}

template <typename DataType>
std::string amit::common::LinkedList<DataType>::ToStr() const
{
    std::stringstream ss;
    Node*             iter = head_->next;
    ss << "{";
    while (iter)
    {
        ss << iter->data << "->";
        iter = iter->next;
    }

    const std::string temp(ss.str());
    std::string       ret_str(temp.cbegin(), temp.cend() - 2);
    ret_str += '}';
    return ret_str;
}

template <typename DataType>
bool amit::common::LinkedList<DataType>::UnitTest()
{
    LinkedList<int> list;

    bool success = true;
    for (int i = 1; i < 7; i++)
    {
        success |= list.Add(i);
    }

    std::string out = list.ToStr();
    std::cout << out.c_str();

    std::cout << list.GetMiddle() << std::endl;

    list.Reverse();
    std::cout << list.ToStr();

    std::cout << list.GetMiddle() << std::endl;

    return true;
}
#endif
