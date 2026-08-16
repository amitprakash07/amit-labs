#ifndef SANDBOX_CODING_PROBLEMS_OTHERS_ROTATIONAL_CIPHER_H
#define SANDBOX_CODING_PROBLEMS_OTHERS_ROTATIONAL_CIPHER_H

#include <cassert>
#include <string>

#include "sandbox/coding_problems/interface.h"

using namespace std;

class RotationalCipher : public ICodingProblem {
  string Get(const string& input, int rotationFactor) {
    // Write your code here
    string ret_string = {};

    if (rotationFactor > 0) {
      uint32_t alpha_factor = rotationFactor % 26;
      uint32_t digit_factor = rotationFactor % 10;

      for (char ch : input) {
        uint8_t rotated_char = ch;
        if ('A' <= ch && ch <= 'Z') {
          rotated_char =
              static_cast<char>(static_cast<uint8_t>(ch) + rotationFactor);

          if (rotated_char > 'Z') {
            rotated_char =
                static_cast<char>('A' + (rotated_char - 'Z') % 26);
          }
        } else if ('a' <= ch && ch <= 'z') {
          rotated_char =
              static_cast<char>(static_cast<uint8_t>(ch) + alpha_factor);

          if (rotated_char > 'z') {
            rotated_char = static_cast<char>(
                'a' + (static_cast<uint8_t>(rotated_char) - 'z') - 1);
          }
        } else if ('0' <= ch && ch <= '9') {
          rotated_char =
              static_cast<char>(static_cast<uint8_t>(ch) + digit_factor);

          if (rotated_char > '9') {
            rotated_char = static_cast<char>(
                '0' + (static_cast<uint8_t>(rotated_char) - '9') - 1);
          }
        }

        ret_string.push_back(static_cast<char>(rotated_char));
      }
    }

    return ret_string;
  }

  void RunTests() override {
    string input_1 = "All-convoYs-9-be:Alert1.";
    int rotationFactor_1 = 4;
    string expected_1 = "Epp-gsrzsCw-3-fi:Epivx5.";
    string output_1 = Get(input_1, rotationFactor_1);
    assert(expected_1 == output_1);

    string input_2 = "abcdZXYzxy-999.@";
    int rotationFactor_2 = 200;
    string expected_2 = "stuvRPQrpq-999.@";
    string output_2 = Get(input_2, rotationFactor_2);
    assert(expected_2 == output_2);
  }
};

#endif