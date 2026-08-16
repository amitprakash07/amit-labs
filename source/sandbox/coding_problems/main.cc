#include <functional>
#include <map>
#include <set>
#include <string>

#include "core/common/heap_container.h"
#include "core/common/linked_list.h"
#include "core/common/maths_util.h"
#include "core/common/queue.h"
#include "core/common/sort.h"
#include "core/common/stack.h"
#include "core/common/string_util.h"
#include "core/common/to_str.h"
#include "core/common/tree.h"
#include "core/common/util.h"
#include "sandbox/coding_problems/interface.h"
#include "sandbox/coding_problems/list.h"

int subarraySum(vector<int>& nums, int k) {
  unordered_map<int, int> counts;
  counts[0] = 1;
  int ans = 0, curr = 0;

  for (int num : nums) {
    curr += num;
    ans += counts[curr - k];
    counts[curr]++;
  }

  return ans;
}

int numberOfSubarrays(vector<int>& nums, int k) {
  unordered_map<int, int> counts;
  counts[0] = 1;
  int ans = 0, curr = 0;

  for (int num : nums) {
    curr += num % 2;
    ans += counts[curr - k];
    counts[curr] += 1;
  }

  return ans;
}

int findMaxLength(std::vector<int> nums) {
  std::map<int, int> count_map = {{0, -1}};

  int max_len = 0, count = 0;

  for (int i = 0; i < nums.size(); i++) {
    count = count + (nums[i] == 1 ? 1 : -1);
    if (count_map.contains(count)) {
      max_len =
          max_len < (i - count_map[count]) ? (i - count_map[count]) : max_len;
    } else {
      count_map[count] = i;
    }
  }
  return max_len;
}

vector<vector<int>> findWinners(vector<vector<int>>& matches) {
  std::set<int> winners;
  map<int, int> loosers;

  std::vector<std::vector<int>> ret_vector;
  ret_vector.push_back(std::vector<int>());
  ret_vector.push_back(std::vector<int>());

  for (size_t it = 0; it < matches.size(); it++) {
    winners.insert(matches[it][0]);
    loosers[matches[it][1]]++;
  }

  for (std::set<int>::const_iterator it = winners.cbegin();
       it != winners.cend(); it++) {
    if (loosers.find(*it) == loosers.end()) {
      ret_vector[0].push_back(*it);
    }
  }

  for (std::map<int, int>::const_iterator it = loosers.cbegin();
       it != loosers.cend(); it++) {
    if (it->second == 1) {
      ret_vector[1].push_back(it->first);
    }
  }

  return ret_vector;
}

int lengthOfLongestSubstring(const string& str) {
  size_t right = 0, left = 0;
  std::map<char, int> char_last_index;
  size_t max_length = 0;

  while (right < str.size()) {
    if (char_last_index.contains(str[right])) {
      left = max(left, static_cast<size_t>(char_last_index[str[right]]));
    }

    char_last_index[str[right]] = static_cast<int>(right) + 1;
    max_length = max(max_length, right - left + 1);
    right++;
  }

  return static_cast<int>(max_length);
}

int main(int argCount, char* argc[]) {
  INSTANTIATE_PROBLEM(DistinctTriangles);
  INSTANTIATE_PROBLEM(MinLengthSubString);
  INSTANTIATE_PROBLEM(ZipLinkList);
  INSTANTIATE_PROBLEM(RotationalCipher);
  INSTANTIATE_PROBLEM(PassingYearBooks);
  INSTANTIATE_PROBLEM(AreTheyEqual);
  INSTANTIATE_PROBLEM(RemoveDuplicateSortedArray);  
  INSTANTIATE_PROBLEM(MaxSubArrayFinder);
  
  lengthOfLongestSubstring("tmmzuxt");
  std::vector<int> array = {0, 0, 1, 0, 0, 0, 1, 1};
  int max_length = findMaxLength(array);
  numberOfSubarrays(array, 2);
  std::vector<int> prefix_array = amit::common::util::PrefixArray(array);
  std::cout << amit::common::util::ToStr(prefix_array);
  INSTANTIATE_PROBLEM(UniqueEmailAddressProblem);
  INSTANTIATE_PROBLEM(SortedSquareArray);
  INSTANTIATE_PROBLEM(MinStartValueProblem);
  INSTANTIATE_PROBLEM(KRadiusAveragesProblem);
}
