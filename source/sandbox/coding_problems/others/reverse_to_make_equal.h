#ifndef SANDBOX_CODING_PROBLEMS_OTHERS_REVERSE_TO_MAKE_EQUAL
#define SANDBOX_CODING_PROBLEMS_OTHERS_REVERSE_TO_MAKE_EQUAL

#include <cassert>

#include "sandbox/coding_problems/interface.h"

// Add any helper functions you may need here


class AreTheyEqual : public ICodingProblem{
public:
  bool IsEqual(const vector<int>& array_a,
                           const vector<int>& array_b) {
    // Write your code here
    for (size_t i = 0; i < array_a.size(); i++) {
      if (array_a[i] != array_b[i]) {
        size_t b_iter = i + 1;
        while (b_iter < array_b.size() && array_a[i] != array_b[b_iter]) {
          b_iter++;
        }

        if (b_iter == array_b.size()) {
          return false;
        }

        size_t a_iter = i;
        for (size_t j = b_iter; j >= i; j--) {
          if (array_b[j] != array_a[a_iter]) {
            return false;
          }

          a_iter++;
        }

        i = a_iter;
      }
    }

    return true;
  }

    void RunTests() override
  {
    vector<int> array_a_1{1, 2, 3, 4};
    vector<int> array_b_1{1, 4, 3, 2};
    bool expected_1 = true;
    bool output_1 = IsEqual(array_a_1, array_b_1);
    assert(expected_1, output_1);

    vector<int> array_a_2{1, 2, 3, 4};
    vector<int> array_b_2{1, 4, 3, 3};
    bool expected_2 = false;
    bool output_2 = IsEqual(array_a_2, array_b_2);
    assert(expected_2, output_2);
  }
};

#endif