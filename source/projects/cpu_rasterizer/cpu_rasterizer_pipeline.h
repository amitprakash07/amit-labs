#ifndef PROJECTS_CPU_RASTERIZER_CPU_RASTERIZER_PIPELINE_H_
#define PROJECTS_CPU_RASTERIZER_CPU_RASTERIZER_PIPELINE_H_

#include "core/graphics/viewport.h"

namespace amit::cpu_rasterizer
{
    void DemoTriangleRasterizeInScreenSpace(const graphics::Viewport& viewport);

    void DemoTriangleRasterizeGraphicsPipeline(const graphics::Viewport& viewport);

    void DemoObjFormatSceneRasterizeGraphicsPipeline(const graphics::Viewport& viewport);

}  // namespace amit::cpu_rasterizer

#endif
