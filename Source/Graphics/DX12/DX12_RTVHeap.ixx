// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

export module jpt.DX12.RTVHeap;

export namespace jpt::DX12
{
	/** Render target view (RTV) descriptor heap */
	class RTVHeap
	{
	private:
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_rtvHeap;

	public:
		RTVHeap() = default;
		RTVHeap(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvHeap);

	public:
		ID3D12DescriptorHeap* Get() const;
	};

	RTVHeap::RTVHeap(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvHeap)
		: m_rtvHeap(rtvHeap)
	{
	}

	ID3D12DescriptorHeap* RTVHeap::Get() const
	{
		return m_rtvHeap.Get();
	}
}