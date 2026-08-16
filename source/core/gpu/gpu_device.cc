#include <cassert>

#include "gpu_device.h"

amit::gpu::GpuDevice::GpuDevice(ID3D12Device10* d3d12_device)
    : d3d12_device_(d3d12_device)
{
    d3d12_device->AddRef();
}

amit::gpu::GpuDevice::~GpuDevice()
{
    if (!cmd_allocators_map_by_type_.empty())
    {
        for (auto pair : cmd_allocators_map_by_type_)
        {
            pair.second->Release();
        }

        cmd_allocators_map_by_type_.clear();
    }

    if (!device_resources_map_.empty())
    {
        for (const auto& pair : device_resources_map_)
        {
            std::vector<GpuDeviceResourceCommon*> list = pair.second;
            for (auto contianer : list)
            {
                delete contianer;
            }
        }

        device_resources_map_.clear();
    }

    if (d3d12_device_)
    {
        d3d12_device_->Release();
        d3d12_device_ = nullptr;
    }
}

#define DEFINE_SPECIALIZED_CREATE_D3D_DESC(resource_type, resource_ptr_type, resource_desc) \
    template <>                                                                             \
    amit::gpu::GpuDeviceResource<resource_ptr_type>* amit::gpu::GpuDevice::Create(const resource_desc& desc)

DEFINE_SPECIALIZED_CREATE_D3D_DESC(CmdQueue, ID3D12CommandQueue, D3D12_COMMAND_QUEUE_DESC)
{
    GpuDeviceResourceDesc<D3D12_COMMAND_QUEUE_DESC> queue_desc = desc;
    GpuDeviceResource<ID3D12CommandQueue>*          ret_queue  = nullptr;

    if (d3d12_device_)
    {
        ID3D12CommandQueue* cmd_queue = nullptr;
        if (SUCCEEDED(d3d12_device_->CreateCommandQueue(
                &queue_desc, __uuidof(ID3D12CommandQueue), reinterpret_cast<void**>(&cmd_queue))))
        {
            ret_queue =
                new GpuDeviceResContainer<CmdQueue, ID3D12CommandQueue, D3D12_COMMAND_QUEUE_DESC>(cmd_queue, desc);
            device_resources_map_[CmdList].push_back(ret_queue);
        }
    }

    return ret_queue;
}

DEFINE_SPECIALIZED_CREATE_D3D_DESC(GraphicsPipeline, ID3D12PipelineState, D3D12_GRAPHICS_PIPELINE_STATE_DESC)
{
    GpuDeviceResource<ID3D12PipelineState>* ret_pipeline = nullptr;

    ID3D12PipelineState* pipeline_state = nullptr;
    if ((SUCCEEDED(d3d12_device_->CreateGraphicsPipelineState(
            &desc, __uuidof(ID3D12PipelineState), reinterpret_cast<void**>(&pipeline_state)))))
    {
        ret_pipeline =
            new GpuDeviceResContainer<GraphicsPipeline, ID3D12PipelineState, D3D12_GRAPHICS_PIPELINE_STATE_DESC>(
                pipeline_state, desc);
        device_resources_map_[GraphicsPipeline].push_back(ret_pipeline);
    }

    return ret_pipeline;
}

DEFINE_SPECIALIZED_CREATE_D3D_DESC(ComputePipeline, ID3D12PipelineState, D3D12_COMPUTE_PIPELINE_STATE_DESC)
{
    GpuDeviceResource<ID3D12PipelineState>* ret_pipeline = nullptr;

    ID3D12PipelineState* pipeline_state = nullptr;
    if ((SUCCEEDED(d3d12_device_->CreateComputePipelineState(
            &desc, __uuidof(ID3D12PipelineState), reinterpret_cast<void**>(&pipeline_state)))))
    {
        ret_pipeline =
            new GpuDeviceResContainer<GraphicsPipeline, ID3D12PipelineState, D3D12_COMPUTE_PIPELINE_STATE_DESC>(
                pipeline_state, desc);
        device_resources_map_[ComputePipeline].push_back(ret_pipeline);
    }

    return ret_pipeline;
}

#define DEFINE_SPECIALIZED_CREATE(resource_type, resource_ptr_type, resource_desc) \
    template <>                                                                    \
    amit::gpu::GpuDeviceResource<resource_ptr_type>* amit::gpu::GpuDevice::Create(       \
        const amit::gpu::GpuDeviceResourceDesc<resource_desc>& desc)

DEFINE_SPECIALIZED_CREATE(CmdList, ID3D12CommandList, GpuCmdListDesc)
{
    ID3D12CommandAllocator*               cmd_allocator = nullptr;
    GpuDeviceResourceDesc<GpuCmdListDesc> cmd_desc      = desc;
    GpuDeviceResource<ID3D12CommandList>* ret_list      = nullptr;

    if (!cmd_allocators_map_by_type_.contains(cmd_desc.Type))
    {
        if (SUCCEEDED(d3d12_device_->CreateCommandAllocator(
                cmd_desc.Type, __uuidof(ID3D12CommandAllocator), reinterpret_cast<void**>(&cmd_allocator))))
        {
            cmd_allocators_map_by_type_[cmd_desc.Type] = cmd_allocator;
        }
    }

    cmd_allocator = cmd_allocators_map_by_type_[cmd_desc.Type];

    if (cmd_allocator)
    {
        ID3D12CommandList* cmd_list = nullptr;
        if (SUCCEEDED(d3d12_device_->CreateCommandList(cmd_desc.Node,
                                                       cmd_desc.Type,
                                                       cmd_allocator,
                                                       nullptr,
                                                       __uuidof(ID3D12CommandList),
                                                       reinterpret_cast<void**>(&cmd_list))))
        {
            ret_list = new GpuDeviceResContainer<CmdList, ID3D12CommandList, amit::gpu::GpuCmdListDesc>(cmd_list, desc);
            device_resources_map_[CmdList].push_back(ret_list);
        }
    }

    return ret_list;
}

void amit::gpu::GpuDevice::UnitTest()
{
    assert(d3d12_device_ != nullptr);
    GpuDeviceResourceDesc<GpuCmdListDesc> cmd_desc;
    GpuDeviceResource<ID3D12CommandList>* cmd = Create<ID3D12CommandList>(cmd_desc);
    cmd->GetPtr()->SetName(L"My Command List");
    ID3D12GraphicsCommandList7* graphics_command_list7 = nullptr;
    cmd->GetPtr()->QueryInterface(__uuidof(ID3D12GraphicsCommandList7),
                                  reinterpret_cast<void**>(&graphics_command_list7));
    ULONG ref_count = graphics_command_list7->Release();
}
