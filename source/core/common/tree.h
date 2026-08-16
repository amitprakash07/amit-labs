#ifndef CORE_COMMON_TREE_H
#define CORE_COMMON_TREE_H

#include <cstdint>
#include <string>
#include <vector>

#include "doubly_linked_list.h"

namespace amit::common
{

    enum class TreeTraverseOrder
    {
        PreOrder,
        InOrder,
        PostOrder
    };

    template <class DataType, uint8_t Dimension, bool ArrayStorage>
    class TreeNode
    {
    public:
        TreeNode()
            : data()
        {
        }
        DataType data;

    protected:
        uint8_t dimension         = Dimension;
        bool    use_array_storage = ArrayStorage;
    };

    template <class DataType>
    class BinaryTreeNode : public TreeNode<DataType, 2, false>
    {
    public:
        BinaryTreeNode* left  = nullptr;
        BinaryTreeNode* right = nullptr;
    };

    template <class DataType>
    class BinaryTree
    {
    public:
        void Construct(std::vector<DataType> data);
        void Traverse(const BinaryTreeNode<DataType>* tree_node,
                      TreeTraverseOrder               order,
                      std::vector<DataType>&          out) const;

        const BinaryTreeNode<DataType>* Root() const
        {
            return root;
        }
        uint32_t FindDepth(BinaryTreeNode<DataType>* node_to_found);
        uint32_t FindDepth(DataType node_to_found);

        void        TraverseIteratively(const BinaryTreeNode<DataType>* tree_node,
                                        TreeTraverseOrder               order,
                                        std::vector<DataType>&          out) const;
        static void UnitTest();

    protected:
        static uint32_t FindDepthHelper(BinaryTreeNode<DataType>* node, BinaryTreeNode<DataType>* find, bool& is_found);
        static uint32_t FindDepthHelper(BinaryTreeNode<DataType>* node, DataType find, bool& is_found);
        BinaryTreeNode<DataType>* root = nullptr;
    };

    template <class DataType>
    class BinarySearchTree : public BinaryTree<DataType>
    {
    public:
        void                       Construct(std::vector<DataType> data);
        void                       Add(DataType data);
        DoublyLinkedList<DataType> Convert();

        static void UnitTest();

    private:
        void Add(BinaryTreeNode<DataType>* node, DataType data);
    };

}  // namespace amit::common

#include "tree.inl"

#endif
