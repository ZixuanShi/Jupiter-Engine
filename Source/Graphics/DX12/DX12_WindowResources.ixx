// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <Core/Validation/Assert.h>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

#include <d3dx12/d3dx12_root_signature.h>
#include <d3dx12/d3dx12_core.h>

export module jpt.DX12_WindowResources;

import jpt.Window;
import jpt.Window_Win32;

import jpt.Graphics_Constants;

import jpt.DX12_SwapChain;
import jpt.DX12_RTVHeap;
import jpt.DX12_SyncObjects;

import jpt.DX12_Device;

import jpt.Vector2;

export namespace jpt::DX12
{
    /** Per-Window specific DX12 resource. Each Window should have its own data */
    class WindowResources
    {
    private:
        Window* m_pOwner = nullptr;

    private:
        SwapChain m_swapChain;
        RTVHeap m_rtvHeap;

        Microsoft::WRL::ComPtr<ID3D12Resource> m_renderTargets[kMaxFramesInFlight];
        SyncObjects m_syncObjects;

        CD3DX12_VIEWPORT m_viewport;
        CD3DX12_RECT m_scissorRect;

        size_t m_currentFrame = 0;

        bool m_shouldRecreateSwapChain = false;

    public:
        void Shutdown(Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue);
        void WaitForPreviousFrame(Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue);

        bool CreateSwapChain(Microsoft::WRL::ComPtr<IDXGIFactory4> factory, Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue);
        bool CreateRTVHeap(const Device& device);
        bool CreateFrameResources(const Device& device);
        bool CreateSyncObjects(Microsoft::WRL::ComPtr<ID3D12Device> device, Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue);

    public:
        void SetOwner(Window* pOwner);
        Window* GetOwner() const { return m_pOwner; }

        const SwapChain& GetSwapChain() const { return m_swapChain; }
        const RTVHeap& GetRTVHeap() const { return m_rtvHeap; }
        const SyncObjects& GetSyncObjects() const { return m_syncObjects; }
        Microsoft::WRL::ComPtr<ID3D12Resource> GetRenderTarget(size_t index) const { return m_renderTargets[index]; }

        const CD3DX12_VIEWPORT& GetViewport() const { return m_viewport; }
        const CD3DX12_RECT& GetScissorRect() const { return m_scissorRect; }
        size_t GetCurrentFrame() const { return m_currentFrame; }
    };

    void WindowResources::Shutdown(Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue)
    {
        m_syncObjects.Shutdown(commandQueue, m_swapChain, m_currentFrame);
    }

    void WindowResources::WaitForPreviousFrame(Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue)
    {
        m_syncObjects.WaitForPreviousFrame(commandQueue, m_swapChain, m_currentFrame);
    }

    bool WindowResources::CreateSwapChain(Microsoft::WRL::ComPtr<IDXGIFactory4> factory, Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue)
    {
        Window_Win32* win32Window = static_cast<Window_Win32*>(m_pOwner);
        JPT_ASSERT(win32Window, "Window is not of type Window_Win32");

        const HWND hwnd = win32Window->GetHWND();
        JPT_ASSERT(m_swapChain.Init(m_pOwner->GetFrameSize(), hwnd, factory, commandQueue));

        m_currentFrame = m_swapChain.GetCurrentBackBufferIndex();

        return true;
    }

    bool WindowResources::CreateRTVHeap(const Device& device)
    {
        m_rtvHeap = device.CreateRTVHeap();
        return true;
    }

    bool WindowResources::CreateFrameResources(const Device& device)
    {
        CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap.Get()->GetCPUDescriptorHandleForHeapStart());

        // Create a RTV for each frame.
        for (UINT n = 0; n < kMaxFramesInFlight; n++)
        {
            JPT_ASSERT(m_swapChain.GetBuffer(n, IID_PPV_ARGS(&m_renderTargets[n])) == S_OK);
            device.CreateRenderTargetView(m_renderTargets[n].Get(), nullptr, rtvHandle);
            rtvHandle.Offset(1, device.GetRTVDescriptorSize());
        }

        return true;
    }

    bool WindowResources::CreateSyncObjects(Microsoft::WRL::ComPtr<ID3D12Device> device, Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue)
    {
        m_syncObjects.Init(device, commandQueue, m_swapChain, m_currentFrame);

        return true;
    }

    void WindowResources::SetOwner(Window* pOwner)
    {
        m_pOwner = pOwner;

        const Vec2i size = pOwner->GetFrameSize();

        m_viewport = CD3DX12_VIEWPORT(0.0f, 0.0f, static_cast<float>(size.x), static_cast<float>(size.y));
        m_scissorRect = CD3DX12_RECT(0, 0, size.x, size.y);
    }
}