// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <vulkan/vulkan.h>

export module jpt.Vulkan.Pipeline;

import jpt.Vulkan.LogicalDevice;
import jpt.Vulkan.SwapChain;

export namespace jpt::Vulkan
{
	class Pipeline
	{
	private:
		VkPipeline m_pipeline;

	public:
		//bool Init(const LogicalDevice& logicalDevice, const SwapChain& swapChain);
		void Shutdown(const LogicalDevice& logicalDevice);
	};

	void Pipeline::Shutdown(const LogicalDevice& logicalDevice)
	{
		vkDestroyPipeline(logicalDevice.Get(), m_pipeline, nullptr);
	}
}