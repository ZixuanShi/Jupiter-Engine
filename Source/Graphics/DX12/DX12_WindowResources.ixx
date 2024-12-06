// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.DX12.WindowResources;

import jpt.Window;

import jpt.DX12.SwapChain;

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
	};
}