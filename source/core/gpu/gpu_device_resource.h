#ifndef CORE_GPU_GPU_DEVICE_RESOURCE_H_
#define CORE_GPU_GPU_DEVICE_RESOURCE_H_

#include <cstdint>
#include <d3d12.h>

namespace amit::gpu
{
    enum GpuDeviceResourceType : uint8_t
    {
        Unknown,
        CmdQueue,
        CmdList,
        RootSignature,
        CommittedResource,
        PlacedResource,
        ComputePipeline,
        GraphicsPipeline,
        DescriptorHeap,
        Fence,
        Heap,
        QueryHeap,
        Test
    };

    class GpuNoDesc
    {
    };

    struct GpuFenceDesc
    {
        UINT64            InitialValue;
        D3D12_FENCE_FLAGS Flags;
    };

    struct GpuPlacedResDesc : D3D12_RESOURCE_DESC
    {
        ID3D12Heap*              pHeap;
        UINT64                   HeapOffset;
        D3D12_RESOURCE_STATES    InitialState;
        const D3D12_CLEAR_VALUE* pOptimizedClearValue;
    };

    struct GpuCmdListDesc
    {
        GpuCmdListDesc()
        {
            Node = 0;
            Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
        }

        uint8_t                 Node;
        D3D12_COMMAND_LIST_TYPE Type;
    };

    struct GpuCommittedResDesc : D3D12_RESOURCE_DESC
    {
        const D3D12_HEAP_PROPERTIES* pHeapProperties;
        D3D12_HEAP_FLAGS             HeapFlags;
        D3D12_RESOURCE_STATES        InitialResourceState;
        const D3D12_CLEAR_VALUE*     pOptimizedClearValue;
    };

    struct GpuRootSignatureDesc : D3D12_ROOT_SIGNATURE_DESC
    {
        UINT        nodeMask;
        const void* pBlobWithRootSignature;
        SIZE_T      blobLengthInBytes;
    };

    template <typename DeviceResourceDesc>
    DeviceResourceDesc GetDeviceResourceDefaultDesc()
    {
        return DeviceResourceDesc{};
    }

    template <>
    D3D12_COMMAND_QUEUE_DESC GetDeviceResourceDefaultDesc<D3D12_COMMAND_QUEUE_DESC>();

    template <class DeviceResourceDesc>
    class GpuDeviceResourceDesc : public DeviceResourceDesc
    {
    public:
        GpuDeviceResourceDesc()
        {
            *this = GetDeviceResourceDefaultDesc<DeviceResourceDesc>();
        }

        GpuDeviceResourceDesc(const DeviceResourceDesc& desc)
        {
            memcpy(this, &desc, sizeof(DeviceResourceDesc));
        }
    };

    class GpuDeviceResourceCommon
    {
    public:
        GpuDeviceResourceCommon(GpuDeviceResourceType resource_type)
            : resource_type_(resource_type)
        {
        }

        GpuDeviceResourceType Type() const
        {
            return resource_type_;
        }

        virtual ~GpuDeviceResourceCommon()
        {
        }

        GpuDeviceResourceCommon()                                           = delete;
        GpuDeviceResourceCommon(const GpuDeviceResourceCommon&)             = delete;
        GpuDeviceResourceCommon(GpuDeviceResourceCommon&&)                  = delete;
        GpuDeviceResourceCommon& operator=(const GpuDeviceResourceCommon&)  = delete;
        GpuDeviceResourceCommon& operator=(const GpuDeviceResourceCommon&&) = delete;

    private:
        GpuDeviceResourceType resource_type_;
    };

    template <class DeviceResourcePtrType>
    class GpuDeviceResource : public GpuDeviceResourceCommon
    {
    public:
        GpuDeviceResource(DeviceResourcePtrType* ptr, GpuDeviceResourceType res_type)
            : GpuDeviceResourceCommon(res_type)
            , device_res_ptr_(ptr)
        {
        }

        DeviceResourcePtrType* GetPtr() const
        {
            return device_res_ptr_;
        }

        virtual ~GpuDeviceResource()
        {
            if (device_res_ptr_)
            {
                reinterpret_cast<IUnknown*>(device_res_ptr_)->Release();
                device_res_ptr_ = nullptr;
            }
        }

        GpuDeviceResource()                                     = delete;
        GpuDeviceResource(const GpuDeviceResource&)             = delete;
        GpuDeviceResource(GpuDeviceResource&&)                  = delete;
        GpuDeviceResource& operator=(const GpuDeviceResource&)  = delete;
        GpuDeviceResource& operator=(const GpuDeviceResource&&) = delete;

    private:
        DeviceResourcePtrType* device_res_ptr_;
    };

    template <GpuDeviceResourceType resource_type, class DeviceResourcePtrType, class DeviceResourceDesc>
    class GpuDeviceResContainer : public GpuDeviceResource<DeviceResourcePtrType>
    {
    public:
        GpuDeviceResContainer(DeviceResourcePtrType* device_res_ptr, const DeviceResourceDesc& device_res_desc)
            : GpuDeviceResource<DeviceResourcePtrType>(device_res_ptr, resource_type)
            , device_res_desc_(device_res_desc)
        {
        }

        ~GpuDeviceResContainer()
        {
        }

        GpuDeviceResContainer()                                         = delete;
        GpuDeviceResContainer(const GpuDeviceResContainer&)             = delete;
        GpuDeviceResContainer(GpuDeviceResContainer&&)                  = delete;
        GpuDeviceResContainer& operator=(const GpuDeviceResContainer&)  = delete;
        GpuDeviceResContainer& operator=(const GpuDeviceResContainer&&) = delete;

    private:
        GpuDeviceResourceDesc<DeviceResourceDesc> device_res_desc_;
    };

#define INSTANTIATE_DEVICE_RESOURCE_CLASS(resource_type, resource_ptr_type, resource_desc)              \
    template <>                                                                                         \
    class GpuDeviceResContainer<resource_type, resource_ptr_type, GpuDeviceResourceDesc<resource_desc>> \
    {                                                                                                   \
    };

    INSTANTIATE_DEVICE_RESOURCE_CLASS(CmdQueue, ID3D12CommandQueue, D3D12_COMMAND_QUEUE_DESC);
    INSTANTIATE_DEVICE_RESOURCE_CLASS(ComputePipeline, ID3D12PipelineState, D3D12_COMPUTE_PIPELINE_STATE_DESC);
    INSTANTIATE_DEVICE_RESOURCE_CLASS(GraphicsPipeline, ID3D12PipelineState, D3D12_GRAPHICS_PIPELINE_STATE_DESC);
    INSTANTIATE_DEVICE_RESOURCE_CLASS(DescriptorHeap, ID3D12DescriptorHeap, D3D12_DESCRIPTOR_HEAP_DESC);
    INSTANTIATE_DEVICE_RESOURCE_CLASS(Heap, ID3D12Heap, D3D12_HEAP_DESC);
    INSTANTIATE_DEVICE_RESOURCE_CLASS(QueryHeap, ID3D12QueryHeap, D3D12_QUERY_HEAP_DESC);

    INSTANTIATE_DEVICE_RESOURCE_CLASS(CmdList, ID3D12CommandList, GpuCmdListDesc);
    INSTANTIATE_DEVICE_RESOURCE_CLASS(CommittedResource, ID3D12Resource, GpuCommittedResDesc);
    INSTANTIATE_DEVICE_RESOURCE_CLASS(PlacedResource, ID3D12Resource, GpuPlacedResDesc);
    INSTANTIATE_DEVICE_RESOURCE_CLASS(Fence, ID3D12Fence, GpuFenceDesc);
    INSTANTIATE_DEVICE_RESOURCE_CLASS(RootSignature, ID3D12RootSignature, GpuRootSignatureDesc);

}  // namespace amit::gpu
#endif
