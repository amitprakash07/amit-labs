#include "decorator_pattern.h"
#include "cpp_constructs_experiments.h"
#include "bit_manipulation_experiments.h"

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

int main(int argcCount, char* argc[])
{
    amit::sandbox_experiments::ExperimentBitManipulation();
    amit::sandbox_experiments::ExperimentVecRotation();
    amit::sandbox_experiments::TestMinFunction();

    decorator::RunTest();

    amit::sandbox_experiments::simple_printf("dcff", 3, 'a', 1.999, 42.5);
}