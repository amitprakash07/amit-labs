#ifndef SANDBOX_CODING_PROBLEMS_OTHERS_REVENUE_MILESTONES_H
#define SANDBOX_CODING_PROBLEMS_OTHERS_REVENUE_MILESTONES_H

/*
 * Revenue Milestones
We keep track of the revenue Facebook makes every day, and we want to know on
what days Facebook hits certain revenue milestones. Given an array of the
revenue on each day, and an array of milestones Facebook wants to reach, return
an array containing the days on which Facebook reached every milestone.
Signature
int[] getMilestoneDays(int[] revenues, int[] milestones)
Input
revenues is a length-N array representing how much revenue FB made on each day
(from day 1 to day N). milestones is a length-K array of total revenue
milestones. Output Return a length-K array where K_i is the day on which FB
first had milestones[i] total revenue. If the milestone is never met, return -1.
Example
revenues = [10, 20, 30, 40, 50, 60, 70, 80, 90, 100]
milestones = [100, 200, 500]
output = [4, 6, 10]
Explanation
On days 4, 5, and 6, FB has total revenue of $100, $150, and $210 respectively.
Day 6 is the first time that FB has >= $200 of total revenue.
 */

#include <vector>

#include "sandbox/coding_problems/interface.h"

class RevenueMilestones : public ICodingProblem {
 public:
  vector<int> Find(const vector<int>& revenues, const vector<int>& milestones) {
    vector<int> answer = {};

    using PrefixSum = int;
    vector<PrefixSum> prefix_sum;
    prefix_sum.resize(revenues.size());

    int milestone_iter = 0;
    for (size_t i = 0; i < revenues.size(); i++) {
      prefix_sum[i] = revenues[i] + (i > 0 ? prefix_sum[i - 1] : 0);
    }

    for (int milestone : milestones) {
      answer.push_back(FindClosest(prefix_sum, milestone) + 1);
    }

    return answer;
  }

  void RunTests() override {}

 private:
  int FindClosest(const vector<int>& values, int value) {
    int index = -1;

    size_t left = 0, right = values.size() - 1;

    while (left < right && (right - left) > 1) {
      size_t middle = (((right - left) + 1) / 2) + left;

      if (values[middle] == value) {
        return middle;
      } else if (values[middle] > value) {
        right = middle;
      } else if (values[middle] < value) {
        left = middle;
      }
    }

    if (value > values[right]) {
      return -1;
    }

    return right;
  }
};

#endif