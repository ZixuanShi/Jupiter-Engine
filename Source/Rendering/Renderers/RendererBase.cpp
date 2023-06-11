// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "JupiterPCH.h"
#include "RendererBase.h"

#if IS_PLATFORM_WIN64
	#include "DirectX/DX12Renderer.h"
#endif

namespace jpt
{
	RendererBase* jpt::RendererBase::Create()
	{
#if IS_PLATFORM_WIN64
		return new DX12Renderer();
#else
#error "Un-Supported platform"
		return nullptr;
#endif
	}
}
