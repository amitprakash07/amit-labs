#ifndef SANDBOX_LEETCODE_UNIQUE_EMAIL_ADDRESS_H
#define SANDBOX_LEETCODE_UNIQUE_EMAIL_ADDRESS_H

/*
 * Every valid email consists of a local name and a domain name, separated by
the '@' sign. Besides lowercase letters, the email may contain one or more '.'
or '+'.

For example, in "alice@leetcode.com", "alice" is the local name, and
"leetcode.com" is the domain name. If you add periods '.' between some
characters in the local name part of an email address, mail sent there will be
forwarded to the same address without dots in the local name. Note that this
rule does not apply to domain names.

For example, "alice.z@leetcode.com" and "alicez@leetcode.com" forward to the
same email address. If you add a plus '+' in the local name, everything after
the first plus sign will be ignored. This allows certain emails to be filtered.
Note that this rule does not apply to domain names.

For example, "m.y+name@email.com" will be forwarded to "my@email.com".
It is possible to use both of these rules at the same time.

Given an array of strings emails where we send one email to each emails[i],
return the number of different addresses that actually receive mails.
 */

#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

#include "sandbox/coding_problems/interface.h"

class UniqueEmailAddressProblem : public ICodingProblem {
 public:
  std::string GetValidEmail(std::string email) {
    std::string validEmail;
    bool domainName = false;
    bool addChar = true;
    for (const char ch : email) {
      if (ch == '.' && !domainName) {
        continue;
      }

      if (ch == '+' && !domainName) {
        addChar = false;
      }

      if (ch == '@' && !domainName) {
        domainName = true;
        if (!addChar) {
          addChar = true;
        }
      }

      if (addChar) {
        validEmail.push_back(ch);
      }
    }

    return validEmail;
  }

  int GetUniqueEmailCount(std::vector<std::string> emails) {
    std::unordered_set<std::string> emailHashList;

    for (std::string email : emails) {
      email = GetValidEmail(email);

      emailHashList.insert(email);
    }

    return emailHashList.size();
  }

  void RunTests() override {
    std::vector<std::string> emails = {"test.email+alex@leetcode.com",
                                       "test.e.mail+bob.cathy@leetcode.com",
                                       "testemail+david@lee.tcode.com"};

    std::cout << "Unique email count " << GetUniqueEmailCount(emails);
  }
};

#endif