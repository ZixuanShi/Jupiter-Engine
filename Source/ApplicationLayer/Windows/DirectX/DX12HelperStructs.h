// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

#include <d3d12.h>

namespace jpt
{
    struct CD3DX12_DEFAULT {};
    extern const DECLSPEC_SELECTANY CD3DX12_DEFAULT D3D12_DEFAULT;

    struct CD3DX12_CPU_DESCRIPTOR_HANDLE : public D3D12_CPU_DESCRIPTOR_HANDLE
    {
        CD3DX12_CPU_DESCRIPTOR_HANDLE() = default;
        explicit CD3DX12_CPU_DESCRIPTOR_HANDLE(const D3D12_CPU_DESCRIPTOR_HANDLE& o) noexcept;
        CD3DX12_CPU_DESCRIPTOR_HANDLE(CD3DX12_DEFAULT) noexcept { ptr = 0; }
        CD3DX12_CPU_DESCRIPTOR_HANDLE(_In_ const D3D12_CPU_DESCRIPTOR_HANDLE& other, INT offsetScaledByIncrementSize) noexcept { InitOffsetted(other, offsetScaledByIncrementSize); }
        CD3DX12_CPU_DESCRIPTOR_HANDLE(_In_ const D3D12_CPU_DESCRIPTOR_HANDLE& other, INT offsetInDescriptors, UINT descriptorIncrementSize) noexcept { InitOffsetted(other, offsetInDescriptors, descriptorIncrementSize); }
        
        CD3DX12_CPU_DESCRIPTOR_HANDLE& Offset(INT offsetInDescriptors, UINT descriptorIncrementSize) noexcept;
        CD3DX12_CPU_DESCRIPTOR_HANDLE& Offset(INT offsetScaledByIncrementSize) noexcept;
        
        bool operator==(_In_ const D3D12_CPU_DESCRIPTOR_HANDLE& other) const noexcept { return (ptr == other.ptr); }
        bool operator!=(_In_ const D3D12_CPU_DESCRIPTOR_HANDLE& other) const noexcept { return (ptr != other.ptr); }
        CD3DX12_CPU_DESCRIPTOR_HANDLE& operator=(const D3D12_CPU_DESCRIPTOR_HANDLE& other) noexcept;

        inline void InitOffsetted(_In_ const D3D12_CPU_DESCRIPTOR_HANDLE& base, INT offsetScaledByIncrementSize) noexcept { InitOffsetted(*this, base, offsetScaledByIncrementSize); }
        inline void InitOffsetted(_In_ const D3D12_CPU_DESCRIPTOR_HANDLE& base, INT offsetInDescriptors, UINT descriptorIncrementSize) noexcept { InitOffsetted(*this, base, offsetInDescriptors, descriptorIncrementSize); }

        static inline void InitOffsetted(_Out_ D3D12_CPU_DESCRIPTOR_HANDLE& handle, _In_ const D3D12_CPU_DESCRIPTOR_HANDLE& base, INT offsetScaledByIncrementSize) noexcept { handle.ptr = SIZE_T(INT64(base.ptr) + INT64(offsetScaledByIncrementSize)); }
        static inline void InitOffsetted(_Out_ D3D12_CPU_DESCRIPTOR_HANDLE& handle, _In_ const D3D12_CPU_DESCRIPTOR_HANDLE& base, INT offsetInDescriptors, UINT descriptorIncrementSize) noexcept { handle.ptr = SIZE_T(INT64(base.ptr) + INT64(offsetInDescriptors) * INT64(descriptorIncrementSize)); }
    };

    struct CD3DX12_RESOURCE_BARRIER : public D3D12_RESOURCE_BARRIER
    {
        CD3DX12_RESOURCE_BARRIER() = default;
        explicit CD3DX12_RESOURCE_BARRIER(const D3D12_RESOURCE_BARRIER& o) noexcept :
            D3D12_RESOURCE_BARRIER(o)
        {}
        static inline CD3DX12_RESOURCE_BARRIER Transition(
            _In_ ID3D12Resource* pResource,
            D3D12_RESOURCE_STATES stateBefore,
            D3D12_RESOURCE_STATES stateAfter,
            UINT subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES,
            D3D12_RESOURCE_BARRIER_FLAGS flags = D3D12_RESOURCE_BARRIER_FLAG_NONE) noexcept
        {
            CD3DX12_RESOURCE_BARRIER result = {};
            D3D12_RESOURCE_BARRIER& barrier = result;
            result.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
            result.Flags = flags;
            barrier.Transition.pResource = pResource;
            barrier.Transition.StateBefore = stateBefore;
            barrier.Transition.StateAfter = stateAfter;
            barrier.Transition.Subresource = subresource;
            return result;
        }
        static inline CD3DX12_RESOURCE_BARRIER Aliasing(
            _In_ ID3D12Resource* pResourceBefore,
            _In_ ID3D12Resource* pResourceAfter) noexcept
        {
            CD3DX12_RESOURCE_BARRIER result = {};
            D3D12_RESOURCE_BARRIER& barrier = result;
            result.Type = D3D12_RESOURCE_BARRIER_TYPE_ALIASING;
            barrier.Aliasing.pResourceBefore = pResourceBefore;
            barrier.Aliasing.pResourceAfter = pResourceAfter;
            return result;
        }
        static inline CD3DX12_RESOURCE_BARRIER UAV(
            _In_ ID3D12Resource* pResource) noexcept
        {
            CD3DX12_RESOURCE_BARRIER result = {};
            D3D12_RESOURCE_BARRIER& barrier = result;
            result.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
            barrier.UAV.pResource = pResource;
            return result;
        }
    };
}