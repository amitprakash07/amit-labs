#ifndef CORE_GRAPHICS_ASSET_ADAPTER_H_
#define CORE_GRAPHICS_ASSET_ADAPTER_H_

#include <cstdint>
#include <filesystem>
#include <optional>
#include <unordered_map>

#include "core/asset_loading/obj_loader.h"
#include "core/graphics_common/mesh_data.h"

namespace amit::assets
{
    struct ImportHandle
    {
        std::uint64_t value = 0;

        explicit operator bool() const
        {
            return value != 0;
        }

        friend bool operator==(const ImportHandle& lhs, const ImportHandle& rhs) = default;
    };

    class ObjAdapter
    {
    public:
        std::optional<ImportHandle> ImportObj(const std::filesystem::path& file_path);

        const graphics::MeshData&         GetRenderMeshData(ImportHandle handle) const;
        const assets::ObjMeshData& GetObjMeshData(ImportHandle handle) const;

    private:
        struct ImportRecord
        {
            assets::ObjMeshData source;
            graphics::MeshData         render;
        };

        static graphics::MeshData ConvertToRenderMesh(const assets::ObjMeshData& source_mesh);

        const ImportRecord& GetImportRecord(ImportHandle handle) const;

        std::uint64_t                                   next_import_id_ = 1;
        std::unordered_map<std::uint64_t, ImportRecord> imports_;
    };

}  // namespace amit::assets

#endif
