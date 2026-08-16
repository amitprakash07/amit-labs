#ifndef SANDBOX_LEETCODE_K_RADIUS_AVERAGES_H
#define SANDBOX_LEETCODE_K_RADIUS_AVERAGES_H

/*
You are given a 0-indexed array nums of n integers, and an integer k.

The k-radius average for a subarray of nums centered at some index i with the
radius k is the average of all elements in nums between the indices i - k and i
+ k (inclusive). If there are less than k elements before or after the index i,
then the k-radius average is -1.

Build and return an array avgs of length n where avgs[i] is the k-radius average
for the subarray centered at index i.

The average of x elements is the sum of the x elements divided by x, using
integer division. The integer division truncates toward zero, which means losing
its fractional part.

For example, the average of four elements 2, 3, 1, and 5 is (2 + 3 + 1 + 5) / 4
= 11 / 4 = 2.75, which truncates to 2.

Input: nums = [7,4,3,9,1,8,5,2,6], k = 3
Output: [-1,-1,-1,5,4,4,-1,-1,-1]
Explanation:
- avg[0], avg[1], and avg[2] are -1 because there are less than k elements
before each index.
- The sum of the subarray centered at index 3 with radius 3 is: 7 + 4 + 3 + 9 +
1 + 8 + 5 = 37. Using integer division, avg[3] = 37 / 7 = 5.
- For the subarray centered at index 4, avg[4] = (4 + 3 + 9 + 1 + 8 + 5 + 2) / 7
= 4.
- For the subarray centered at index 5, avg[5] = (3 + 9 + 1 + 8 + 5 + 2 + 6) / 7
= 4.
- avg[6], avg[7], and avg[8] are -1 because there are less than k elements after
each index. Example 2:

Input: nums = [100000], k = 0
Output: [100000]
Explanation:
- The sum of the subarray centered at index 0 with radius 0 is: 100000.
  avg[0] = 100000 / 1 = 100000.
Example 3:

Input: nums = [8], k = 100000
Output: [-1]
Explanation:
- avg[0] is -1 because there are less than k elements before and after index 0.


Constraints:

n == nums.length
1 <= n <= 105
0 <= nums[i], k <= 105
 */

#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

#include "sandbox/coding_problems/interface.h"

using namespace std;

class KRadiusAveragesProblem : public ICodingProblem {
 public:
  [[nodiscard]] vector<int> GetAverages(vector<int>& nums, int k) {
    // First calculate the prefix array
    vector<int64_t> prefix_array;
    prefix_array.resize(nums.size());
    prefix_array[0] = nums[0];

    vector<int> average_array;
    average_array.resize(nums.size());
    average_array[0] = static_cast<int64_t>(nums[0]);

    for (size_t it = 1; it < nums.size(); it++) {
      average_array[it] = nums[it];
      prefix_array[it] = static_cast<int64_t>(nums[it]) + prefix_array[it - 1];
    }

    if (k > 0) {
      int64_t avg = 0;
      for (int i = 0; i < nums.size(); i++) {
        avg = -1;
        int64_t sum = 0;
        if (((i - k) >= 0) && ((i + k) < nums.size())) {
          avg = 0;
          if ((i - k) == 0) {
            sum = prefix_array[i + k];
          } else {
            sum = prefix_array[i + k] - prefix_array[i - k - 1];
          }

          avg = sum / ((2 * k) + 1);
        }

        average_array[i] = static_cast<int>(avg);
      }
    }

    return average_array;
  }

  void RunTests() override {
    vector<int> nums = {7, 4, 3, 9, 1, 8, 5, 2, 6};
    int k = 3;

    auto result = GetAverages(nums, k);
  }
};

#endif