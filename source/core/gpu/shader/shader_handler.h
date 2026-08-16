#ifndef CORE_GPU_SHADER_SHADER_HANDLER_H_
#define CORE_GPU_SHADER_SHADER_HANDLER_H_

#include <string>
#include <nlohmann/json.hpp>

#include <atlbase.h>

#include "dxcapi.h"

#include "common/defs.h"
#include "platform/platform_objects.h"

namespace amit::gpu
{
    class ShaderHandler
    {
    public:
        static bool           Init();
        static ShaderHandler* Instance();
        static bool           DenInit();

        bool CompileDefaultShaders();

    private:
        struct ShaderDesc
        {
            ShaderDesc(const nlohmann::json& json_shader_dec_obj);
            ShaderDesc(ShaderDesc&& other) noexcept
                : shader_bin_out_file_(std::move(other.shader_bin_out_file_))
                , shader_authored_file_(std::move(other.shader_authored_file_))
            {
                name_              = std::move(other.name_);
                relative_path_     = std::move(other.relative_path_);
                entry_point_       = std::move(other.entry_point_);
                profile_           = std::move(other.profile_);
                macros_            = std::move(other.macros_);
                enable_debug_info_ = other.enable_debug_info_;
            }

            std::string_view GetOutPath() const
            {
                return shader_bin_out_file_.GetPath().AsCharArray();
            }

            const File& GetAuthoredShaderFile() const
            {
                return shader_authored_file_;
            }

            const File& GetShaderOutFile() const
            {
                return shader_bin_out_file_;
            }

            std::string name_;
            std::string relative_path_;
            std::string entry_point_;
            std::string profile_;
            std::string macros_;
            bool        enable_debug_info_;

        private:
            File shader_bin_out_file_;
            File shader_authored_file_;
        };

        bool CompileDefaultShadersUsingDxc();
        bool CompileDefaultShadersUsingDxcApi();
        bool InstanceInit();
        bool PopulateShaderDesc();

        static ShaderHandler*   shader_handler_singleton_ptr_;
        const std::string       kCoreShaderDir               = CORE_STRINGIZE_MACRO(CORE_SHADER_DIR);
        const std::string       kCoreShaderAssetDescFileName = "asset_shaders.json";
        bool                    use_dxc_for_compilation_       = false;
        IDxcUtils*              dxc_util_ptr_                  = nullptr;
        IDxcCompiler3*          dxc_compiler_3_ptr_            = nullptr;
        std::vector<ShaderDesc> default_shader_list_;
    };
}  // namespace amit::gpu

#endif
