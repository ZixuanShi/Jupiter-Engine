// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <Core/Validation/Assert.h>
#include "Debugging/Logger.h"

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

export module jpt.DX12_SyncObjects;

import jpt.Graphics_Constants;

import jpt.DX12_SwapChain;

import jpt.TypeDefs;

export namespace jpt::DX12
{
    class SyncObjects
    {
    private:
        Microsoft::WRL::ComPtr<ID3D12Fence> m_fence;
        uint64 m_fenceValue = 0;
        Microsoft::WRL::ComPtr<ID3D12Fence> m_frameFences[kMaxFramesInFlight];
        uint64 m_frameFenceValues[kMaxFramesInFlight] = {};
        HANDLE m_fenceEvent = nullptr;

    public:
        bool Init(Microsoft::WRL::ComPtr<ID3D12Device> device, Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue, const SwapChain& swapChain, size_t& frameIndex);
        void Terminate(Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue, const SwapChain& swapChain, size_t& frameIndex);

        void WaitForPreviousFrame(Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue, const SwapChain& swapChain, size_t& frameIndex);
    };

    bool SyncObjects::Init(Microsoft::WRL::ComPtr<ID3D12Device> device, Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue, const SwapChain& swapChain, size_t& frameIndex)
    {
        JPT_ASSERT(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence)) == S_OK);
        m_fenceValue = 1;

        // Create an event handle to use for frame synchronization.
        m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
        if (m_fenceEvent == nullptr)
        {
            JPT_LOG(HRESULT_FROM_WIN32(GetLastError()));
        }

        // Wait for the command list to execute; we are reusing the same command 
        // list in our main loop but for now, we just want to wait for setup to 
        // complete before continuing.
        WaitForPreviousFrame(commandQueue, swapChain, frameIndex);

        return true;
    }

    void SyncObjects::Terminate(Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue, const SwapChain& swapChain, size_t& frameIndex)
    {
        WaitForPreviousFrame(commandQueue, swapChain, frameIndex);
        CloseHandle(m_fenceEvent);
    }

    void SyncObjects::WaitForPreviousFrame(Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue, const SwapChain& swapChain, size_t& frameIndex)
    {
        // WAITING FOR THE FRAME TO COMPLETE BEFORE CONTINUING IS NOT BEST PRACTICE.
        // This is code implemented as such for simplicity. The D3D12HelloFrameBuffering
        // sample illustrates how to use fences for efficient resource usage and to
        // maximize GPU utilization.
        
        // Signal and increment the fence value.
        const UINT64 fence = m_fenceValue;
        JPT_ASSERT(commandQueue->Signal(m_fence.Get(), fence) == S_OK);
        ++m_fenceValue;

        // Wait until the previous frame is finished.
        if (m_fence->GetCompletedValue() < fence)
        {
            JPT_ASSERT(m_fence->SetEventOnCompletion(fence, m_fenceEvent) == S_OK);
            WaitForSingleObject(m_fenceEvent, INFINITE);
        }

        frameIndex = swapChain.GetCurrentBackBufferIndex();
    }
}