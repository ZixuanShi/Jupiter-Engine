// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

export module jpt.DX12.DebugLayer;

#if !IS_RELEASE

using Microsoft::WRL::ComPtr;

export namespace jpt::DX12
{
    bool InitDebugLayer(UINT& dxgiFactoryFlags)
    {
        // Enable the debug layer (requires the Graphics Tools "optional feature").
        // NOTE: Enabling the debug layer after device creation will invalidate the active device.
        ComPtr<ID3D12Debug> debugController;
        if (FAILED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
        {
            return false;
        }

        debugController->EnableDebugLayer();

        // Enable additional debug layers.
        dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;

        return true;
    }
}

#endif