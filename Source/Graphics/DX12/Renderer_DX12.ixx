// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Applications/App/Application.h"
#include "Core/Minimal/CoreMacros.h"
#include "Core/Validation/Assert.h"
#include "Debugging/Logger.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <shellapi.h>
#if IS_DEBUG
	#include <dxgidebug.h>
#endif

export module jpt.Renderer_DX12;

import jpt.Renderer;
import jpt.Graphics.Constants;
import jpt.Vertex;

import jpt.CommandLine;

import jpt.DX12.DebugLayer;
import jpt.DX12.Device;

import jpt.TypeDefs;
import jpt.Time.TypeDefs;

import jpt.Window;
import jpt.Event.Window.Resize;
import jpt.Event.Window.Close;

using namespace jpt::DX12;
using namespace DirectX;
using Microsoft::WRL::ComPtr;

export namespace jpt
{
	class Renderer_DX12 final : public Renderer
	{
		using Super = Renderer;

	private:
		// Pipeline objects.
		D3D12_VIEWPORT m_viewport;
		D3D12_RECT m_scissorRect;
		ComPtr<IDXGISwapChain3> m_swapChain;
		Device m_device;
		ComPtr<ID3D12Resource> m_renderTargets[kMaxFramesInFlight];
		ComPtr<ID3D12CommandAllocator> m_commandAllocator;
		ComPtr<ID3D12CommandQueue> m_commandQueue;
		ComPtr<ID3D12RootSignature> m_rootSignature;
		ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
		ComPtr<ID3D12PipelineState> m_pipelineState;
		ComPtr<ID3D12GraphicsCommandList> m_commandList;
		UINT m_rtvDescriptorSize;

		// App resources.
		ComPtr<ID3D12Resource> m_vertexBuffer;
		D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;

		// Synchronization objects.
		UINT m_frameIndex;
		HANDLE m_fenceEvent;
		ComPtr<ID3D12Fence> m_fence;
		UINT64 m_fenceValue;

		bool m_useWarpDevice = false;

	public:
		virtual bool Init() override;
		virtual void Update(TimePrecision deltaSeconds) override;
		virtual void Shutdown() override;

		virtual void DrawFrame() override;

	private:
		bool LoadPipeline();
		bool LoadAssets();
		void PopulateCommandList();
		void WaitForPreviousFrame();
	};

	bool Renderer_DX12::Init()
	{
		JPT_ENSURE(Super::Init());

		m_useWarpDevice = CommandLine::GetInstance().Get("warp", false);

		bool success = true;
		success &= LoadPipeline();
		success &= LoadAssets();

		if (success)
		{
			JPT_INFO("DX12 renderer initialized successfully");
		}

		return success;
	}

	void Renderer_DX12::Update(TimePrecision deltaSeconds)
	{
		Super::Update(deltaSeconds);

	}

	void Renderer_DX12::Shutdown()
	{

		Super::Shutdown();
	}

	void Renderer_DX12::DrawFrame()
	{
		Super::DrawFrame();

	}

	bool Renderer_DX12::LoadPipeline()
	{
		bool success = true;
		UINT dxgiFactoryFlags = 0;

#if IS_DEBUG
		success &= InitDebugLayer(dxgiFactoryFlags);
#endif
		success &= m_device.Init(dxgiFactoryFlags, m_useWarpDevice);


		if (success)
		{
			JPT_INFO("DX12 renderer Loaded Pipeline successfully");
		}
		return success;
	}

	bool Renderer_DX12::LoadAssets()
	{
		return true;
	}

	void Renderer_DX12::PopulateCommandList()
	{
	}

	void Renderer_DX12::WaitForPreviousFrame()
	{
	}
}