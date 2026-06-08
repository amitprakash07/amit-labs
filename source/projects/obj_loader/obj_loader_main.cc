#include <filesystem>
#include <iostream>
#include <optional>

#include "core/asset_loading/obj_loader.h"

int main(int argc, char* argv[])
{
    const std::filesystem::path obj_path =
        argc > 1 ? std::filesystem::path{argv[1]} : std::filesystem::path{"data/assets/models/textured_quad.obj"};

    const std::optional<amit::asset_loading::ObjMeshData> mesh_data = amit::asset_loading::LoadObjMesh(obj_path);
    if (!mesh_data.has_value())
    {
        std::cerr << "Failed to load OBJ mesh: " << obj_path.string() << '\n';
        return 1;
    }

    std::cout << "Loaded OBJ mesh: " << obj_path.string() << '\n';
    std::cout << "Vertex count: " << mesh_data->vertices.size() << '\n';
    std::cout << "Index count: " << mesh_data->indices.size() << '\n';
    std::cout << "Triangle count: " << mesh_data->indices.size() / 3 << '\n';
    std::cout << "Material count: " << mesh_data->material_names.size() << '\n';
    std::cout << "Group count: " << mesh_data->groups.size() << '\n';

    for (const amit::asset_loading::ObjGroup& group : mesh_data->groups)
    {
        std::string material_label = "none";
        if (group.material_id >= 0 &&
            static_cast<std::size_t>(group.material_id) < mesh_data->material_names.size())
        {
            material_label = mesh_data->material_names[static_cast<std::size_t>(group.material_id)];
        }

        std::cout << "  [" << group.id << "] " << group.name << " material=\"" << material_label
                  << "\" first_index=" << group.first_index << " index_count=" << group.index_count
                  << " triangles=" << (group.index_count / 3) << '\n';
    }

    if (!mesh_data->vertices.empty())
    {
        const amit::asset_loading::ObjVertex& first_vertex = mesh_data->vertices.front();
        std::cout << "First vertex position: " << first_vertex.position.x << ", " << first_vertex.position.y << ", "
                  << first_vertex.position.z << '\n';
        std::cout << "First vertex uv: " << first_vertex.texture_coordinate.u << ", "
                  << first_vertex.texture_coordinate.v << '\n';
        std::cout << "First vertex color: " << first_vertex.color.r << ", " << first_vertex.color.g << ", "
                  << first_vertex.color.b << '\n';
    }

    return 0;
}
