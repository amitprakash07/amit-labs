#ifndef SANDBOX_CODING_PROBLEMS_OTHERS_PASSING_YEAR_BOOKS_H
#define SANDBOX_CODING_PROBLEMS_OTHERS_PASSING_YEAR_BOOKS_H

#include "sandbox/coding_problems/interface.h"

class PassingYearBooks : public ICodingProblem {
 public:
  vector<int> FindSignatureCounts(const vector<int>& arr) {
    // Write your code here
    vector<int> ret_sign_count;
    ret_sign_count.resize(arr.size());
    memset(ret_sign_count.data(), 0, sizeof(int) * ret_sign_count.size());
    vector<int> current_hold, previous_hold;
    // we know all with their own yearbook, to start will put in 0 for all to
    // get started
    current_hold.resize(arr.size() + 1);
    previous_hold.resize(arr.size() + 1);
    for (size_t i = 0; i < current_hold.size(); i++) {
      current_hold[i] = i;
      previous_hold[i] = i;
    }

    bool can_pass = true;
    bool first_pass = true;
    while (can_pass) {
      previous_hold = current_hold;
      memset(current_hold.data(), 0, sizeof(int) * current_hold.size());
      for (int i = 1; i <= arr.size(); i++) {
        if (first_pass) {
          ret_sign_count[previous_hold[i] - 1]++;
          current_hold[arr[i - 1]] = previous_hold[i];
        } else if (previous_hold[i] != i) {
          ret_sign_count[previous_hold[i] - 1]++;
          current_hold[arr[i - 1]] = previous_hold[i];
        } else {
          current_hold[i] = i;
        }
      }

      first_pass = false;
      can_pass = previous_hold != current_hold;
    }

    return ret_sign_count;
  }

  void RunTests() override {
    vector<int> arr_1{2, 1};
    vector<int> expected_1{2, 2};
    vector<int> output_1 = FindSignatureCounts(arr_1);
    assert(expected_1 == output_1, true);

    vector<int> arr_2{1, 2};
    vector<int> expected_2{1, 1};
    vector<int> output_2 = FindSignatureCounts(arr_2);
    assert(expected_2 == output_2, true);
  }
};

#endif