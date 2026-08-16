#include "gpu_device_resource.h"


template <>
D3D12_COMMAND_QUEUE_DESC amit::gpu::GetDeviceResourceDefaultDesc<D3D12_COMMAND_QUEUE_DESC>()
{
    D3D12_COMMAND_QUEUE_DESC desc{};
    desc.Type     = D3D12_COMMAND_LIST_TYPE_DIRECT;
    desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
    desc.Flags    = D3D12_COMMAND_QUEUE_FLAG_NONE;
    desc.NodeMask = 0;
    return desc;
}