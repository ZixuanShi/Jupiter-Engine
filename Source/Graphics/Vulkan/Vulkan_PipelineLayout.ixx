// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Logger.h"

#include <vulkan/vulkan.h>

export module jpt.Vulkan.PipelineLayout;

import jpt.Vulkan.LogicalDevice;
import jpt.Vulkan.DescriptorSetLayout;

import jpt.TypeDefs;

export namespace jpt::Vulkan
{
	class PipelineLayout
	{
	private:
		VkPipelineLayout m_pipelineLayout;

	public:
		bool Init(const LogicalDevice& logicalDevice, DescriptorSetLayout& descriptorSetLayout);
		void Shutdown(const LogicalDevice& logicalDevice);

	public:
		VkPipelineLayout Get() const { return m_pipelineLayout; }
	};

	bool PipelineLayout::Init(const LogicalDevice& logicalDevice, DescriptorSetLayout& descriptorSetLayout)
	{
		VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 1;
		pipelineLayoutInfo.pSetLayouts = descriptorSetLayout.GetPtr();

		if (const VkResult result = vkCreatePipelineLayout(logicalDevice.Get(), &pipelineLayoutInfo, nullptr, &m_pipelineLayout); result != VK_SUCCESS)
		{
			JPT_ERROR("Failed to create pipeline layout! VkResult: %i", static_cast<uint32>(result));
			return false;
		}

		return true;
	}

	void PipelineLayout::Shutdown(const LogicalDevice& logicalDevice)
	{
		vkDestroyPipelineLayout(logicalDevice.Get(), m_pipelineLayout, nullptr);
	}
}