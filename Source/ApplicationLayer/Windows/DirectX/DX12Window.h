// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

#include "ApplicationLayer/Windows/Window.h"

namespace jpt
{
	/** Windows 64 window implementation class */
	class DX12Window final : public WindowBase
	{
		using Super = WindowBase;

	private:
		// Count for frame buffer
		static constexpr uint32 kFrameCount = 2;

		// Pipeline objects
		D3D12_VIEWPORT m_viewport;
		D3D12_RECT m_scissorRect;
		Microsoft::WRL::ComPtr<IDXGISwapChain3> m_swapChain;
		Microsoft::WRL::ComPtr<ID3D12Device> m_device;
		Microsoft::WRL::ComPtr<ID3D12Resource> m_renderTargets[kFrameCount];
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_commandAllocator;
		Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_commandQueue;
		Microsoft::WRL::ComPtr<ID3D12RootSignature> m_rootSignature;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
		Microsoft::WRL::ComPtr<ID3D12PipelineState> m_pipelineState;
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList;
		uint32 m_rtvDescriptorSize = 0;

		// App resources
		Microsoft::WRL::ComPtr<ID3D12Resource> m_vertexBuffer;
		D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;

		// Synchoronization objects
		uint32 m_frameIndex = 0;
		HANDLE m_fenceEvent = nullptr;
		Microsoft::WRL::ComPtr<ID3D12Fence> m_fence;
		uint64 m_fenceValue = 0;

	public:
		virtual bool Init() override;
		virtual void Render() override;
		virtual void Clean() override;

		virtual void ParseCommandLineArgs(WCHAR* argv[], int32 argc) override;

	private:
		// Init
		bool LoadPipeline();
		bool LoadAssets();
		
		// Clean
		bool WaitForPreviousFrame();

		/** Helper function for acquiring the first available hardware adapter that supports Direct3D 12.
		   If no such adapter can be found, *ppAdapter will be set to nullptr. */
		void GetHardwareAdapter(_In_ IDXGIFactory1* pFactory, _Outptr_result_maybenull_ IDXGIAdapter1** ppAdapter, bool requestHighPerformanceAdapter = false);

		bool PopulateCommandList();
	};
}

