// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Logger.h"

#include <vulkan/vulkan.h>

export module jpt.Vulkan.DescriptorPool;

import jpt.Graphics.Constants;
import jpt.Vulkan.LogicalDevice;

export namespace jpt::Vulkan
{
	class DescriptorPool
	{
	private:
		VkDescriptorPool m_descriptorPool = VK_NULL_HANDLE;

	public:
		bool Init(const LogicalDevice& logicalDevice);
		void Shutdown(const LogicalDevice& logicalDevice);

	public:
		VkDescriptorPool GetHandle() const { return m_descriptorPool; }
	};

	bool DescriptorPool::Init(const LogicalDevice& logicalDevice)
	{
		VkDescriptorPoolSize poolSize{};
		poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSize.descriptorCount = static_cast<uint32>(kMaxFramesInFlight);

		VkDescriptorPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.poolSizeCount = 1;
		poolInfo.pPoolSizes = &poolSize;
		poolInfo.maxSets = static_cast<uint32>(kMaxFramesInFlight);

		if (const VkResult result = vkCreateDescriptorPool(logicalDevice.GetHandle(), &poolInfo, nullptr, &m_descriptorPool); result != VK_SUCCESS)
		{
			JPT_ERROR("Failed to create descriptor pool: %d", result);
			return false;
		}

		return true;
	}

	void DescriptorPool::Shutdown(const LogicalDevice& logicalDevice)
	{
		vkDestroyDescriptorPool(logicalDevice.GetHandle(), m_descriptorPool, nullptr);
	}
}