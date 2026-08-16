#ifndef SANDBOX_CODING_PROBLEMS_OTHERS_COUNT_TRIANGLES_H
#define SANDBOX_CODING_PROBLEMS_OTHERS_COUNT_TRIANGLES_H

/**
 *
 * Counting Triangles
Given a list of N triangles with integer side lengths, determine how many
different triangles there are. Two triangles are considered to be the same if
they can both be placed on the plane such that their vertices occupy exactly the
same three points. Signature int countDistinctTriangles(ArrayList<Sides> arr) or
int countDistinctTrianges(int[][] arr)
Input
In some languages, arr is an Nx3 array where arr[i] is a length-3 array that
contains the side lengths of the ith triangle. In other languages, arr is a list
of structs/objects that each represent a single triangle with side lengths a, b,
and c. It's guaranteed that all triplets of side lengths represent real
triangles. All side lengths are in the range [1, 1,000,000,000] 1 <= N <=
1,000,000 Output Return the number of distinct triangles in the list. Example 1
arr = [[2, 2, 3], [3, 2, 2], [2, 5, 6]]
output = 2
The first two triangles are the same, so there are only 2 distinct triangles.
Example 2
arr = [[8, 4, 6], [100, 101, 102], [84, 93, 173]]
output = 3
All of these triangles are distinct.
Example 3
arr = [[5, 8, 9], [5, 9, 8], [9, 5, 8], [9, 8, 5], [8, 9, 5], [8, 5, 9]]
output = 1
All of these triangles are the same.
 */

#include <algorithm>
#include <cassert>

#include "sandbox/coding_problems/interface.h"

class DistinctTriangles : public ICodingProblem {
 public:
  struct Sides {
    Sides(long long a_, long long b_, long long c_) {
      a = a_;
      b = b_;
      c = c_;

      sort();
    }

    Sides(const Sides& other) {
      a = other.a;
      b = other.b;
      c = other.c;

      sort();
    }

    Sides& operator=(const Sides& other) {
      if (this != &other) {
        this->a = other.a;
        this->b = other.b;
        this->c = other.c;

        sort();
      }

      return *this;
    }

    Sides() = delete;
    long long a;
    long long b;
    long long c;

    long long GetSum() const { return a + b + c; }
    bool operator==(const Sides& other) const { return IsEqual(other); }

    bool operator!=(const Sides& other) const { return !IsEqual(other); }

    bool operator<(const Sides& other) const {
      long long sum_a = GetSum();
      long long sum_b = other.GetSum();

      if (sum_a < sum_b) {
        return true;
      } else if (sum_a > sum_b) {
        return false;
      }

      if (IsEqual(other)) {
        return false;
      }

      if (sorted_a > other.sorted_a) {
        return false;
      }

      if (sorted_b > other.sorted_b) {
        return false;
      }

      if (sorted_c > other.sorted_c) {
        return false;
      }

      return true;
    }

   private:
    bool IsEqual(const Sides& other) const {
      if (other.sorted_a == sorted_a && other.sorted_b == sorted_b &&
          other.sorted_c == sorted_c) {
        return true;
      }

      return false;
    }

    inline void swap(long long& a, long long& b) {
      long double temp = b;
      b = a;
      a = temp;
    }

    void sort() {
      if (sorted) {
        return;
      }

      sorted_a = a;
      sorted_b = b;
      sorted_c = c;

      if (sorted_a > sorted_b) {
        swap(sorted_a, sorted_b);
      }

      if (sorted_a > sorted_c) {
        swap(sorted_a, sorted_c);
      }

      if (sorted_b > sorted_c) {
        swap(sorted_b, sorted_c);
      }

      sorted = true;
    }

    long long sorted_arr[3];
    long long& sorted_a = sorted_arr[0];
    long long& sorted_b = sorted_arr[1];
    long long& sorted_c = sorted_arr[2];
    bool sorted = false;
  };

  int Count(vector<Sides> arr) {
    // Write your code here
    sort(arr.begin(), arr.end());
    uint32_t count = 1;
    for (size_t i = 1; i < arr.size(); i++) {
      if (arr[i] != arr[i - 1]) {
        count++;
      }
    }

    return count;
  }

  void RunTests() override {
    vector<Sides> arr_1 = {
        {7, 6, 5}, {5, 7, 6}, {8, 2, 9}, {2, 3, 4}, {2, 4, 3}};
    int expected_1 = 3;
    int output_1 = Count(arr_1);
    assert(expected_1 == output_1);

    vector<Sides> arr_2 = {{3, 4, 5}, {8, 8, 9}, {7, 7, 7}};
    int expected_2 = 3;
    int output_2 = Count(arr_2);
    assert(expected_2 == output_2);
  }
};

#endif