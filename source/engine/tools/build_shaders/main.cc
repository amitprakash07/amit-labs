#include "gpu/shader/shader_handler.h"

int main(char* args[], int arg_count)
{
    amit::gpu::ShaderHandler::Instance()->CompileDefaultShaders();
}