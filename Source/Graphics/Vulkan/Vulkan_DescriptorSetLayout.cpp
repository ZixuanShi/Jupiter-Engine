// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Logger.h"

#include <vulkan/vulkan.h>

module jpt.Vulkan_DescriptorSetLayout;

import jpt.Vulkan_LogicalDevice;

namespace jpt::Vulkan
{
    bool DescriptorSetLayout::Init()
    {
        VkDescriptorSetLayoutBinding uboLayoutBinding{};
        uboLayoutBinding.binding = 0;
        uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        uboLayoutBinding.descriptorCount = 1;
        uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

        VkDescriptorSetLayoutBinding samplerLayoutBinding{};
        samplerLayoutBinding.binding = 1;
        samplerLayoutBinding.descriptorCount = 1;
        samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        samplerLayoutBinding.pImmutableSamplers = nullptr;
        samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

        VkDescriptorSetLayoutBinding bindings[] = { uboLayoutBinding, samplerLayoutBinding };

        VkDescriptorSetLayoutCreateInfo layoutInfo{};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = JPT_ARRAY_COUNT(bindings);
        layoutInfo.pBindings = bindings;

        if (const VkResult result = vkCreateDescriptorSetLayout(LogicalDevice::GetVkDevice(), &layoutInfo, nullptr, &m_descriptorSetLayout); result != VK_SUCCESS)
        {
            JPT_ERROR("Failed to create descriptor set layout: %d", result);
            return false;
        }

        return true;
    }

    void DescriptorSetLayout::Terminate()
    {
        vkDestroyDescriptorSetLayout(LogicalDevice::GetVkDevice(), m_descriptorSetLayout, nullptr);
    }
}