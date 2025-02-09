// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <vulkan/vulkan.h>

export module jpt.Vulkan.PhysicalDevice;

import jpt.Vulkan.Extensions;
import jpt.Vulkan.SwapChain.SupportDetails;

import jpt.TypeDefs;
import jpt.DynamicArray;

export namespace jpt::Vulkan
{
	/** Actual GPU unit hardware 
		- Find the queue family indices
		- Query for swapchain support, memory properties, etc.	*/
	class PhysicalDevice
	{
	private:
		VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
		uint32 m_grahicsFamilyIndex = UINT32_MAX;
		VkSampleCountFlagBits m_msaaSamples = VK_SAMPLE_COUNT_1_BIT;

	public:
		bool Init();

		uint32 FindPresentFamilyIndex(VkSurfaceKHR surface) const;
		SwapChainSupportDetails QuerySwapChainSupport(VkSurfaceKHR surface) const;
		uint32 FindMemoryType(uint32 typeFilter, VkMemoryPropertyFlags properties) const;
		VkFormat FindDepthFormat() const;
		VkFormat FindSupportedFormat(const DynamicArray<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) const;

		VkPhysicalDeviceProperties GetProperties() const;
		VkPhysicalDeviceFeatures GetFeatures() const;

	public:
		VkPhysicalDevice GetHandle() const { return m_physicalDevice; }
		uint32 GetGraphicsFamilyIndex() const { return m_grahicsFamilyIndex; }
		VkSampleCountFlagBits GetMsaaSamples() const { return m_msaaSamples; }

		static PhysicalDevice& Get();
		static VkPhysicalDevice GetVkPhysicalDevice();

	private:
		VkSampleCountFlagBits FindMaxUsableSampleCount() const;
	};
}