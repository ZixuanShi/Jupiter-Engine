// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "JupiterPCH.h"
#include "DX12Window.h"

#include "ApplicationLayer/Applications/Platform_Win64/Win64Application.h"

namespace jpt
{
	using namespace Microsoft::WRL;

	bool DX12Window::Init()
	{
		m_viewport    = { 0.0f, 0.0f, static_cast<float>(m_width), static_cast<float>(m_height) };
		m_scissorRect = { 0,    0,    static_cast<LONG>(m_width),  static_cast<LONG>(m_height) };

		JPT_RETURN_FALSE_IF_LOG(!LoadPipeline(), "Failed loading pipeline");
		JPT_RETURN_FALSE_IF_LOG(!LoadAssets(), "Failed loading assets");

		return true;
	}

	void DX12Window::Render()
	{
		// Record all the commands we need to render the scene into the command list.
		PopulateCommandList();

		// Execute the command list
		ID3D12CommandList* ppCommandLists[] = { m_commandList.Get() };
		m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

		// Present the frame
		m_swapChain->Present(1, 0);

		WaitForPreviousFrame();
	}

	void DX12Window::Clean()
	{
		// Ensure that the GPU is no longer referencing resources that are about to be cleaned up by the destructor
		WaitForPreviousFrame();
		CloseHandle(m_fenceEvent);
	}

	void DX12Window::ParseCommandLineArgs(wchar_t* argv[], int32 argc)
	{
		for (int32 i = 1; i < argc; ++i)
		{
			if (_wcsnicmp(argv[i], L"-warp", wcslen(argv[i])) == 0 ||
				_wcsnicmp(argv[i], L"/warp", wcslen(argv[i])) == 0)
			{
				m_useWarpDevice = true;
				m_title = m_title + L" (WARP)";
			}
		}
	}

	bool DX12Window::LoadPipeline()
	{
		uint32 dxgiFactoryFlags = 0;

		// Enable the debug layer
#if IS_DEBUG
		ComPtr<ID3D12Debug> debugController;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
		{
			debugController->EnableDebugLayer();
			dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
		}
#endif

		// Create the device
		ComPtr<IDXGIFactory4> factory;
		JPT_RETURN_FALSE_IF_LOG(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&factory)) != S_OK, "Failed to create DXGIFactory2");

		if (m_useWarpDevice)
		{
			ComPtr<IDXGIAdapter> warpAdapter;
			JPT_RETURN_FALSE_IF_LOG(factory->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter)) != S_OK, "Failed to enum warp adapt factory"); 
			JPT_RETURN_FALSE_IF_LOG(D3D12CreateDevice(warpAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_device)) != S_OK, "Failed to create D3D12 Device");
		}
		else
		{
			ComPtr<IDXGIAdapter1> hardwareAdapter;
			GetHardwareAdapter(factory.Get(), &hardwareAdapter);
			JPT_RETURN_FALSE_IF_LOG(D3D12CreateDevice(hardwareAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_device)) != S_OK, "Failed to create D3D12 Device");
		}

		// Fill out a command queue description, then create the command queue:
		D3D12_COMMAND_QUEUE_DESC queueDesc = {};
		queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		JPT_RETURN_FALSE_IF_LOG(m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue)) != S_OK, "Failed to create Command Queue");

		// Fill out a swapchain description, then create the swap chain
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
		swapChainDesc.BufferCount = kFrameCount;
		swapChainDesc.Width = m_width;
		swapChainDesc.Height = m_height;
		swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapChainDesc.SampleDesc.Count = 1;
		ComPtr<IDXGISwapChain1> swapChain;
		JPT_RETURN_FALSE_IF_LOG(factory->CreateSwapChainForHwnd(m_commandQueue.Get(), Win64Application::GetHwnd(), &swapChainDesc, nullptr, nullptr, &swapChain) != S_OK, "Failed to Create SwapChainForHwnd");

		// Set support for fullscreen transitions
		JPT_RETURN_FALSE_IF_LOG(factory->MakeWindowAssociation(Win64Application::GetHwnd(), DXGI_MWA_NO_ALT_ENTER) != S_OK, "Can't make window association");

		// Assign member variables
		JPT_RETURN_FALSE_IF_LOG(swapChain.As(&m_swapChain) != S_OK, "Failed to assign swapChain as member");
		m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();

		// Create Descriptor heaps. Describe and create a render target view (RTV) descriptor heap
		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
		rtvHeapDesc.NumDescriptors = kFrameCount;
		rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		JPT_RETURN_FALSE_IF_LOG(m_device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_rtvHeap)) != S_OK, "Failed to create Descriptor Heap");
		m_rtvDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

		// Create a RTV for each frame
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());
		
		for (uint32 i = 0; i < kFrameCount; ++i)
		{
			JPT_RETURN_FALSE_IF_LOG(m_swapChain->GetBuffer(i, IID_PPV_ARGS(&m_renderTargets[i])) != S_OK, "Failed to get swap chain buffer at index i");
			m_device->CreateRenderTargetView(m_renderTargets[i].Get(), nullptr, rtvHandle);
			rtvHandle.ptr = SIZE_T((INT64)rtvHandle.ptr + (INT64)m_rtvDescriptorSize);
		}

		JPT_RETURN_FALSE_IF_LOG(m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocator)) != S_OK, "Failed to create command allocator");

		return true;
	}

	bool DX12Window::LoadAssets()
	{
		// Create an empty root signature
		D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
		ComPtr<ID3DBlob> signature;
		ComPtr<ID3DBlob> error;
		JPT_RETURN_FALSE_IF_LOG(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error) != S_OK, "Failed to serialize root signature 0x%lu", GetLastError());
		JPT_RETURN_FALSE_IF_LOG(m_device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature)) != S_OK, "Failed to create root signature 0x%lu", GetLastError());

		// Create the pipeline state, which includes compiling and loading shaders
		ComPtr<ID3DBlob> vertexShader;
		ComPtr<ID3DBlob> pixelShader;

#if IS_DEBUG
		// Enable better shader debugging with the graphics debugging tools
		const uint32 compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
		const uint32 compileFlags = D3DCOMPILE_OPTIMIZATION_LEVEL1;
#endif

		const jpt::wstring basicShaderPath = jpt::GetOutputAssetPathW(L"Engine/Shaders/BasicShader.hlsl");
		JPT_RETURN_FALSE_IF_LOG(D3DCompileFromFile(basicShaderPath.c_str(), nullptr, nullptr, "VSMain", "vs_5_0", compileFlags, 0, &vertexShader, nullptr) != S_OK, "Failed to compile vertexShader 0x%lu", GetLastError());
		JPT_RETURN_FALSE_IF_LOG(D3DCompileFromFile(basicShaderPath.c_str(), nullptr, nullptr, "PSMain", "ps_5_0", compileFlags, 0, &pixelShader, nullptr) != S_OK, "Failed to compile pixelShader 0x%lu", GetLastError());

		// Define the vertex input layout
		D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,  D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		};

		// Describe and create the graphics pipeline state object (PSO)
		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
		psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
		psoDesc.pRootSignature = m_rootSignature.Get();
		psoDesc.VS = D3D12_SHADER_BYTECODE(vertexShader->GetBufferPointer(), vertexShader->GetBufferSize());
		psoDesc.PS = D3D12_SHADER_BYTECODE(pixelShader->GetBufferPointer(), pixelShader->GetBufferSize());
		psoDesc.RasterizerState = D3D12_RASTERIZER_DESC(D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_BACK, false, D3D12_DEFAULT_DEPTH_BIAS, D3D12_DEFAULT_DEPTH_BIAS_CLAMP, D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS, true, false, false, 0, D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF);
		D3D12_BLEND_DESC blendDesc;
		const D3D12_RENDER_TARGET_BLEND_DESC defaultRenderTargetBlendDesc =
		{
			FALSE,FALSE,
			D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
			D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
			D3D12_LOGIC_OP_NOOP,
			D3D12_COLOR_WRITE_ENABLE_ALL,
		};
		blendDesc.AlphaToCoverageEnable = false;
		blendDesc.IndependentBlendEnable = false;
		for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
			blendDesc.RenderTarget[i] = defaultRenderTargetBlendDesc;
		psoDesc.BlendState = blendDesc;
		psoDesc.DepthStencilState.DepthEnable = false;
		psoDesc.DepthStencilState.StencilEnable = false;
		psoDesc.SampleMask = UINT_MAX;
		psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		psoDesc.NumRenderTargets = 1;
		psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		psoDesc.SampleDesc.Count = 1;
		JPT_RETURN_FALSE_IF_LOG(m_device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipelineState)) != S_OK, "Failed to create graphics pipeline state 0x%lu", GetLastError());

		// Create the command list.
		JPT_RETURN_FALSE_IF_LOG(m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocator.Get(), m_pipelineState.Get(), IID_PPV_ARGS(&m_commandList)) != S_OK, "Failed to create command list 0x%lu", GetLastError());

		// Command lists are created in the recording state, but there is nothing
		// to record yet. The main loop expects it to be closed, so close it now
		JPT_RETURN_FALSE_IF_LOG(m_commandList->Close() != S_OK, "Failed to close command list when loading assets 0x%lu", GetLastError());

		// Create vertex buffer
		Vertex triangleVertices[] =
		{
			{ FVec3( 0.00f,  0.25f * m_aspectRatio, 0.0f), LinearColor(1.0f, 0.0f, 0.0f, 1.0f) },
			{ FVec3( 0.25f, -0.25f * m_aspectRatio, 0.0f), LinearColor(0.0f, 1.0f, 0.0f, 1.0f) },
			{ FVec3(-0.25f, -0.25f * m_aspectRatio, 0.0f), LinearColor(0.0f, 0.0f, 1.0f, 1.0f) },
		};
		const uint32 vertexBufferSize = sizeof(triangleVertices);

		// Note: using upload heaps to transfer static data like vert buffers is not 
        // recommended. Every time the GPU needs it, the upload heap will be marshalled 
        // over. Please read up on Default Heap usage. An upload heap is used here for 
        // code simplicity and because there are very few verts to actually transfer.
		D3D12_HEAP_PROPERTIES heapProperties;
		heapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
		heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		heapProperties.CreationNodeMask = 1;
		heapProperties.VisibleNodeMask = 1;

		D3D12_RESOURCE_DESC desc;
		desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		desc.Alignment = 0;
		desc.Width = vertexBufferSize;
		desc.Height = 1;
		desc.DepthOrArraySize = 1;
		desc.MipLevels = 1;
		desc.Format = DXGI_FORMAT_UNKNOWN;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		desc.Flags = D3D12_RESOURCE_FLAG_NONE;
		JPT_RETURN_FALSE_IF_LOG(m_device->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&m_vertexBuffer)) != S_OK, "Failed to create committed resource 0x%lu", GetLastError());
		
		// Copy the triangle data to the vertex buffer
		uint8* pVertexDataBegin = nullptr;
		D3D12_RANGE readRange(0, 0);	// We do not intend to read from this resource on the CPU.
		JPT_RETURN_FALSE_IF_LOG(m_vertexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin)) != S_OK, "Failed to map vertex buffer");
		std::memcpy(pVertexDataBegin, triangleVertices, sizeof(triangleVertices));
		m_vertexBuffer->Unmap(0, nullptr);

		// Initialize the vertex buffer view
		m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
		m_vertexBufferView.StrideInBytes = sizeof(Vertex);
		m_vertexBufferView.SizeInBytes = vertexBufferSize;

		// Create synchronization objects
		JPT_RETURN_FALSE_IF_LOG(m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence)) != S_OK, "Failed to create fence 0x%lu", GetLastError());
		m_fenceValue = 1;

		// Create an event handle to use for frame synchronization
		m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		JPT_RETURN_FALSE_IF_LOG(m_fenceEvent == nullptr, "Failed to create fence event 0x%lu", GetLastError());

		// Wait for the command list to execute; we are reusing the same command 
        // list in our main loop but for now, we just want to wait for setup to 
        // complete before continuing.
        WaitForPreviousFrame();

		return true;
	}

	bool DX12Window::WaitForPreviousFrame()
	{
		// WAITING FOR THE FRAME TO COMPLETE BEFORE CONTINUING IS NOT BEST PRACTICE.
		// This is code implemented as such for simplicity. The D3D12HelloFrameBuffering
		// sample illustrates how to use fences for efficient resource usage and to
		// maximize GPU utilization.

		// Signal and increment the fence value
		const uint64 fence = m_fenceValue;
		JPT_RETURN_FALSE_IF_LOG(m_commandQueue->Signal(m_fence.Get(), fence) != S_OK, "Failed to get fence signal");
		++m_fenceValue;

		// Wait until the previous frame is finished
		if (m_fence->GetCompletedValue() < fence)
		{
			JPT_RETURN_FALSE_IF_LOG(m_fence->SetEventOnCompletion(fence, m_fenceEvent) != S_OK, "Failed to set event on completion");
			WaitForSingleObject(m_fenceEvent, INFINITE);
		}

		m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();

		return true;
	}

	_Use_decl_annotations_
	void DX12Window::GetHardwareAdapter(IDXGIFactory1* pFactory, IDXGIAdapter1** ppAdapter, bool requestHighPerformanceAdapter)
	{
		*ppAdapter = nullptr;

		ComPtr<IDXGIAdapter1> adapter;

		ComPtr<IDXGIFactory6> factory6;
		if (SUCCEEDED(pFactory->QueryInterface(IID_PPV_ARGS(&factory6))))
		{
			for(uint32 adapterIndex = 0; 
				SUCCEEDED(factory6->EnumAdapterByGpuPreference(
					adapterIndex, 
					requestHighPerformanceAdapter == true? DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE: DXGI_GPU_PREFERENCE_UNSPECIFIED,
					IID_PPV_ARGS(&adapter)));
				++adapterIndex)
			{
				DXGI_ADAPTER_DESC1 desc;
				adapter->GetDesc1(&desc);

				if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
				{
					// Don't select the basic render driver adapter
					// If yuou want a software adapter, pass in "/warp" on the command line
					continue;
				}

				// Check to see whether the adapter supports Direct3D 12, but don't create the actual device yet
				if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), nullptr)))
				{
					break;
				}
			}
		}

		if (adapter.Get() == nullptr)
		{
			for (uint32 adapterIndex = 0; SUCCEEDED(pFactory->EnumAdapters1(adapterIndex, &adapter)); ++adapterIndex)
			{
				DXGI_ADAPTER_DESC1 desc;
				adapter->GetDesc1(&desc);

				if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
				{
					// Don't select the Basic Render Driver adapter.
					// If you want a software adapter, pass in "/warp" on the command line.
					continue;
				}

				// Check to see whether the adapter supports Direct3D 12, but don't create the
				// actual device yet.
				if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
				{
					break;
				}
			}
		}

		*ppAdapter = adapter.Detach();
	}

	bool DX12Window::PopulateCommandList()
	{
		// Command list allocators can only be reset when the associated command lists have finished execution on the GPU
		// apps should use fences to determine GPU execution progress.
		JPT_RETURN_FALSE_IF_LOG(m_commandAllocator->Reset() != S_OK, "Failed to reset m_command allocator");

		// However, when ExecuteCommandList() is called on a particular command list, 
		// that command list can then be reset at any time and must be before re-recording.
		JPT_RETURN_FALSE_IF_LOG(m_commandList->Reset(m_commandAllocator.Get(), m_pipelineState.Get()) != S_OK, "Failed to reset command list");

		// Set necessary state
		m_commandList->SetGraphicsRootSignature(m_rootSignature.Get());
		m_commandList->RSSetViewports(1, &m_viewport);
		m_commandList->RSSetScissorRects(1, &m_scissorRect);

		// Indicate that the back buffer will be used as a render target.
		D3D12_RESOURCE_BARRIER barrier(D3D12_RESOURCE_BARRIER_TYPE_TRANSITION, D3D12_RESOURCE_BARRIER_FLAG_NONE);
		barrier.Transition.pResource = m_renderTargets[m_frameIndex].Get();
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
		barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		m_commandList->ResourceBarrier(1, &barrier);

		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());
		rtvHandle.ptr = SIZE_T((INT64)rtvHandle.ptr + (INT64)m_frameIndex * (INT64)m_rtvDescriptorSize);
		m_commandList->OMSetRenderTargets(1, &rtvHandle, false, nullptr);

		// Record commands.
		static float red = 0.0f;
		static bool incflag = true;
		if (incflag)
		{
			red += 0.003f;
			incflag = (red < 0.8f);
		}
		else if (!incflag)
		{
			red -= 0.003f;
			incflag = (red <= 0.0f);
		}
		const float clearColor[] = { red, 0.2f, 0.4f, 1.0f };
		m_commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

		m_commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_commandList->IASetVertexBuffers(0, 1, &m_vertexBufferView);
		m_commandList->DrawInstanced(3, 1, 0, 0);

		// Indicate that the back buffer will now be used to present
		D3D12_RESOURCE_BARRIER backBuffer = barrier;
		backBuffer.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		backBuffer.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
		m_commandList->ResourceBarrier(1, &backBuffer);
		JPT_RETURN_FALSE_IF_LOG(m_commandList->Close() != S_OK, "Failed to close command list when done populating");

		return true;
	}
}