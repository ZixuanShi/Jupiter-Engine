// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <Core/Validation/Assert.h>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

export module jpt.DX12.Device;

import jpt.Graphics.Constants;

import jpt.DX12.RTVHeap;

export namespace jpt::DX12
{
	class Device
	{
	private:
		Microsoft::WRL::ComPtr<ID3D12Device> m_device;
		UINT m_rtvDescriptorSize = 0;

	public:
		bool Init(Microsoft::WRL::ComPtr<IDXGIFactory4> factory, bool useWarpDevice);

		Microsoft::WRL::ComPtr<ID3D12CommandQueue> CreateCommandQueue() const;
		RTVHeap CreateRTVHeap() const;
		void CreateRenderTargetView(_In_opt_  ID3D12Resource* pResource, _In_opt_  const D3D12_RENDER_TARGET_VIEW_DESC* pDesc, _In_  D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor) const;

		UINT GetRTVDescriptorSize() const;

	private:
		void GetHardwareAdapter(_In_ IDXGIFactory1* pFactory,
			_Outptr_result_maybenull_ IDXGIAdapter1** ppAdapter,
			bool requestHighPerformanceAdapter = false);
	};

	bool Device::Init(Microsoft::WRL::ComPtr<IDXGIFactory4> factory, bool useWarpDevice)
	{
		if (useWarpDevice)
		{
			Microsoft::WRL::ComPtr<IDXGIAdapter> warpAdapter;
			JPT_ASSERT(factory->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter)) == S_OK);
			JPT_ASSERT(D3D12CreateDevice(warpAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_device)) == S_OK);
		}
		else
		{
			Microsoft::WRL::ComPtr<IDXGIAdapter1> hardwareAdapter;
			GetHardwareAdapter(factory.Get(), &hardwareAdapter);
			JPT_ASSERT(D3D12CreateDevice(hardwareAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_device)) == S_OK);
		}

		m_rtvDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		return true;
	}

	Microsoft::WRL::ComPtr<ID3D12CommandQueue> Device::CreateCommandQueue() const
	{
		D3D12_COMMAND_QUEUE_DESC queueDesc = {};
		queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

		Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue;
		HRESULT hr = m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&commandQueue));
		JPT_ASSERT(hr == S_OK);

		return commandQueue;
	}

	RTVHeap Device::CreateRTVHeap() const
	{
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvHeap;

		// Describe and create a render target view (RTV) descriptor heap.
		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
		rtvHeapDesc.NumDescriptors = kMaxFramesInFlight;
		rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		JPT_ASSERT(m_device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&rtvHeap)) == S_OK);

		return RTVHeap(rtvHeap);
	}

	void Device::CreateRenderTargetView(ID3D12Resource* pResource, const D3D12_RENDER_TARGET_VIEW_DESC* pDesc, D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor) const
	{
		m_device->CreateRenderTargetView(pResource, pDesc, DestDescriptor);
	}

	UINT Device::GetRTVDescriptorSize() const
	{
		return m_rtvDescriptorSize;
	}

	_Use_decl_annotations_
	void Device::GetHardwareAdapter(IDXGIFactory1* pFactory, IDXGIAdapter1** ppAdapter, bool requestHighPerformanceAdapter /*= false*/)
	{
		*ppAdapter = nullptr;

		Microsoft::WRL::ComPtr<IDXGIAdapter1> adapter;

		Microsoft::WRL::ComPtr<IDXGIFactory6> factory6;
		if (SUCCEEDED(pFactory->QueryInterface(IID_PPV_ARGS(&factory6))))
		{
			for (
				UINT adapterIndex = 0;
				SUCCEEDED(factory6->EnumAdapterByGpuPreference(
					adapterIndex,
					requestHighPerformanceAdapter == true ? DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE : DXGI_GPU_PREFERENCE_UNSPECIFIED,
					IID_PPV_ARGS(&adapter)));
					++adapterIndex)
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

		if (adapter.Get() == nullptr)
		{
			for (UINT adapterIndex = 0; SUCCEEDED(pFactory->EnumAdapters1(adapterIndex, &adapter)); ++adapterIndex)
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
}