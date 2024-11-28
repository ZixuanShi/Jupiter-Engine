// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Vulkan.Surface;

import jpt.Surface;

export namespace jpt::Vulkan
{
	class VulkanSurface final : public Surface
	{
		virtual bool Init() override;
	};

	bool VulkanSurface::Init()
	{
		return true;
	}
}