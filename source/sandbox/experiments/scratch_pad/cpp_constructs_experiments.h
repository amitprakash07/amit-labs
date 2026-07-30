#ifndef SANDBOX_EXPERIMENTS_SCRATCH_PAD_CPP_CONSTRUCTS_EXPERIMENTS_H
#define SANDBOX_EXPERIMENTS_SCRATCH_PAD_CPP_CONSTRUCTS_EXPERIMENTS_H

#include <cstdarg>
#include <iostream>
#include <vector>
#include <ctime>
#include <xutility>

namespace amit::sandbox_experiments
{
    template <class T>
    struct S
    {
        std::vector<T> v;

        S(std::initializer_list<T> l)
            : v(l)
        {
            std::cout << "constructed with a " << l.size() << "-element list\n";
        }

        void append(std::initializer_list<T> l)
        {
            v.insert(v.end(), l.begin(), l.end());
        }

        std::pair<const T*, std::size_t> c_arr() const
        {
            return {&v[0], v.size()};  // copy list-initialization in return statement
                                       // this is NOT a use of std::initializer_list
        }
    };

    inline void TestMinFunction()
    {
        S<int> s = {1, 2, 3, 4, 5};  // copy list-initialization
        s.append({6, -7, 8});        // list-initialization in function call

        int min_val =
            std::min({*s.v.begin(), *(s.v.end() - 1), *(s.v.end() - 3), *(s.v.end() - 2)});  // s.v.begin(), s.v.end());
        std::cout << "Min value is " << min_val;

        std::cout << "The vector now has " << s.c_arr().second << " ints:\n";
        for (auto n : s.v)
            std::cout << n << ' ';
        std::cout << '\n';

        std::cout << "Range-for over brace-init-list: \n";
        for (int x : {-1, -2, -3})  // the rule for auto makes this ranged-for work
            std::cout << x << ' ';
        std::cout << '\n';
    }

    inline void simple_printf(const char* fmt...)  // C-style "const char* fmt, ..." is also valid
    {
        va_list args;
        va_start(args, fmt);

        while (*fmt != '\0')
        {
            if (*fmt == 'd')
            {
                int i = va_arg(args, int);
                std::cout << i << '\n';
            }
            else if (*fmt == 'c')
            {
                // note automatic conversion to integral type
                int c = va_arg(args, int);
                std::cout << static_cast<char>(c) << '\n';
            }
            else if (*fmt == 'f')
            {
                double d = va_arg(args, double);
                std::cout << d << '\n';
            }
            ++fmt;
        }

        va_end(args);
    }

    inline void ExperimentVecRotation()
    {
        std::vector<int> data = {1, 2, 3, 4, 5, 6, 7};

        std::rotate(data.begin(), data.begin() + 3, data.end());

        for (auto val : data)
        {
            std::cout << val << ' ';
        }

        std::cout << "std::endl";
    }

    inline void ExperimentTimeHandling()
    {
        std::time_t todayTime;
        todayTime = time(&todayTime);

        tm*  localTime = std::localtime(&todayTime);
        char timeAsStr[60];

        size_t writtenSize = strftime(timeAsStr, 60, "%Y_%m_%d_%Z_%H_%M_%S", localTime);
        writtenSize;
    }
}

#endif