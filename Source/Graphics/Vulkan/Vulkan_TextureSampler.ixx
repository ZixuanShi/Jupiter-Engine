// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <vulkan/vulkan.h>

export module jpt.Vulkan.Texture.Sampler;

import jpt.Texture.Sampler;

export namespace jpt::Vulkan
{
	class TextureSampler_Vulkan final : public TextureSampler
	{
	private:
		VkSampler m_sampler = VK_NULL_HANDLE;

	public:
		virtual bool Init() override;
		virtual void Shutdown() override;

		VkSampler GetHandle() const { return m_sampler; }
	};
}