#ifndef SANDBOX_LEETCODE_TRIPLET_SUM_H
#define SANDBOX_LEETCODE_TRIPLET_SUM_H

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
#include <algorithm>

#include "sandbox/coding_problems/interface.h"

using namespace std;

class TripletSumProblem : public ICodingProblem
{
public:
    struct Triplet
    {
    public:
        Triplet() = delete;
        Triplet(int a, int b, int c)
        {
            vals.resize(3);
            vals[0] = a;
            vals[1] = b;
            vals[2] = c;
            sort(vals.begin(), vals.end());
        }

        bool operator==(const Triplet& other) const
        {
            if (vals[0] == other.vals[0] && vals[1] == other.vals[1] && vals[2] == other.vals[2])
            {
                return true;
            }

            return false;
        }

        bool operator<(const Triplet& other) const
        {
            if (vals[0] > other.vals[0] || vals[1] > other.vals[2] || vals[2] > other.vals[2])
            {
                return false;
            }

            if (*this == other)
            {
                return false;
            }

            return true;
        }

        vector<int> ToVec() const
        {
            return vals;
        }

    private:
        vector<int> vals;
    };

    void RunTests() override
    {
    }
};

#endif