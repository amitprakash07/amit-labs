
#include <iostream>
#include <filesystem>
#include "platform/win32/win32_includes.h"

#include "shader_handler.h"

#include "nlohmann/json.hpp"
#include "platform/platform_objects.h"
#include "platform/platform_systems.h"

amit::gpu::ShaderHandler* amit::gpu::ShaderHandler::shader_handler_singleton_ptr_ = nullptr;

std::wstring amit::common::ToWideString(const std::string& in)
{
    return std::wstring{in.cbegin(), in.cend()};
}

bool amit::gpu::ShaderHandler::Init()
{
    if (shader_handler_singleton_ptr_ == nullptr)
    {
        shader_handler_singleton_ptr_ = new (std::nothrow) ShaderHandler();
    }

    bool success = shader_handler_singleton_ptr_ != nullptr && shader_handler_singleton_ptr_->InstanceInit();
    return success;
}

amit::gpu::ShaderHandler* amit::gpu::ShaderHandler::Instance()
{
    if (Init())
    {
        return shader_handler_singleton_ptr_;
    }

    return nullptr;
}

bool amit::gpu::ShaderHandler::DenInit()
{
    delete shader_handler_singleton_ptr_;
    shader_handler_singleton_ptr_ = nullptr;
    return true;
}

bool amit::gpu::ShaderHandler::CompileDefaultShaders()
{
    if (PopulateShaderDesc())
    {
        if (use_dxc_for_compilation_)
        {
            return CompileDefaultShadersUsingDxc();
        }

        return CompileDefaultShadersUsingDxcApi();
    }

    return false;
}

amit::gpu::ShaderHandler::ShaderDesc::ShaderDesc(const nlohmann::json& json_shader_dec_obj)
    : shader_bin_out_file_(FilePath(""))
    , shader_authored_file_(FilePath(""))
{
    name_              = json_shader_dec_obj["name"];
    relative_path_     = json_shader_dec_obj["file"];
    entry_point_       = json_shader_dec_obj["entry_point"];
    profile_           = json_shader_dec_obj["profile"];
    macros_            = json_shader_dec_obj["macros"];
    enable_debug_info_ = json_shader_dec_obj["enable_debug_info"];

    shader_authored_file_ = File(FilePath(ShaderHandler::Instance()->kCoreShaderDir, relative_path_));

    FilePath current_module = FileSystem::GetCurrentModulePath();
    shader_bin_out_file_ =
        File(FilePath(std::string(current_module.ParentDirectory()),
                      std::string(shader_authored_file_.GetMetaData().GetFileNameWithoutExtension()) + ".out"));
}

bool amit::gpu::ShaderHandler::CompileDefaultShadersUsingDxc()
{
    bool              success = false;
    std::stringstream ss;

    for (const auto& it : default_shader_list_)
    {
        const ShaderDesc& shader_desc = it;
        const File&       shader_file = shader_desc.GetAuthoredShaderFile();
        if (shader_file.DoesExist())
        {
            ss.str("");
            ss.clear();
            ss << "dxc.exe";
            ss << " -T " << shader_desc.profile_;
            ss << " -E " << shader_desc.entry_point_;

            if (!shader_desc.macros_.empty())
            {
                ss << " -D " << shader_desc.macros_;
            }

            ss << " -Fo " << shader_desc.GetOutPath();
            ss << " " << shader_file.GetPath().AsCharArray();
            std::string cmd                = ss.str();
            int         shader_execute_cmd = system(cmd.c_str());

            if (shader_execute_cmd != 0)
            {
                std::cout << "Something is wrong\n";
            }
            success = true;
        }
    }

    return success;
}

bool amit::gpu::ShaderHandler::CompileDefaultShadersUsingDxcApi()
{
    if (dxc_compiler_3_ptr_ != nullptr && dxc_util_ptr_ != nullptr)
    {
        IDxcIncludeHandler* dxc_default_include_handler_ptr = nullptr;
        HRESULT             op_result = dxc_util_ptr_->CreateDefaultIncludeHandler(&dxc_default_include_handler_ptr);

        if (dxc_default_include_handler_ptr != nullptr)
        {
            std::vector<std::wstring> args;
            for (const ShaderDesc& shader_desc : default_shader_list_)
            {
                args.emplace_back(L"-T");
                args.emplace_back(amit::common::ToWideString(shader_desc.profile_));
                args.emplace_back(L"-E");
                args.emplace_back(amit::common::ToWideString(shader_desc.entry_point_));

                if (!shader_desc.macros_.empty())
                {
                    args.emplace_back(L"-D");
                    args.emplace_back(amit::common::ToWideString(shader_desc.macros_));
                }

                args.emplace_back(L"-Fo");
                args.emplace_back(amit::common::ToWideString(std::string(shader_desc.GetOutPath())));

                IDxcBlobEncoding* source_blob = nullptr;
                dxc_util_ptr_->LoadFile(
                    amit::common::ToWideString(shader_desc.GetAuthoredShaderFile().GetPath().AsCharArray())
                        .c_str(),
                    nullptr,
                    &source_blob);

                if (source_blob)
                {
                    DxcBuffer dxc_source_buffer{
                        source_blob->GetBufferPointer(), source_blob->GetBufferSize(), DXC_CP_ACP};

                    std::vector<LPCWSTR> w_arg_buffer;
                    for (std::wstring& w_arg : args)
                    {
                        w_arg_buffer.push_back(w_arg.c_str());
                    }

                    IDxcResult* compiled_result_ptr = nullptr;
                    op_result                       = dxc_compiler_3_ptr_->Compile(&dxc_source_buffer,
                                                             w_arg_buffer.data(),
                                                             static_cast<uint32_t>(args.size()),
                                                             dxc_default_include_handler_ptr,
                                                             __uuidof(IDxcResult),
                                                             reinterpret_cast<void**>(&compiled_result_ptr));

                    if (SUCCEEDED(op_result) && compiled_result_ptr != nullptr)
                    {
                        if (compiled_result_ptr->HasOutput(DXC_OUT_ERRORS))
                        {
                            IDxcBlobUtf8* errors_ptr = nullptr;
                            op_result                = compiled_result_ptr->GetOutput(
                                DXC_OUT_ERRORS, __uuidof(IDxcBlobUtf8), reinterpret_cast<void**>(&errors_ptr), nullptr);

                            if (SUCCEEDED(op_result))
                            {
                                if (errors_ptr != nullptr)
                                {
                                    size_t      error_str_size = errors_ptr->GetStringLength();
                                    std::string error = errors_ptr->GetStringPointer();
                                    std::cout << error_str_size << error;
                                    wprintf(L"Warnings and Errors:\n%S\n", errors_ptr->GetStringPointer());
                                }
                            }
                        }
                        else if (compiled_result_ptr->HasOutput(DXC_OUT_OBJECT))
                        {
                            IDxcBlob*      shader_blob      = nullptr;
                            IDxcBlobUtf16* shader_blob_name = nullptr;
                            op_result                       = compiled_result_ptr->GetOutput(DXC_OUT_OBJECT,
                                                                       __uuidof(IDxcBlobUtf16),
                                                                       reinterpret_cast<void**>(&shader_blob),
                                                                       &shader_blob_name);

                            if (SUCCEEDED(op_result) && shader_blob != nullptr)
                            {
                                FILE* fp = NULL;

                                _wfopen_s(&fp, shader_blob_name->GetStringPointer(), L"wb");
                                fwrite(shader_blob->GetBufferPointer(), shader_blob->GetBufferSize(), 1, fp);
                                fclose(fp);
                            }
                        }
                    }
                }
            }
        }
    }

    return false;
}

bool amit::gpu::ShaderHandler::InstanceInit()
{
    DxcCreateInstance(CLSID_DxcUtils, __uuidof(IDxcUtils), reinterpret_cast<void**>(&dxc_util_ptr_));
    DxcCreateInstance(CLSID_DxcCompiler, __uuidof(IDxcCompiler3), reinterpret_cast<void**>(&dxc_compiler_3_ptr_));

    return dxc_compiler_3_ptr_ != nullptr && dxc_util_ptr_ != nullptr;
}

bool amit::gpu::ShaderHandler::PopulateShaderDesc()
{
    File shader_asset(FilePath(kCoreShaderDir, kCoreShaderAssetDescFileName));
    if (shader_asset.Open())
    {
        std::istream   input(shader_asset.GetRawBuffer());
        nlohmann::json shader_asset_desc = nlohmann::json::parse(input);

        for (const auto& it : shader_asset_desc["compile"])
        {
            ShaderDesc shader_desc(it);
            default_shader_list_.push_back(std::move(shader_desc));
        }

        return true;
    }

    return false;
}
