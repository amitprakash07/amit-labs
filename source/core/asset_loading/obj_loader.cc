#include "obj_loader.h"

#define TINYOBJLOADER_DISABLE_FAST_FLOAT
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include <cstddef>
#include <string>

namespace
{
    bool HasVertexColor(const tinyobj::attrib_t& attrib, std::size_t vertex_index)
    {
        const std::size_t color_index = 3 * vertex_index;
        return color_index + 2 < attrib.colors.size();
    }

    amit::assets::ObjColor GetVertexColor(const tinyobj::attrib_t& attrib, std::size_t vertex_index)
    {
        const std::size_t color_index = 3 * vertex_index;
        if (color_index + 2 >= attrib.colors.size())
        {
            return {1.0f, 1.0f, 1.0f};
        }

        return {attrib.colors[color_index], attrib.colors[color_index + 1], attrib.colors[color_index + 2]};
    }

    amit::assets::ObjColor GetMaterialDiffuseColor(const std::vector<tinyobj::material_t>& materials,
                                                            int                                     material_id)
    {
        if (material_id < 0 || material_id >= static_cast<int>(materials.size()))
        {
            return {1.0f, 1.0f, 1.0f};
        }

        const tinyobj::material_t& material = materials[static_cast<std::size_t>(material_id)];
        return {static_cast<float>(material.diffuse[0]),
                static_cast<float>(material.diffuse[1]),
                static_cast<float>(material.diffuse[2])};
    }

    amit::assets::ObjColor ResolveVertexColor(const tinyobj::attrib_t&              attrib,
                                                     std::size_t                           vertex_index,
                                                     const std::vector<tinyobj::material_t>& materials,
                                                     int                                   material_id)
    {
        if (HasVertexColor(attrib, vertex_index))
        {
            return GetVertexColor(attrib, vertex_index);
        }

        return GetMaterialDiffuseColor(materials, material_id);
    }

    amit::assets::ObjTextureCoordinate GetTextureCoordinate(const tinyobj::attrib_t& attrib,
                                                                   int                     texture_coordinate_index)
    {
        if (texture_coordinate_index < 0)
        {
            return {};
        }

        const std::size_t texcoord_index = 2 * static_cast<std::size_t>(texture_coordinate_index);
        if (texcoord_index + 1 >= attrib.texcoords.size())
        {
            return {};
        }

        return {attrib.texcoords[texcoord_index], attrib.texcoords[texcoord_index + 1]};
    }

    std::optional<amit::assets::ObjVertex> MakeVertex(const tinyobj::attrib_t&              attrib,
                                                             const tinyobj::index_t&               index,
                                                             const std::vector<tinyobj::material_t>& materials,
                                                             int                                   material_id)
    {
        if (index.vertex_index < 0)
        {
            return std::nullopt;
        }

        const std::size_t vertex_index   = static_cast<std::size_t>(index.vertex_index);
        const std::size_t position_index = 3 * vertex_index;

        if (position_index + 2 >= attrib.vertices.size())
        {
            return std::nullopt;
        }

        return amit::assets::ObjVertex{
            .position           = {attrib.vertices[position_index],
                                   attrib.vertices[position_index + 1],
                                   attrib.vertices[position_index + 2]},
            .texture_coordinate = GetTextureCoordinate(attrib, index.texcoord_index),
            .color              = ResolveVertexColor(attrib, vertex_index, materials, material_id)};
    }

    std::string ResolveGroupName(const tinyobj::shape_t&               shape,
                                 int                                   group_material_id,
                                 const std::vector<tinyobj::material_t>& materials)
    {
        if (!shape.name.empty())
        {
            return shape.name;
        }

        if (group_material_id >= 0 && group_material_id < static_cast<int>(materials.size()))
        {
            return materials[static_cast<std::size_t>(group_material_id)].name;
        }

        return "unnamed";
    }

}  // namespace

std::optional<amit::assets::ObjMeshData> amit::assets::LoadObjMesh(const std::filesystem::path& file_path)
{
    tinyobj::ObjReaderConfig reader_config;
    reader_config.triangulate  = true;
    // When false, tinyobj drops default white vertex colors unless every vertex has explicit RGB in the OBJ.
    // That lets .mtl Kd drive color for assets like the Cornell Box.
    reader_config.vertex_color = false;

    tinyobj::ObjReader reader;
    if (!reader.ParseFromFile(file_path.string(), reader_config))
    {
        return std::nullopt;
    }

    const tinyobj::attrib_t&               attrib    = reader.GetAttrib();
    const std::vector<tinyobj::shape_t>&   shapes    = reader.GetShapes();
    const std::vector<tinyobj::material_t> materials = reader.GetMaterials();

    ObjMeshData mesh_data;

    mesh_data.material_names.reserve(materials.size());
    for (const tinyobj::material_t& material : materials)
    {
        mesh_data.material_names.push_back(material.name);
    }

    for (const tinyobj::shape_t& shape : shapes)
    {
        const tinyobj::mesh_t& mesh = shape.mesh;

        if (mesh.num_face_vertices.size() != mesh.material_ids.size())
        {
            return std::nullopt;
        }

        const std::uint32_t group_first_index = static_cast<std::uint32_t>(mesh_data.indices.size());
        int                 group_material_id = -1;

        std::size_t index_offset = 0;
        for (std::size_t face_index = 0; face_index < mesh.num_face_vertices.size(); ++face_index)
        {
            const unsigned int face_vertex_count = mesh.num_face_vertices[face_index];
            const int          material_id       = mesh.material_ids[face_index];

            if (group_material_id < 0)
            {
                group_material_id = material_id;
            }

            if (index_offset + face_vertex_count > mesh.indices.size())
            {
                return std::nullopt;
            }

            for (unsigned int vertex_in_face = 0; vertex_in_face < face_vertex_count; ++vertex_in_face)
            {
                const std::optional<ObjVertex> vertex =
                    MakeVertex(attrib, mesh.indices[index_offset + vertex_in_face], materials, material_id);
                if (!vertex.has_value())
                {
                    return std::nullopt;
                }

                mesh_data.indices.push_back(static_cast<std::uint32_t>(mesh_data.vertices.size()));
                mesh_data.vertices.push_back(*vertex);
            }

            index_offset += face_vertex_count;
        }

        const std::uint32_t group_index_count =
            static_cast<std::uint32_t>(mesh_data.indices.size()) - group_first_index;
        if (group_index_count == 0)
        {
            continue;
        }

        mesh_data.groups.push_back(ObjGroup{.id           = static_cast<std::uint32_t>(mesh_data.groups.size()),
                                            .name         = ResolveGroupName(shape, group_material_id, materials),
                                            .material_id  = group_material_id,
                                            .first_index  = group_first_index,
                                            .index_count  = group_index_count});
    }

    return mesh_data;
}
