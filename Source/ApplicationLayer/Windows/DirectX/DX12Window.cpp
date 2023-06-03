#include "JupiterPCH.h"
#include "DX12Window.h"

#include "ApplicationLayer/Applications/Platform_Win64/Win64Application.h"
#include "DX12HelperStructs.h"

namespace jpt
{
	using namespace Microsoft::WRL;

	bool DX12Window::Init()
	{
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
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());
		for (uint32 i = 0; i < kFrameCount; ++i)
		{
			JPT_RETURN_FALSE_IF_LOG(m_swapChain->GetBuffer(i, IID_PPV_ARGS(&m_renderTargets[i])) != S_OK, "Failed to get swap chain buffer at index i");
			m_device->CreateRenderTargetView(m_renderTargets[i].Get(), nullptr, rtvHandle);
			rtvHandle.Offset(1, m_rtvDescriptorSize);
		}

		JPT_RETURN_FALSE_IF_LOG(m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocator)) != S_OK, "Failed to create command allocator");

		return true;
	}

	bool DX12Window::LoadAssets()
	{
		JPT_RETURN_FALSE_IF_LOG(m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocator.Get(), nullptr, IID_PPV_ARGS(&m_commandList)) != S_OK, "Failed to create command list");

		// Command lists are created in the recording state, but there is nothing
		// to record yet. The main loop expects it to be closed, so close it now
		JPT_RETURN_FALSE_IF_LOG(m_commandList->Close() != S_OK, "Failed to close command list when loading assets");

		// Create synchronization objects
		JPT_RETURN_FALSE_IF_LOG(m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence)) != S_OK, "Failed to create fence");
		m_fenceValue = 1;

		// Create an event handle to use for frame synchronization
		m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		JPT_RETURN_FALSE_IF_LOG(m_fenceEvent == nullptr, "Failed to create fence event 0x%lu", GetLastError());

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

		// Indicate that the back buffer will be used as a render target.
		CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_frameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
		m_commandList->ResourceBarrier(1, &barrier);

		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart(), m_frameIndex, m_rtvDescriptorSize);

		// Record commands.
		const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
		m_commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

		// Indicate that the back buffer will now be used to present
		barrier = CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_frameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
		m_commandList->ResourceBarrier(1, &barrier);
		JPT_RETURN_FALSE_IF_LOG(m_commandList->Close() != S_OK, "Failed to close command list when done populating");

		return true;
	}
}