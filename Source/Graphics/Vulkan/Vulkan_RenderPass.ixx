// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <vulkan/vulkan.h>

export module jpt.Vulkan.RenderPass;

export namespace jpt::Vulkan
{
	/** Describes the attachments that will be used during the rendering process
		We need to specify how many color and depth buffers there will be, 
		how many samples to use for each of them and how their contents should be handled throughout the rendering operations.*/
	class RenderPass
	{
	private:
		VkRenderPass m_renderPass = VK_NULL_HANDLE;

	public:
		bool Init(VkFormat imageFormat);
		void Shutdown();

	public:
		VkRenderPass GetHandle() const { return m_renderPass; }
	};
}