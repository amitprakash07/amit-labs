
#include "cpu_rasterizer_pipeline.h"

#include "core/graphics/viewport.h"

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    amit::graphics::Viewport viewport({.value = 800}, {.value = 600});

    enum class DemoConfig : uint8_t
    {
        TriangleRasterizeInScreenSpace,
        TriangleRasterizeGraphicsPipeline,
        ObjFormatSceneRasterizeGraphicsPipeline
    } demo_config = DemoConfig::ObjFormatSceneRasterizeGraphicsPipeline;

    switch (demo_config)
    {
    case DemoConfig::TriangleRasterizeInScreenSpace:
        amit::cpu_rasterizer::DemoTriangleRasterizeInScreenSpace(viewport);
        break;
    case DemoConfig::TriangleRasterizeGraphicsPipeline:
        amit::cpu_rasterizer::DemoTriangleRasterizeGraphicsPipeline(viewport);
        break;
    case DemoConfig::ObjFormatSceneRasterizeGraphicsPipeline:
        amit::cpu_rasterizer::DemoObjFormatSceneRasterizeGraphicsPipeline(viewport);
        break;
    }

    return 0;
}
