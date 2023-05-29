#include "JupiterPCH.h"
#include "Window.h"

#if IS_PLATFORM_WIN64
	#include "DirectX/DX12Window.h"
#endif

namespace jpt
{
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
