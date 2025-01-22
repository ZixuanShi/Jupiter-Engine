// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <vulkan/vulkan.h>

export module jpt.Vulkan.PhysicalDevice;

import jpt.Vulkan.Extensions;
import jpt.Vulkan.SwapChain.SupportDetails;

import jpt.TypeDefs;
import jpt.String;
import jpt.Optional;

export namespace jpt::Vulkan
{
	/** Actual GPU unit hardware 
		- Query for swapchain support, memory properties, etc.	*/
	class PhysicalDevice
	{
	private:
		struct DevicePicker
		{
			VkPhysicalDevice device = VK_NULL_HANDLE;
			uint32 score = 0;
			String deviceName;

			constexpr bool operator<(const DevicePicker& other) const { return score < other.score; }
			constexpr bool operator>(const DevicePicker& other) const { return score > other.score; }
		};

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
		Optional<DevicePicker> ScoreDevice(VkPhysicalDevice physicalDevice) const;
		bool AreDeviceExtensionsSupported(VkPhysicalDevice physicalDevice) const;
		VkSampleCountFlagBits FindMaxUsableSampleCount() const;
	};
}