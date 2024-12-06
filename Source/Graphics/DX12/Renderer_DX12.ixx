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
import jpt.DX12.CommandQueue;
import jpt.DX12.WindowResources;

import jpt.DynamicArray;
import jpt.TypeDefs;
import jpt.Time.TypeDefs;

import jpt.Window;
import jpt.Event.Window.Resize;
import jpt.Event.Window.Close;

using namespace jpt::DX12;

export namespace jpt
{
	class Renderer_DX12 final : public Renderer
	{
		using Super = Renderer;

	private:
		// Pipeline objects.
		Device m_device;
		CommandQueue m_commandQueue;

		DynamicArray<WindowResources> m_windowResources;

		bool m_useWarpDevice = false;

	public:
		virtual bool Init() override;
		virtual void Update(TimePrecision deltaSeconds) override;
		virtual void Shutdown() override;

		virtual void DrawFrame() override;

	public:
		const CommandQueue& GetCommandQueue() const { return m_commandQueue; }

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
		Microsoft::WRL::ComPtr<IDXGIFactory4> factory;
		CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&factory));

#if !IS_RELEASE
		success &= InitDebugLayer(dxgiFactoryFlags);
#endif
		success &= m_device.Init(factory, m_useWarpDevice);
		m_commandQueue = m_device.CreateCommandQueue();

		// Main Window
		Window* pMainWindow = GetApplication()->GetMainWindow();
		WindowResources& resources = m_windowResources.EmplaceBack();
		resources.SetOwner(pMainWindow);

		// Per-Window specific DX12 resource. Each Window should have its own data
		resources.CreateSwapChain(factory, m_commandQueue);

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