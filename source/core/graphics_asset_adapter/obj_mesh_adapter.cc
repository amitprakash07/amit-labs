#include "obj_mesh_adapter.h"

#include <cstdint>

namespace
{
    std::uint8_t ToRgbChannel(float channel_value)
    {
        if (channel_value < 0.0f)
        {
            return 0;
        }

        if (channel_value > 1.0f)
        {
            return 255;
        }

        return static_cast<std::uint8_t>(channel_value * 255.0f);
    }

    amit::graphics::Rgb8 ToRgb8(const amit::asset_loading::ObjColor& color)
    {
        return {ToRgbChannel(color.r), ToRgbChannel(color.g), ToRgbChannel(color.b)};
    }

}  // namespace

amit::graphics::MeshData amit::graphics_asset_adapter::ConvertObjMeshToGraphicsMesh(
    const asset_loading::ObjMeshData& obj_mesh_data)
{
    graphics::MeshData graphics_mesh_data;
    graphics_mesh_data.vertices.reserve(obj_mesh_data.vertices.size());
    graphics_mesh_data.indices.reserve(obj_mesh_data.indices.size());

    for (const asset_loading::ObjVertex& obj_vertex : obj_mesh_data.vertices)
    {
        graphics_mesh_data.vertices.push_back(graphics::VertexAttributes<graphics::LocalSpace>{
            .position = graphics::LocalSpace::Position{
                geometry::Point3D{obj_vertex.position.x, obj_vertex.position.y, obj_vertex.position.z}},
            .color    = ToRgb8(obj_vertex.color),
            .uv       = {obj_vertex.texture_coordinate.u, obj_vertex.texture_coordinate.v}});
    }

    graphics_mesh_data.indices = obj_mesh_data.indices;

    return graphics_mesh_data;
}

std::optional<amit::graphics::MeshData> amit::graphics_asset_adapter::LoadObjMeshAsGraphicsMesh(
    const std::filesystem::path& file_path)
{
    const std::optional<asset_loading::ObjMeshData> obj_mesh_data = asset_loading::LoadObjMesh(file_path);
    if (!obj_mesh_data.has_value())
    {
        return std::nullopt;
    }

    return ConvertObjMeshToGraphicsMesh(*obj_mesh_data);
}
