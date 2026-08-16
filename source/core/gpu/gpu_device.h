#ifndef CORE_GPU_GPU_DEVICE_H_
#define CORE_GPU_GPU_DEVICE_H_

#include <vector>
#include <map>

#include <d3d12.h>

#include "gpu_device_resource.h"

namespace amit::gpu
{
     class GpuDevice
    {
    public:
        GpuDevice(ID3D12Device10* d3d12_device);
        GpuDevice() = delete;
        ~GpuDevice();

        template <class DeviceResourcePtrType, class DeviceResourceDesc>
        GpuDeviceResource<DeviceResourcePtrType>* Create(const DeviceResourceDesc& desc)
        {
            return nullptr;
        }

        template <class DeviceResourcePtrType, class DeviceResourceDesc>
        GpuDeviceResource<DeviceResourcePtrType>* Create(const GpuDeviceResourceDesc<DeviceResourceDesc>& desc)
        {
            return nullptr;
        }

        GpuDevice(const GpuDevice&)  = delete;
        GpuDevice(const GpuDevice&&) = delete;

        GpuDevice& operator=(const GpuDevice&)  = delete;
        GpuDevice& operator=(const GpuDevice&&) = delete;

        void UnitTest();

    private:
        ID3D12Device10*                                                        d3d12_device_;
        std::map<GpuDeviceResourceType, std::vector<GpuDeviceResourceCommon*>> device_resources_map_;
        std::map<D3D12_COMMAND_LIST_TYPE, ID3D12CommandAllocator*>             cmd_allocators_map_by_type_;
    };
}  // namespace amit::gpu

template <>
amit::gpu::GpuDeviceResource<ID3D12CommandList>* amit::gpu::GpuDevice::Create<ID3D12CommandList, amit::gpu::GpuCmdListDesc>(
    const amit::gpu::GpuCmdListDesc& desc);

#define DECLARE_SPECIALIZED_CREATE_D3D_DESC(resource_type, resource_ptr_type, resource_desc) \
    template <>                                                                              \
    amit::gpu::GpuDeviceResource<resource_ptr_type>* amit::gpu::GpuDevice::Create(const resource_desc& desc)

DECLARE_SPECIALIZED_CREATE_D3D_DESC(CmdQueue, ID3D12CommandQueue, D3D12_COMMAND_QUEUE_DESC);
DECLARE_SPECIALIZED_CREATE_D3D_DESC(ComputePipeline, ID3D12PipelineState, D3D12_COMPUTE_PIPELINE_STATE_DESC);
DECLARE_SPECIALIZED_CREATE_D3D_DESC(GraphicsPipeline, ID3D12PipelineState, D3D12_GRAPHICS_PIPELINE_STATE_DESC);
DECLARE_SPECIALIZED_CREATE_D3D_DESC(DescriptorHeap, ID3D12DescriptorHeap, D3D12_DESCRIPTOR_HEAP_DESC);
DECLARE_SPECIALIZED_CREATE_D3D_DESC(Heap, ID3D12Heap, D3D12_HEAP_DESC);
DECLARE_SPECIALIZED_CREATE_D3D_DESC(QueryHeap, ID3D12QueryHeap, D3D12_QUERY_HEAP_DESC);

#define DECLARE_SPECIALIZED_CREATE(resource_type, resource_ptr_type, resource_desc) \
    template <>                                                                     \
    amit::gpu::GpuDeviceResource<resource_ptr_type>* amit::gpu::GpuDevice::Create(        \
        const amit::gpu::GpuDeviceResourceDesc<resource_desc>& desc)

DECLARE_SPECIALIZED_CREATE(CommittedResource, ID3D12Resource, GpuCommittedResDesc);
DECLARE_SPECIALIZED_CREATE(PlacedResource, ID3D12Resource, GpuPlacedResDesc);
DECLARE_SPECIALIZED_CREATE(Fence, ID3D12Fence, GpuFenceDesc);
DECLARE_SPECIALIZED_CREATE(RootSignature, ID3D12RootSignature, GpuRootSignatureDesc);

#endif