// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <Core/Validation/Assert.h>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

export module jpt.DX12.SwapChain;

export namespace jpt::DX12
{
	class SwapChain
	{
	private:
		Microsoft::WRL::ComPtr<IDXGISwapChain3> m_swapChain;
	};
}