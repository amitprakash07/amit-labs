
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
        ObjQuadModelRasterizeGraphicsPipeline,
        ObjCornellBoxModelSceneRasterizeGraphicsPipeline
    } demo_config = DemoConfig::ObjCornellBoxModelSceneRasterizeGraphicsPipeline;

    switch (demo_config)
    {
    case DemoConfig::TriangleRasterizeInScreenSpace:
        amit::cpu_rasterizer::DemoTriangleRasterizeInScreenSpace(viewport);
        break;
    case DemoConfig::TriangleRasterizeGraphicsPipeline:
        amit::cpu_rasterizer::DemoTriangleRasterizeGraphicsPipeline(viewport);
        break;
    case DemoConfig::ObjQuadModelRasterizeGraphicsPipeline:
        amit::cpu_rasterizer::DemoQuadObjModelSceneRasterizeGraphicsPipeline(viewport);
        break;
    case DemoConfig::ObjCornellBoxModelSceneRasterizeGraphicsPipeline:
        amit::cpu_rasterizer::DemoCornellBoxObjModelSceneRasterizeGraphicsPipeline(viewport);
        break;
    }

    return 0;
}
