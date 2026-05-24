#ifndef CORE_GRAPHICS_COMMON_MESH_DATA_H_
#define CORE_GRAPHICS_COMMON_MESH_DATA_H_

#include <cstdint>
#include <vector>

#include "render_primitives.h"

namespace amit::graphics
{
    struct MeshData
    {
        std::vector<VertexAttributes<LocalSpace>> vertices;
        std::vector<std::uint32_t>                 indices;
    };

}  // namespace amit::graphics

#endif
