#include <Windows.h>

#include "common/tree.h"
#include "common/heap_container.h"
#include "common/linked_list.h"
#include "common/maths_util.h"
#include "common/sort.h"
#include "common/stack.h"
#include "common/string_util.h"
#include "common/util.h"
#include "gpu/gpu_factory.h"
#include "gpu/gpu_device.h"
#include "gpu/shader/shader_handler.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    MessageBox(nullptr, "Testing the Win Main", "Testing", MB_OK);

    {
        amit::gpu::GpuFactory gpu;

        if (gpu.Initialize())
        {
            amit::gpu::GpuDevice* device = gpu.GetDevice();
            if (device)
            {
                device->UnitTest();
            }
        }
    }

    {
        //amit::gpu::ShaderHandler::Init();
        amit::gpu::ShaderHandler::Instance()->CompileDefaultShaders();
    }

    {
        amit::common::HeapContainer<int>::UnitTest();
        amit::common::BinarySearchTree<int>::UnitTest();
    }

    {
        // Bubble sort
        std::vector<int> data = {5, 8, 9, 2, 6, 7, 3, 1, 4};
        std::cout << amit::common::util::ToStr(data);
        amit::common::Sort::BubbleSort(data);
        std::cout << amit::common::util::ToStr(data);
    }

    {
        std::cout << amit::common::maths::GetHCF(119, 544);
        amit::common::BinaryTree<int>::UnitTest();
        amit::common::Queue<int>::UnitTest();
        amit::common::Stack<int>::UnitTest();
    }

    {
        char*       to_char_array = amit::common::ToStr(32);
        std::string temp_string(to_char_array);
        std::cout << temp_string.c_str();
        std::cout << std::string(amit::common::ToStr(-6748)).c_str();
    }

    {
        amit::common::LinkedList<int>::UnitTest();
    }

    {
        std::cout << amit::common::SimplifyPath("/.../a/../b/c/../d/./");
        std::cout << amit::common::SimplifyPath("/../");
        std::cout << amit::common::SimplifyPath("/home/user/Documents/../Pictures");
        std::cout << amit::common::RemoveDuplicates("abbaca");
    }
}
