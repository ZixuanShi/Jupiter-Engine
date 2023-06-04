// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "JupiterPCH.h"
#include "DX12HelperStructs.h"

namespace jpt
{
	jpt::CD3DX12_CPU_DESCRIPTOR_HANDLE::CD3DX12_CPU_DESCRIPTOR_HANDLE(const D3D12_CPU_DESCRIPTOR_HANDLE& o) noexcept
		: D3D12_CPU_DESCRIPTOR_HANDLE(o)
	{
	}

	CD3DX12_CPU_DESCRIPTOR_HANDLE& CD3DX12_CPU_DESCRIPTOR_HANDLE::Offset(INT offsetInDescriptors, UINT descriptorIncrementSize) noexcept
	{
		ptr = SIZE_T(INT64(ptr) + INT64(offsetInDescriptors) * INT64(descriptorIncrementSize));
		return *this;
	}

	CD3DX12_CPU_DESCRIPTOR_HANDLE& CD3DX12_CPU_DESCRIPTOR_HANDLE::Offset(INT offsetScaledByIncrementSize) noexcept
	{
		ptr = SIZE_T(INT64(ptr) + INT64(offsetScaledByIncrementSize));
		return *this;
	}

	CD3DX12_CPU_DESCRIPTOR_HANDLE& CD3DX12_CPU_DESCRIPTOR_HANDLE::operator=(const D3D12_CPU_DESCRIPTOR_HANDLE& other) noexcept
	{
		ptr = other.ptr;
		return *this;
	}
}
