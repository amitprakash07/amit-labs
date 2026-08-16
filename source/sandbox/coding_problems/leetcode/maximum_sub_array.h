#ifndef SANDBOX_LEETCODE_MAXIMUM_SUB_ARRAY_H
#define SANDBOX_LEETCODE_MAXIMUM_SUB_ARRAY_H

#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

#include "sandbox/coding_problems/interface.h"

using namespace std;

struct SubArray {
  bool operator>(const SubArray& other) const {
    return max_sum > other.max_sum;
  }

  size_t left_index;
  size_t right_index;
  int max_sum;
};

class MaxSubArrayFinder : public ICodingProblem {
 public:
  SubArray Find(vector<int> array, size_t low, size_t high) {
    if (low == high) {
      return {low, low, array[low]};
    }

    size_t mid = (high - low + 1) / 2;
    SubArray left = SubArraySum(array, low, mid);
    SubArray right = SubArraySum(array, mid + 1, high);
    SubArray cross = CrossSubArray(array, low, mid, high);

    if (left > right) {
      return left;
    }

    if (right > left) {
      return right;
    }

    return cross;
  }

 private:
  SubArray CrossSubArray(const vector<int>& array, size_t low, size_t mid,
                         size_t high) {
    int left_max_sum = INT_MIN, right_max_sum = INT_MIN;
    size_t left_max = 0, right_max = 0;

    int sum = 0;
    for (size_t i = 0; i <= mid; i++) {
      sum += array[i];
      if (sum > left_max_sum) {
        left_max_sum = sum;
        left_max = i;
      }
    }

    sum = 0;
    for (size_t i = mid + 1; i < high; i++) {
      sum += array[i];
      if (sum > right_max_sum) {
        right_max_sum = sum;
        right_max = i;
      }
    }

    return {left_max, right_max, left_max_sum + right_max_sum};
  }

  SubArray SubArraySum(const vector<int>& array, size_t start, size_t end) {
    int sum = 0;
    for (size_t i = start; i <= end; i++) {
      sum += array[i];
    }

    return {start, end, sum};
  }

  void RunTests() override {
    vector<int> nums = {100, 113, 110, 85,  105, 102, 86, 63, 81,
                        101, 94,  106, 101, 79,  94,  90, 97};
    vector<int> change = {0,  13, -3, -35, 20,  -3, -16, -23, 18,
                          20, -7, 12, -5,  -22, 15, -4,  7};

    MaxSubArrayFinder finder;
    // SubArray sub_array = finder.Find(nums, 0, nums.size());
    SubArray change_sub_array = finder.Find(change, 0, change.size() - 1);

    std::cout << change_sub_array.max_sum;
  }
};

#endif