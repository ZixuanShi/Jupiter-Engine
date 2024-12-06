// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module; 

#include <Core/Validation/Assert.h>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

export module jpt.DX12.CommandQueue;

export namespace jpt::DX12
{
	class CommandQueue
	{
	private:
		Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_commandQueue;

	public:
		CommandQueue() = default;
		CommandQueue(Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue)
			: m_commandQueue(commandQueue) 
		{
		}

		ID3D12CommandQueue* Get() const
		{ 
			return m_commandQueue.Get();
		}
	};
}