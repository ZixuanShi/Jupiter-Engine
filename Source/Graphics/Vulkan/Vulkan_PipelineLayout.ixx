// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module; 

#include "Debugging/Logger.h"

#include <vulkan/vulkan.h>

export module jpt.Vulkan.PipelineLayout;

import jpt.Vulkan.LogicalDevice;

export namespace jpt::Vulkan
{
	class PipelineLayout
	{
	private:
		VkPipelineLayout m_pipelineLayout = VK_NULL_HANDLE;

	public:
		bool Init(const LogicalDevice& logicalDevice, VkDescriptorSetLayout descriptorSetLayout);
		void Shutdown(const LogicalDevice& logicalDevice);

	public:
		VkPipelineLayout GetHandle() const { return m_pipelineLayout; }
	};

	bool PipelineLayout::Init(const LogicalDevice& logicalDevice, VkDescriptorSetLayout descriptorSetLayout)
	{
		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 1;
		pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;

		if (const VkResult result = vkCreatePipelineLayout(logicalDevice.GetHandle(), &pipelineLayoutInfo, nullptr, &m_pipelineLayout); result != VK_SUCCESS)
		{
			JPT_ERROR("Failed to create pipeline layout: %d", result);
			return false;
		}

		return true;
	}

	void PipelineLayout::Shutdown(const LogicalDevice& logicalDevice)
	{
		vkDestroyPipelineLayout(logicalDevice.GetHandle(), m_pipelineLayout, nullptr);
	}
}