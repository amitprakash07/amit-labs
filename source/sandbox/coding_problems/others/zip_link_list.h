#ifndef SANDBOX_CODING_PROBLEMS_OTHERS_ZIP_LINK_LIST_H
#define SANDBOX_CODING_PROBLEMS_OTHERS_ZIP_LINK_LIST_H

#include <functional>
#include <vector>

#include "core/common/linked_list.h"
#include "sandbox/coding_problems/interface.h"

/*
 * Given a singly linked list <l0, l1, 12, ... , ln- 1> , define the "zip" of
 the list to be <l1, ln-1, l1, ln-i, . .. >. e.g. List Before Zipping:
 l0->l1->l2->....ln-1 After Zipping: l0->ln-1->l1->ln-2->l3->ln-3....
 */

class ZipLinkList : public ICodingProblem {
 public:
  void RunTests() override {
    amit::common::LinkedList<uint32_t> init_data(
        std::vector<uint32_t>({1, 2, 3, 4, 5, 6, 7}));

    std::cout << init_data.ToStr();

    amit::common::LinkedList<uint32_t>::Node* current_pair_start = init_data.Data();
    amit::common::LinkedList<uint32_t>::Node* middle_node = init_data.GetMiddleNode();
    amit::common::LinkedList<uint32_t>::Node* last_node = nullptr;
    amit::common::LinkedList<uint32_t>::Node* second_last_node = nullptr;

    while (current_pair_start && current_pair_start->next &&
           current_pair_start != middle_node) {
      init_data.GetLastTwo(current_pair_start, second_last_node, last_node);
      second_last_node->next = nullptr;
      amit::common::LinkedList<uint32_t>::Node* pair_next = current_pair_start->next;
      current_pair_start->next = last_node;
      last_node->next = pair_next;
      current_pair_start = pair_next;
    }

    std::cout << init_data.ToStr();
  }
};

#endif