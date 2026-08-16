#ifndef CORE_COMMON_SORT_H
#define CORE_COMMON_SORT_H

#include <vector>

#include "defs.h"

namespace amit::common
{
    class Sort
    {
    public:
        template <typename T>
        static void MergeSort(T* o_List, int iLength, int (*SortFunc)(T i_A, T i_B) = nullptr);

        template <typename T>
        static void QuickSort(T* o_List, int iListlength, int (*SortFunc)(T i_A, T i_B) = nullptr);

        static void BubbleSort(std::vector<int>& array);

    private:
        template <typename T>
        static void InternalMergeSort(T* o_list, unsigned int iStart, unsigned int iEnd, int (*SortingFunc)(T i_A, T i_B) = nullptr);
        template <typename T>
        static void Merge(T* o_list, unsigned int iStart, unsigned int middle, unsigned int iEnd, int (*SortingFunc)(T i_A, T i_B) = nullptr);
    };
}  // namespace amit::common

#endif