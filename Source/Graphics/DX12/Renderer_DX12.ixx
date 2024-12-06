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
#include <d3dx12/d3dx12_core.h>

export module jpt.Renderer_DX12;

import jpt.Renderer;
import jpt.Graphics.Constants;
import jpt.Vertex;

import jpt.CommandLine;

import jpt.DX12.DebugLayer;
import jpt.DX12.Device;
import jpt.DX12.WindowResources;

import jpt.DynamicArray;
import jpt.TypeDefs;
import jpt.Time.TypeDefs;

import jpt.File.IO;
import jpt.File.Path;
import jpt.File.Path.Utils;

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
		// Shared Pipeline objects aross windows
		Device m_device;
		Microsoft::WRL::ComPtr<ID3D12CommandQueue>        m_commandQueue;
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator>    m_commandAllocator;
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList;
		Microsoft::WRL::ComPtr<ID3D12RootSignature>       m_rootSignature;
		Microsoft::WRL::ComPtr<ID3D12PipelineState>       m_pipelineState;

		Microsoft::WRL::ComPtr<ID3D12Resource> m_vertexBuffer;
		D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;

		DynamicArray<WindowResources> m_windowResources;

		bool m_useWarpDevice = false;

	public:
		virtual bool Init() override;
		virtual void Update(TimePrecision deltaSeconds) override;
		virtual void Shutdown() override;

		virtual void DrawFrame() override;

	private:
		void PopulateCommandList();
		void WaitForPreviousFrame();

		void CompileShaders();
		void CreateVertexBuffer(float aspectRatio);
	};

	bool Renderer_DX12::Init()
	{
		JPT_ENSURE(Super::Init());

		m_useWarpDevice = CommandLine::GetInstance().Get("warp", false);

		bool success = true;

		UINT dxgiFactoryFlags = 0;
		Microsoft::WRL::ComPtr<IDXGIFactory4> factory;
		CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&factory));

#if !IS_RELEASE
		success &= InitDebugLayer(dxgiFactoryFlags);
#endif
		success &= m_device.Init(factory, m_useWarpDevice);
		m_commandQueue = m_device.CreateCommandQueue();
		m_commandAllocator = m_device.CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT);

		// Main Window
		Window* pMainWindow = GetApplication()->GetMainWindow();
		WindowResources& resources = m_windowResources.EmplaceBack();
		resources.SetOwner(pMainWindow);

		// Per-Window specific DX12 resource. Each Window should have its own data
		resources.CreateSwapChain(factory, m_commandQueue);
		resources.CreateRTVHeap(m_device);
		resources.CreateFrameResources(m_device);

		// Shared resources aross windows
		m_rootSignature = m_device.CreateRootSignature();

		CompileShaders();

		m_device.Get()->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocator.Get(), m_pipelineState.Get(), IID_PPV_ARGS(&m_commandList));
		m_commandList->Close();

		CreateVertexBuffer(pMainWindow->GetAspectRatio());

		resources.CreateSyncObjects(m_device.Get(), m_commandQueue);

		if (success)
		{
			JPT_INFO("DX12 renderer Init successfully");
		}
		return success;
	}

	void Renderer_DX12::Update(TimePrecision deltaSeconds)
	{
		Super::Update(deltaSeconds);

	}

	void Renderer_DX12::Shutdown()
	{
		for (WindowResources& resources : m_windowResources)
		{
			resources.Shutdown(m_commandQueue);
		}

		Super::Shutdown();
	}

	void Renderer_DX12::DrawFrame()
	{
		Super::DrawFrame();

	}

	void Renderer_DX12::PopulateCommandList()
	{
	}

	void Renderer_DX12::WaitForPreviousFrame()
	{
	}

	void Renderer_DX12::CompileShaders()
	{
		// Compile Shaders
		Microsoft::WRL::ComPtr<ID3DBlob> vertexShader;
		Microsoft::WRL::ComPtr<ID3DBlob> pixelShader;
		const File::Path shadersPath = File::Combine(File::Source::Engine, L"Assets/Shaders/shaders.hlsl");

#if !IS_RELEASE
		// Enable better shader debugging with the graphics debugging tools.
		UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
		UINT compileFlags = 0;
#endif
		JPT_ASSERT(D3DCompileFromFile(shadersPath.ConstBuffer(), nullptr, nullptr, "VSMain", "vs_5_0", compileFlags, 0, &vertexShader, nullptr) == S_OK);
		JPT_ASSERT(D3DCompileFromFile(shadersPath.ConstBuffer(), nullptr, nullptr, "PSMain", "ps_5_0", compileFlags, 0, &pixelShader, nullptr) == S_OK);

		// Define the vertex input layout.
		D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
		};

		// Describe and create the graphics pipeline state object (PSO).
		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
		psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
		psoDesc.pRootSignature = m_rootSignature.Get();
		psoDesc.VS = CD3DX12_SHADER_BYTECODE(vertexShader.Get());
		psoDesc.PS = CD3DX12_SHADER_BYTECODE(pixelShader.Get());
		psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		psoDesc.DepthStencilState.DepthEnable = FALSE;
		psoDesc.DepthStencilState.StencilEnable = FALSE;
		psoDesc.SampleMask = UINT_MAX;
		psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		psoDesc.NumRenderTargets = 1;
		psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		psoDesc.SampleDesc.Count = 1;
		JPT_ASSERT(m_device.Get()->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipelineState)) == S_OK);
	}

	void Renderer_DX12::CreateVertexBuffer(float aspectRatio)
	{
		// Define the geometry for a triangle.
		Vertex triangleVertices[] =
		{
			{ { 0.0f, 0.25f * aspectRatio, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
			{ { 0.25f, -0.25f * aspectRatio, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
			{ { -0.25f, -0.25f * aspectRatio, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }
		};

		const UINT vertexBufferSize = sizeof(triangleVertices);

		// Note: using upload heaps to transfer static data like vert buffers is not 
		// recommended. Every time the GPU needs it, the upload heap will be marshalled 
		// over. Please read up on Default Heap usage. An upload heap is used here for 
		// code simplicity and because there are very few verts to actually transfer.
		CD3DX12_HEAP_PROPERTIES heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		CD3DX12_RESOURCE_DESC vertexBufferDesc = CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize);
		JPT_ASSERT(m_device.Get()->CreateCommittedResource(
			&heapProperties,
			D3D12_HEAP_FLAG_NONE,
			&vertexBufferDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&m_vertexBuffer)) == S_OK);

		// Copy the triangle data to the vertex buffer.
		UINT8* pVertexDataBegin;
		CD3DX12_RANGE readRange(0, 0);        // We do not intend to read from this resource on the CPU.
		JPT_ASSERT(m_vertexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin)) == S_OK);
		memcpy(pVertexDataBegin, triangleVertices, sizeof(triangleVertices));
		m_vertexBuffer->Unmap(0, nullptr);

		// Initialize the vertex buffer view.
		m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
		m_vertexBufferView.StrideInBytes = sizeof(Vertex);
		m_vertexBufferView.SizeInBytes = vertexBufferSize;
	}
}