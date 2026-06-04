#include <bitset>
#include <functional>
#include <iostream>
#include <vector>
#include "decorator_pattern.h"

struct Data
{
    uint32_t InstanceID : 24;
    uint32_t InstanceMask : 8;
    uint32_t InstanceContributionToHitGroupIndex : 24;
    uint32_t Flags : 8;
};

// void* operator new(size_t size)
//{
//     std::cout << "Allocated " << size << std::endl;
//     return malloc(size);
// }
//
// void operator delete(void* ptr)
//{
//     free(ptr);
// }

struct DataForTestClassForOverloading
{
private:
    uint8_t m_data;
};

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

void TestMinFunction()
{
    S<int> s = {1, 2, 3, 4, 5};  // copy list-initialization
    s.append({6, -7, 8});         // list-initialization in function call

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

class TestClassForOverloading
{
public:
    DataForTestClassForOverloading GetData()
    {
        return m_data;
    }
    const DataForTestClassForOverloading& GetData() const
    {
        return GetData();
    }

private:
    DataForTestClassForOverloading m_data;
};

class TestLambdaExecution
{
public:
    TestLambdaExecution(size_t count)
    {
        m_lambdaList.resize(count);

        for (size_t it = 0; it < count; it++)
        {
            m_values.push_back(it + 1);
        }
    }

    void InitData(size_t index, std::function<uint32_t(uint32_t)> lambda)
    {
        m_lambdaList[index] = lambda;
    }

    void ExecuteLambda()
    {
        for (size_t it = 0; it < m_values.size(); it++)
        {
            m_values[it] = m_lambdaList[it](m_values[it]);
        }
    }

    static void Test()
    {
        size_t              count = 20;
        TestLambdaExecution testLambda(count);

        {
            uint32_t addMore = 1;

            for (size_t it = 0; it < count; it++, addMore++)
            {
                testLambda.InitData(it, [addMore](uint32_t inVal) -> uint32_t { return (inVal * 2) + addMore; });
            }
        }

        testLambda.ExecuteLambda();
    }

    void UpdateValues(uint32_t index, uint32_t factor)
    {
        m_values[index] = ((m_values[index] + factor) * factor) + 2301;
    }

    static void UseLambdaToUpdate()
    {
        size_t              count  = 200000;
        uint32_t            factor = 1;
        TestLambdaExecution testLambda_1(count);

        for (uint32_t it = 0; it < count; it++, factor++)
        {
            auto lambda = [&testLambda_1](uint32_t index, uint32_t factor) {
                testLambda_1.m_values[index] = ((testLambda_1.m_values[index] + factor) * factor) + 2301;
            };

            lambda(it, factor);
        }
    }

    static void UseFunctionToUpdate()
    {
        size_t              count  = 200000;
        uint32_t            factor = 1;
        TestLambdaExecution testLambda_2(count);
        for (uint32_t it = 0; it < count; it++, factor++)
        {
            testLambda_2.UpdateValues(it, factor);
        }
    }

private:
    std::vector<uint32_t>                                     m_values;
    std::vector<std::function<uint32_t(uint32_t anotherVal)>> m_lambdaList;
};

enum TestEnum
{
    One = 0x1,
    Two = 0x2
};

#include <iostream>
#include <utility>

struct Default
{
    int foo() const
    {
        return 1;
    }
    void foo_void()
    {
    }
};

struct NonDefault
{
    NonDefault() = delete;
    int foo() const
    {
        return 1;
    }
};

struct T
{
    enum
    {
        int_t,
        float_t
    } type;

    template <typename Integer, std::enable_if_t<std::is_integral<Integer>::value, bool> = true>
    T(Integer)
        : type(int_t)
    {
    }

    template <typename Floating, std::enable_if_t<std::is_floating_point<Floating>::value, bool> = true>
    T(Floating)
        : type(float_t)
    {
    }  // OK
};

#include <cstdarg>
#include <iostream>

void simple_printf(const char* fmt...)  // C-style "const char* fmt, ..." is also valid
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

void ExperimentVecRotation()
{
    std::vector<int> data = {1, 2, 3, 4, 5, 6, 7};
    
    std::rotate(data.begin(), data.begin() + 3, data.end());

    for (auto val : data)
    {
        std::cout << val << ' ';
    }
    std::cout << std::endl;
}

int main(int argcCount, char* argc[])
{
    ExperimentVecRotation();

    TestMinFunction();
    decorator::RunTest();
    simple_printf("dcff", 3, 'a', 1.999, 42.5);

    std::time_t todayTime;
    todayTime = time(&todayTime);

    tm*  localTime = std::localtime(&todayTime);
    char timeAsStr[60];

    size_t writtenSize = strftime(timeAsStr, 60, "%Y_%m_%d_%Z_%H_%M_%S", localTime);
    writtenSize;

    decltype(Default().foo()) n1 = 1;  // type of n1 is int

    constexpr bool void_type = std::is_void<decltype(Default().foo_void())>();
    //  decltype(NonDefault().foo()) n2 = n1;               // error: no default
    //  constructor
    decltype(std::declval<NonDefault>().foo()) n2 = n1;  // type of n2 is int
    std::cout << "n1 = " << n1 << '\n' << "n2 = " << n2 << '\n';

    for (uint32_t i = 0; i < 10; i++)
    {
        std::cout << i << std::endl;
    }

    for (uint32_t i = 0; i < 10; ++i)
    {
        std::cout << i << std::endl;
    }

    TestLambdaExecution::Test();
    // TestLambdaExecution::UseLambdaToUpdate();
    // TestLambdaExecution::UseFunctionToUpdate();

    // TestClassForOverloading test;
    // const DataForTestClassForOverloading temp = test.GetData();

    // uint32_t val = 0x20000100;

    // uint64_t val_64 = 0x0123456789abcdef;
    // char* val_64_ptr = (char*)&val_64;

    // uint32_t val_32_lsb = 0;
    // char* val_32_1_ptr = (char*)&val_32_lsb;

    // uint32_t val_32_msb = 0;
    // char* val_32_2_ptr = (char*)&val_32_msb;

    // uint8_t ptrIter = 0;
    // for (uint8_t i = 0; i < 4; i++) {
    //   val_32_1_ptr[i] = val_64_ptr[ptrIter];
    //   ptrIter++;
    // }

    // for (uint8_t i = 0; i < 4; i++) {
    //   val_32_2_ptr[i] = val_64_ptr[ptrIter];
    //   ptrIter++;
    // }

    // uint64_t make64 = 0;
    // make64 = val_32_msb;
    // make64 = make64 << 32;
    // make64 |= val_32_lsb;

    /*char* make64Ptr = (char*) & make64;
  ptrIter = 0;
  for (uint8_t i = 0; i < 4; i++)
  {
      make64Ptr[ptrIter] = val_32_1_ptr[i];
      ptrIter++;
  }

  for (uint8_t i = 0; i < 4; i++)
  {
      make64Ptr[ptrIter] = val_32_2_ptr[i];
      ptrIter++;
  }*/

    // val;
    // uint32_t testEnum = One | Two;

    // uint32_t buildFlags =
    //     D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_ALLOW_UPDATE |
    //     D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_ALLOW_COMPACTION |
    //     D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PERFORM_UPDATE;

    // std::cout << std::bitset<8>(buildFlags) << std::endl;

    // if (buildFlags &
    //     D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PERFORM_UPDATE) {
    //   uint32_t performUpdateMask =
    //       D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PERFORM_UPDATE;
    //   buildFlags ^= performUpdateMask;
    //   std::cout << std::bitset<8>(buildFlags) << std::endl;
    // }

    // std::string s1("0123456789");
    // Data* d1 = new Data();
    // Data d;
    // d.InstanceID = 10;
    // d.InstanceMask = 5;
    // d.InstanceContributionToHitGroupIndex = 1;
    // d.Flags = 0;

    // std::cout << "Sizeof data is " << sizeof(Data) << std::endl;
    // std::cout << std::bitset<32>(d.InstanceID) << std::endl
    //           << std::bitset<32>(d.InstanceMask);

    // uint32_t instanceId = d.InstanceID;
    // uint32_t instanceMask = d.InstanceMask;
    // std::cout << "Instance Val" << instanceId << std::endl;
    // std::cout << "Instance Mask" << instanceMask << std::endl;
    // instanceMask++;
    // instanceId++;

    // d.InstanceID = instanceId;
    // d.InstanceMask = instanceMask;
    // std::cout << std::bitset<32>(d.InstanceID) << std::endl
    //           << std::bitset<32>(d.InstanceMask);
}