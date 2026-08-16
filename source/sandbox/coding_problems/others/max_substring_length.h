#ifndef SANDBOX_CODING_PROBLEMS_OTHERS_MAX_SUBSTRING_LENGTH
#define SANDBOX_CODING_PROBLEMS_OTHERS_MAX_SUBSTRING_LENGTH

#include <cassert>
#include <string>
#include <unordered_map>

#include "sandbox/coding_problems/interface.h"
// Add any extra import statements you may need here

using namespace std;

// Add any helper functions you may need here

class MinLengthSubString : public ICodingProblem {
  int Find(string s, string t) {
    size_t min = s.size();
    size_t max = 0;

    unordered_map<char, size_t> found_index_map;

    for (char ch : t) {
      size_t start_pos = 0;
      if (found_index_map.find(ch) != found_index_map.end()) {
        start_pos = found_index_map[ch] + 1;

        if (start_pos >= s.size()) {
          return -1;
        }
      }

      size_t pos = s.find(ch, start_pos);

      if (pos != std::string::npos) {
        found_index_map[ch] = pos;

        if (pos < min) {
          min = pos;
        }

        if (pos > max) {
          max = pos;
        }
      } else {
        return -1;
      }
    }

    return (max - min) + 1;
  }
  void RunTests() override {
    string s_1 = "dcbefebce";
    string t_1 = "fd";
    int expected_1 = 5;
    int output_1 = Find(s_1, t_1);
    assert(expected_1 == output_1);

    string s_2 = "bfbeadbcbcbfeaaeefcddcccbbbfaaafdbebedddf";
    string t_2 = "cbccfafebccdccebdd";
    int expected_2 = -1;
    int output_2 = Find(s_2, t_2);
    assert(expected_2 == output_2);
  }
};

#endif