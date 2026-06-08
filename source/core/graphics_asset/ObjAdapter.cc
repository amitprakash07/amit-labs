#include "ObjAdapter.h"

#include <cstdint>
#include <stdexcept>

#include "core/maths/geometry/point.h"

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

const amit::graphics_asset::ObjAdapter::ImportRecord& amit::graphics_asset::ObjAdapter::GetImportRecord(
    ImportHandle handle) const
{
    const auto import_record = imports_.find(handle.value);
    if (import_record == imports_.end())
    {
        throw std::out_of_range("graphics_asset::ObjAdapter: invalid ImportHandle");
    }

    return import_record->second;
}

amit::graphics::MeshData amit::graphics_asset::ObjAdapter::ConvertToRenderMesh(
    const asset_loading::ObjMeshData& source_mesh)
{
    graphics::MeshData render_mesh;
    render_mesh.vertices.reserve(source_mesh.vertices.size());
    render_mesh.indices.reserve(source_mesh.indices.size());

    for (const asset_loading::ObjVertex& source_vertex : source_mesh.vertices)
    {
        render_mesh.vertices.push_back(graphics::VertexAttributes<graphics::LocalSpace>{
            .position = graphics::LocalSpace::Position{geometry::Point3D{source_vertex.position.x,
                                                                         source_vertex.position.y,
                                                                         source_vertex.position.z}},
            .color    = ToRgb8(source_vertex.color),
            .uv       = {source_vertex.texture_coordinate.u, source_vertex.texture_coordinate.v}});
    }

    render_mesh.indices = source_mesh.indices;

    return render_mesh;
}

std::optional<amit::graphics_asset::ImportHandle> amit::graphics_asset::ObjAdapter::ImportObj(
    const std::filesystem::path& file_path)
{
    const std::optional<asset_loading::ObjMeshData> source_mesh = asset_loading::LoadObjMesh(file_path);
    if (!source_mesh.has_value())
    {
        return std::nullopt;
    }

    const ImportHandle handle{.value = next_import_id_++};
    imports_.emplace(handle.value,
                     ImportRecord{.source = *source_mesh, .render = ConvertToRenderMesh(*source_mesh)});

    return handle;
}

const amit::graphics::MeshData& amit::graphics_asset::ObjAdapter::GetRenderMeshData(ImportHandle handle) const
{
    return GetImportRecord(handle).render;
}

const amit::asset_loading::ObjMeshData& amit::graphics_asset::ObjAdapter::GetObjMeshData(ImportHandle handle) const
{
    return GetImportRecord(handle).source;
}
