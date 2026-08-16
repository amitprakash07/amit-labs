#ifndef CORE_ASSET_LOADING_OBJ_LOADER_H_
#define CORE_ASSET_LOADING_OBJ_LOADER_H_

#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

namespace amit::assets
{
    struct ObjPosition
    {
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;
    };

    struct ObjTextureCoordinate
    {
        float u = 0.0f;
        float v = 0.0f;
    };

    struct ObjColor
    {
        float r = 1.0f;
        float g = 1.0f;
        float b = 1.0f;
    };

    // Per-corner vertex data after triangulation. Color comes from explicit OBJ vertex RGB when
    // present; otherwise from the face material's diffuse (Kd) in the companion .mtl file.
    struct ObjVertex
    {
        ObjPosition          position;
        ObjTextureCoordinate texture_coordinate;
        ObjColor             color;
    };

    // One OBJ object/group (`g` tag), mapped to a contiguous slice of the global index buffer.
    struct ObjGroup
    {
        std::uint32_t id = 0;
        std::string   name;
        int           material_id = -1;
        std::uint32_t first_index  = 0;
        std::uint32_t index_count   = 0;
    };

    struct ObjMeshData
    {
        std::vector<ObjVertex>     vertices;
        std::vector<std::uint32_t> indices;
        std::vector<std::string>   material_names;
        std::vector<ObjGroup>      groups;
    };

    std::optional<ObjMeshData> LoadObjMesh(const std::filesystem::path& file_path);

}  // namespace amit::assets

#endif
