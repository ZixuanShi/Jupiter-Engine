// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Logger.h"

#include <vulkan/vulkan.h>

module jpt.Vulkan_PipelineLayout;

import jpt.Application;
import jpt.Renderer_Vulkan;

import jpt.Vulkan_Constants;
import jpt.Vulkan_LogicalDevice;
import jpt.Vulkan_DescriptorSetLayout;

namespace jpt::Vulkan
{
    bool PipelineLayout::Init()
    {
        const Renderer_Vulkan* pVulkanRenderer = GetVkRenderer();
        const DescriptorSetLayout& descriptorSetLayout = pVulkanRenderer->GetDescriptorSetLayout();
        const LogicalDevice& logicalDevice = pVulkanRenderer->GetLogicalDevice();

        VkPushConstantRange pushConstantRange{};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(PushConstantData);

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 1;
        pipelineLayoutInfo.pSetLayouts = descriptorSetLayout.GetHandlePtr();
        pipelineLayoutInfo.pushConstantRangeCount = 1;    // Shared for all shaders
        pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

        if (const VkResult result = vkCreatePipelineLayout(logicalDevice.GetHandle(), &pipelineLayoutInfo, nullptr, &m_pipelineLayout); result != VK_SUCCESS)
        {
            JPT_ERROR("Failed to create pipeline layout: %d", result);
            return false;
        }

        return true;
    }

    void PipelineLayout::Shutdown()
    {
        vkDestroyPipelineLayout(LogicalDevice::GetVkDevice(), m_pipelineLayout, nullptr);
    }
}