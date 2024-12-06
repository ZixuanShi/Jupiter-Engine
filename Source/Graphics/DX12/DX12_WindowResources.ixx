// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <Core/Validation/Assert.h>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <d3dx12/d3dx12_root_signature.h>

export module jpt.DX12.WindowResources;

import jpt.Window;
import jpt.Window_Win32;

import jpt.Graphics.Constants;

import jpt.DX12.SwapChain;
import jpt.DX12.RTVHeap;
import jpt.DX12.SyncObjects;

import jpt.DX12.Device;

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

		// Viewport and Scissor Rectangles

		size_t m_currentFrame = 0;

		bool m_shouldRecreateSwapChain = false;

	public:
		void Shutdown(Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue);

		bool CreateSwapChain(Microsoft::WRL::ComPtr<IDXGIFactory4> factory, Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue);
		bool CreateRTVHeap(const Device& device);
		bool CreateFrameResources(const Device& device);
		bool CreateSyncObjects(Microsoft::WRL::ComPtr<ID3D12Device> device, Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue);

	public:
		void SetOwner(Window* pOwner) { m_pOwner = pOwner; }
		Window* GetOwner() const { return m_pOwner; }
	};

	void WindowResources::Shutdown(Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue)
	{
		m_syncObjects.Shutdown(commandQueue, m_swapChain, m_currentFrame);
	}

	bool WindowResources::CreateSwapChain(Microsoft::WRL::ComPtr<IDXGIFactory4> factory, Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue)
	{
		Window_Win32* win32Window = static_cast<Window_Win32*>(m_pOwner);
		JPT_ASSERT(win32Window, "Window is not of type Window_Win32");

		const HWND hwnd = win32Window->GetHWND();
		JPT_ASSERT(m_swapChain.Init(m_pOwner->GetSize(), hwnd, factory, commandQueue));

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
}