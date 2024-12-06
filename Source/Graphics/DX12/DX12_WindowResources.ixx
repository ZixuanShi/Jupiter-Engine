// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <Core/Validation/Assert.h>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

export module jpt.DX12.WindowResources;

import jpt.Window;
import jpt.Window_Win32;

import jpt.DX12.SwapChain;

import jpt.DX12.CommandQueue;

export namespace jpt::DX12
{
	/** Per-Window specific DX12 resource. Each Window should have its own data */
	class WindowResources
	{
	private:
		Window* m_pOwner = nullptr;

	private:
		SwapChain m_swapChain;

		// Viewport and Scissor Rectangles
		// Render targets
		// rtvHeap
		// Synchronization objects

		size_t m_currentFrame = 0;

		bool m_shouldRecreateSwapChain = false;

	public:
		bool CreateSwapChain(Microsoft::WRL::ComPtr<IDXGIFactory4> factory, const CommandQueue& commandQueue);

	public:
		void SetOwner(Window* pOwner) { m_pOwner = pOwner; }
		Window* GetOwner() const { return m_pOwner; }
	};

	bool WindowResources::CreateSwapChain(Microsoft::WRL::ComPtr<IDXGIFactory4> factory, const CommandQueue& commandQueue)
	{
		Window_Win32* win32Window = static_cast<Window_Win32*>(m_pOwner);
		JPT_ASSERT(win32Window, "Window is not of type Window_Win32");

		const HWND hwnd = win32Window->GetHWND();
		JPT_ASSERT(m_swapChain.Init(m_pOwner->GetSize(), hwnd, factory, commandQueue));

		m_currentFrame = m_swapChain.GetCurrentFrameIndex();

		return true;
	}
}