#ifndef SANDBOX_LEETCODE_SORTED_SQUARE_ARRAY_H
#define SANDBOX_LEETCODE_SORTED_SQUARE_ARRAY_H

/*
 * Given an integer array nums sorted in non-decreasing order, 
 return an array of the squares of each number sorted in non-decreasing order. 

Example 1:

Input: nums = [-4,-1,0,3,10]
Output: [0,1,9,16,100]
Explanation: After squaring, the array becomes [16,1,0,9,100].
After sorting, it becomes [0,1,9,16,100].
 */

#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

#include "sandbox/coding_problems/interface.h"

using namespace std;

class SortedSquareArray : public ICodingProblem {
public:
    vector<int> sortedSquares(vector<int>& nums) {
        vector<int> sorted_square;
        sorted_square.resize(nums.size());

        // {-4, -1, 0, 3, 10}

        uint32_t left = 0;
        uint32_t right = nums.size() - 1;

        int to_be_squared = 0;
        for (int iter = nums.size() - 1; iter >= 0; iter--)
        {
            if (abs(nums[left]) > abs(nums[right]))
            {
                to_be_squared = abs(nums[left]);
                left++;
            }
            else
            {
                to_be_squared = abs(nums[right]);
                right--;
            }

            sorted_square[iter] = to_be_squared * to_be_squared;
        }

        return sorted_square;
    }

    void RunTests() override {
        std::vector<int> nums = { -7, -3, 2, 3, 11 };

        vector<int> sorted = sortedSquares(nums);
    }
};

#endif