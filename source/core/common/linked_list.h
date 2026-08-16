#ifndef CORE_COMMON_LINKED_LIST_H
#define CORE_COMMON_LINKED_LIST_H

#include <string>
#include <vector>

namespace amit::common
{
    template <typename DataType>
    class LinkedList
    {
    public:
        struct Node
        {
            Node();
            DataType data;
            Node*    next = nullptr;
        };

        LinkedList();
        LinkedList(DataType data);

        LinkedList(std::vector<DataType> init_data)
            : LinkedList()
        {
            bool success = true;
            for (DataType data : init_data)
            {
                success |= Add(data);
            }
        }

        LinkedList(LinkedList&)            = delete;
        LinkedList(LinkedList&&)           = delete;
        LinkedList operator=(LinkedList&)  = delete;
        LinkedList operator=(LinkedList&&) = delete;

        ~LinkedList();

        [[nodiscard]] bool Add(DataType data, int index = -1);
        size_t             Size() const;
        DataType           Remove(int index = -1);
        DataType           operator[](size_t index) const;
        DataType           GetMiddle() const;

        Node* GetMiddleNode() const
        {
            if (head_ && head_->next)
            {
                Node* dummy = head_->next;  // start of link list

                Node* slow_ptr = dummy;
                Node* fast_ptr = dummy;

                while (fast_ptr && fast_ptr->next)
                {
                    slow_ptr = slow_ptr->next;
                    fast_ptr = fast_ptr->next->next;
                }

                return slow_ptr;
            }

            return nullptr;
        }

        void GetLastTwo(Node* from, Node*& second_last, Node*& last_node) const
        {
            second_last     = nullptr;
            last_node       = from;
            Node* next_node = from->next;

            while (next_node)
            {
                second_last = last_node;
                last_node   = next_node;
                next_node   = next_node->next;
            }
        }

        void Reverse()
        {
            if (head_ && head_->next)
            {
                Node* prev    = nullptr;
                Node* current = head_->next;

                while (current)
                {
                    Node* next    = current->next;
                    current->next = prev;
                    prev          = current;
                    current       = next;
                }

                head_->next = prev;
            }
        }

        Node*       Data() const;
        std::string ToStr() const;
        static bool UnitTest();

    private:
        // sentinel pointers
        Node*  head_  = nullptr;
        size_t count_ = 0;
    };

}  // namespace amit::common

#include "linked_list.inl"

#endif
