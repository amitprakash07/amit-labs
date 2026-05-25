
#include "cpu_rasterizer_pipeline.h"

#include "core/graphics/viewport.h"

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    amit::graphics::Viewport viewport({.value = 800}, {.value = 600});

    amit::cpu_rasterizer::RasterizeGraphicsPipeline(viewport);

    return 0;
}
