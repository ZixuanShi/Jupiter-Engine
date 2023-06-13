// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "JupiterPCH.h"
#include "Window.h"

#if IS_PLATFORM_WIN64
	#include "DirectX/DX12Window.h"
#endif

namespace jpt
{
	bool WindowBase::Init()
	{
		m_aspectRatio = static_cast<float>(m_width) / static_cast<float>(m_height);

		return true;
	}

	WindowBase* WindowBase::Create()
	{
#if IS_PLATFORM_WIN64
		return new DX12Window();
#else
#error "Un-Supported platform"
		return nullptr;
#endif
	}
}
