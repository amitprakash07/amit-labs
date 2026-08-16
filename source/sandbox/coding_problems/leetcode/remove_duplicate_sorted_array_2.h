#ifndef SANDBOX_LEETCODE_REMOVE_DUPLICATE_SORTED_ARRAY_2_H
#define SANDBOX_LEETCODE_REMOVE_DUPLICATE_SORTED_ARRAY_2_H

/*
nums = {1,2,2,2,2,4,5,7,7}, length  = 9
nums = {1,2,2,4,5,7,7}, k= 7
*/

#include <vector>
#include <iostream>

#include "sandbox/coding_problems/interface.h"

class RemoveDuplicateSortedArray : public ICodingProblem
{
    int RemoveDuplicateSortedArrayNewLength(std::vector<int>& nums) const
    {
        size_t insert_iter = 0;
        if(!nums.empty())
        {
            insert_iter = 2;
            for(size_t i = 2; i < nums.size(); i++)
            {
                if(nums[i] == nums[insert_iter - 1] && nums[i] == nums[insert_iter - 2])
                {
                    continue;
                }
                nums[insert_iter] = nums[i];
                insert_iter++;
            }
        }

        return static_cast<int>(insert_iter);
    }

    void RunTests() override
    {
        std::vector<int> nums = {1,2,2,2,2,4,5,7,7};
        std::cout << RemoveDuplicateSortedArrayNewLength(nums);
    }
};

#endif