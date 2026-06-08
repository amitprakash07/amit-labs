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
        std::vector<std::uint32_t>                indices;
    };

    inline std::vector<RenderPrimitiveTriangle<LocalSpace>> ConvertMeshDataToRenderPrimitives(const MeshData& mesh_data)
    {
        std::vector<RenderPrimitiveTriangle<LocalSpace>> render_primitives;
        render_primitives.reserve(mesh_data.indices.size() / 3);

        for (size_t i = 0; i < mesh_data.indices.size(); i += 3)
        {
            const auto& v0 = mesh_data.vertices[mesh_data.indices[i]];
            const auto& v1 = mesh_data.vertices[mesh_data.indices[i + 1]];
            const auto& v2 = mesh_data.vertices[mesh_data.indices[i + 2]];

            render_primitives.push_back(RenderPrimitiveTriangle<LocalSpace>(v0, v1, v2));
        }

        return render_primitives;
    }

    inline std::vector<RenderPrimitiveTriangle<LocalSpace>> ConvertMeshIndexRangeToRenderPrimitives(
        const MeshData& mesh_data,
        std::uint32_t   first_index,
        std::uint32_t   index_count)
    {
        std::vector<RenderPrimitiveTriangle<LocalSpace>> render_primitives;
        render_primitives.reserve(index_count / 3);

        for (std::uint32_t index_offset = first_index; index_offset + 2 < first_index + index_count; index_offset += 3)
        {
            const auto& v0 = mesh_data.vertices[mesh_data.indices[index_offset]];
            const auto& v1 = mesh_data.vertices[mesh_data.indices[index_offset + 1]];
            const auto& v2 = mesh_data.vertices[mesh_data.indices[index_offset + 2]];

            render_primitives.push_back(RenderPrimitiveTriangle<LocalSpace>(v0, v1, v2));
        }

        return render_primitives;
    }

}  // namespace amit::graphics

#endif
