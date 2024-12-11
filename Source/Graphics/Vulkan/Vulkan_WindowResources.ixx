// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Validation/Assert.h"
#include "Debugging/Logger.h"

#include <vulkan/vulkan.h>

export module jpt.Vulkan.WindowResources;

import jpt.Window;

import jpt.Graphics.Constants;

export namespace jpt::Vulkan
{
	/** Per-Window specific Vulkan resource. Each Window should have its own data */
	class WindowResources
	{
	private:
		Window* m_pOwner = nullptr;

	private:


	public:
		Window* GetOwner() const { return m_pOwner; }
		void SetOwner(Window* pOwner) { m_pOwner = pOwner; }
	};
}