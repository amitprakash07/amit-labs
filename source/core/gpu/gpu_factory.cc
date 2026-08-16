
#include <d3d12.h>

#include "gpu_factory.h"
#include "gpu_device.h"

amit::gpu::GpuFactory::GpuFactory()
    : dxgi_factory_(nullptr)
{
}

amit::gpu::GpuFactory::~GpuFactory()
{
    for (auto adapter : adapter_devices_)
    {
        delete adapter;
    }

    adapter_devices_.clear();
}

uint8_t amit::gpu::GpuFactory::GetDeviceCount() const
{
    return static_cast<uint8_t>(available_adapters_.size());
}

amit::gpu::GpuDevice* amit::gpu::GpuFactory::GetDevice(uint8_t device_adapter) const
{
    uint8_t device_count = GetDeviceCount();
    if (device_adapter < device_count)
    {
        while (adapter_devices_.empty() || adapter_devices_.size() < device_adapter)
        {
            ID3D12Device10* device = nullptr;
            HRESULT         device_creation_op = D3D12CreateDevice(available_adapters_[device_adapter],
                                                           D3D_FEATURE_LEVEL_12_0,
                                                           __uuidof(ID3D12Device10),
                                                           reinterpret_cast<void**>(&device));
            if (SUCCEEDED(device_creation_op))
            {
                GpuDevice* gpu_device = new GpuDevice(device);
                adapter_devices_.push_back(gpu_device);
            }
            else
            {
                return nullptr;
            }
        }

        return adapter_devices_[device_adapter];
    }

    return nullptr;
}

bool amit::gpu::GpuFactory::Initialize()
{
    if (dxgi_factory_ == nullptr)
    {
        IDXGIFactory2* temp    = nullptr;
        IDXGIAdapter1* adapter = nullptr;

        if (CreateDXGIFactory2(0, __uuidof(IDXGIFactory2), reinterpret_cast<void**>(&temp)) == S_OK)
        {
            dxgi_factory_        = temp;
            uint8_t adapter_iter = 0;
            HRESULT success      = S_OK;
            do
            {
                success = temp->EnumAdapters1(adapter_iter, &adapter);
                if (success != DXGI_ERROR_NOT_FOUND)
                {
                    available_adapters_.push_back(adapter);
                    adapter_iter++;
                }
            } while (success == S_OK);
        }
    }

    return dxgi_factory_ != nullptr;
}