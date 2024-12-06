// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <Core/Validation/Assert.h>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

export module jpt.DX12.SwapChain;

import jpt.Graphics.Constants;

import jpt.DX12.CommandQueue;

import jpt.Vector2;
import jpt.TypeDefs;

export namespace jpt::DX12
{
	class SwapChain
	{
	private:
		Microsoft::WRL::ComPtr<IDXGISwapChain3> m_swapChain;

	public:
		bool Init(Vec2i size, HWND hwnd, Microsoft::WRL::ComPtr<IDXGIFactory4> factory, const CommandQueue& commandQueue);
		uint32 GetCurrentFrameIndex() const;
	};

	bool SwapChain::Init(Vec2i size, HWND hwnd, Microsoft::WRL::ComPtr<IDXGIFactory4> factory, const CommandQueue& commandQueue)
	{
		// Describe and create the swap chain.
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
		swapChainDesc.BufferCount = kMaxFramesInFlight;
		swapChainDesc.Width = size.x;
		swapChainDesc.Height = size.y;
		swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapChainDesc.SampleDesc.Count = 1;

		Microsoft::WRL::ComPtr<IDXGISwapChain1> swapChain;
		JPT_ASSERT(factory->CreateSwapChainForHwnd(
			commandQueue.Get(),        // Swap chain needs the queue so that it can force a flush on it.
			hwnd,
			&swapChainDesc,
			nullptr,
			nullptr,
			&swapChain
		) == S_OK);

		swapChain.As(&m_swapChain);

		return true;
	}

	uint32 SwapChain::GetCurrentFrameIndex() const
	{
		return m_swapChain->GetCurrentBackBufferIndex();
	}
}