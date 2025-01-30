// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Validation/Assert.h"
#include "Debugging/Logger.h"

#include <vulkan/vulkan.h>

module jpt.Vulkan.PhysicalDevice;

import jpt.Application;
import jpt.Renderer_Vulkan;

import jpt.Vulkan.Extensions;
import jpt.Vulkan.SwapChain.SupportDetails;

import jpt.DynamicArray;
import jpt.HashSet;

namespace jpt::Vulkan
{
	bool PhysicalDevice::Init()
	{
		const Renderer_Vulkan* pVulkanRenderer = GetVkRenderer();
		const VkInstance vkInstance = pVulkanRenderer->GetVkInstance();

		// Find a GPU that supports Vulkan
		uint32 deviceCount = 0;
		vkEnumeratePhysicalDevices(vkInstance, &deviceCount, nullptr);
		if (deviceCount == 0)
		{
			JPT_ERROR("Failed to find GPUs with Vulkan support!");
			return false;
		}

		DynamicArray<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(vkInstance, &deviceCount, devices.Buffer());

		DevicePicker bestDevice;
		for (const VkPhysicalDevice& device : devices)
		{
			if (Optional<DevicePicker> picker = ScoreDevice(device))
			{
				if (picker.Value().score > bestDevice.score)
				{
					bestDevice = picker.Value();
				}
			}
		}

		m_physicalDevice = bestDevice.device;
		m_msaaSamples = FindMaxUsableSampleCount();
		JPT_INFO("GPU: %s", bestDevice.deviceName.ConstBuffer());

		if (m_physicalDevice == VK_NULL_HANDLE)
		{
			JPT_ERROR("Failed to find a discrete GPU!");
			return false;
		}

		// Find the queue family that supports graphics
		uint32 queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyCount, nullptr);

		DynamicArray<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyCount, queueFamilies.Buffer());

		for (uint32 i = 0; i < queueFamilyCount; ++i)
		{
			// VK_QUEUE_GRAPHICS_BIT or VK_QUEUE_COMPUTE_BIT implicitly support VK_QUEUE_TRANSFER_BIT
			if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				m_grahicsFamilyIndex = i;
				break;
			}
		}

		if (m_grahicsFamilyIndex == UINT32_MAX)
		{
			JPT_ERROR("Failed to find a queue family that supports graphics!");
			return false;
		}

		return true;
	}

	uint32 PhysicalDevice::FindPresentFamilyIndex(VkSurfaceKHR surface) const
	{
		uint32 queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyCount, nullptr);

		DynamicArray<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyCount, queueFamilies.Buffer());

		for (uint32 i = 0; i < queueFamilyCount; ++i)
		{
			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(m_physicalDevice, i, surface, &presentSupport);
			if (presentSupport)
			{
				return i;
			}
		}

		JPT_ASSERT(false, "Failed to find a queue family that supports presentation");
		return UINT32_MAX;
	}

	SwapChainSupportDetails PhysicalDevice::QuerySwapChainSupport(VkSurfaceKHR surface) const
	{
		SwapChainSupportDetails result;

		// Capabilities
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_physicalDevice, surface, &result.capabilities);

		// Format
		uint32 formatCount = 0;
		vkGetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevice, surface, &formatCount, nullptr);
		if (formatCount != 0)
		{
			result.formats.Resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevice, surface, &formatCount, result.formats.Buffer());
		}

		// Present mode
		uint32 presentModeCount = 0;
		vkGetPhysicalDeviceSurfacePresentModesKHR(m_physicalDevice, surface, &presentModeCount, nullptr);
		if (presentModeCount != 0)
		{
			result.presentModes.Resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(m_physicalDevice, surface, &presentModeCount, result.presentModes.Buffer());
		}

		JPT_ASSERT(result.IsValid(), "Swap chain support details are invalid");
		return result;
	}

	uint32 PhysicalDevice::FindMemoryType(uint32 typeFilter, VkMemoryPropertyFlags properties) const
	{
		VkPhysicalDeviceMemoryProperties memoryProperties;
		vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &memoryProperties);
		for (uint32 i = 0; i < memoryProperties.memoryTypeCount; ++i)
		{
			if ((typeFilter & (1 << i)) &&
				(memoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
			{
				return i;
			}
		}

		JPT_ASSERT(false, "Failed to find a suitable memory type");
		return UINT32_MAX;
	}

	VkFormat PhysicalDevice::FindDepthFormat() const
	{
		const DynamicArray<VkFormat> candidates =
		{
			VK_FORMAT_D32_SFLOAT,
			VK_FORMAT_D32_SFLOAT_S8_UINT,
			VK_FORMAT_D24_UNORM_S8_UINT
		};

		const VkImageTiling tiling = VK_IMAGE_TILING_OPTIMAL;
		const VkFormatFeatureFlags features = VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT;

		return FindSupportedFormat(candidates, tiling, features);
	}

	VkFormat PhysicalDevice::FindSupportedFormat(const DynamicArray<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) const
	{
		for (VkFormat format : candidates)
		{
			VkFormatProperties props;
			vkGetPhysicalDeviceFormatProperties(m_physicalDevice, format, &props);

			if (tiling == VK_IMAGE_TILING_LINEAR &&
				(props.linearTilingFeatures & features) == features)
			{
				return format;
			}
			else if (tiling == VK_IMAGE_TILING_OPTIMAL &&
				(props.optimalTilingFeatures & features) == features)
			{
				return format;
			}
		}

		JPT_ASSERT(false, "Failed to find a supported format");
		return VK_FORMAT_UNDEFINED;
	}

	VkSampleCountFlagBits PhysicalDevice::FindMaxUsableSampleCount() const
	{
		const VkPhysicalDeviceProperties properties = GetProperties();
		const VkSampleCountFlags counts = properties.limits.framebufferColorSampleCounts & properties.limits.framebufferDepthSampleCounts;

		if (counts & VK_SAMPLE_COUNT_64_BIT)
		{ 
			return VK_SAMPLE_COUNT_64_BIT; 
		}
		else if (counts & VK_SAMPLE_COUNT_32_BIT)
		{
			return VK_SAMPLE_COUNT_32_BIT;
		}
		else if (counts & VK_SAMPLE_COUNT_16_BIT)
		{
			return VK_SAMPLE_COUNT_16_BIT;
		}
		else if (counts & VK_SAMPLE_COUNT_8_BIT)
		{
			return VK_SAMPLE_COUNT_8_BIT;
		}
		else if (counts & VK_SAMPLE_COUNT_4_BIT)
		{
			return VK_SAMPLE_COUNT_4_BIT;
		}
		else if (counts & VK_SAMPLE_COUNT_2_BIT)
		{
			return VK_SAMPLE_COUNT_2_BIT;
		}
		else
		{
			return VK_SAMPLE_COUNT_1_BIT;
		}
	}

	VkPhysicalDeviceProperties PhysicalDevice::GetProperties() const
	{
		JPT_ASSERT(m_physicalDevice != VK_NULL_HANDLE, "Physical device is not initialized");

		VkPhysicalDeviceProperties properties;
		vkGetPhysicalDeviceProperties(m_physicalDevice, &properties);
		return properties;
	}

	VkPhysicalDeviceFeatures PhysicalDevice::GetFeatures() const
	{
		JPT_ASSERT(m_physicalDevice != VK_NULL_HANDLE, "Physical device is not initialized");

		VkPhysicalDeviceFeatures features;
		vkGetPhysicalDeviceFeatures(m_physicalDevice, &features);
		return features;
	}

	PhysicalDevice& PhysicalDevice::Get()
	{
		Renderer_Vulkan* pVulkanRenderer = GetVkRenderer();
		return pVulkanRenderer->GetPhysicalDevice();
	}

	VkPhysicalDevice PhysicalDevice::GetVkPhysicalDevice()
	{
		return Get().GetHandle();
	}

	Optional<PhysicalDevice::DevicePicker> PhysicalDevice::ScoreDevice(VkPhysicalDevice physicalDevice) const
	{
		// Some devices are not suitable
		if (!AreDeviceExtensionsSupported(physicalDevice))
		{
			return {};
		}

		VkPhysicalDeviceProperties deviceProperties;
		vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);

		VkPhysicalDeviceFeatures deviceFeatures;
		vkGetPhysicalDeviceFeatures(physicalDevice, &deviceFeatures);

		if (!deviceFeatures.geometryShader ||
			!deviceFeatures.fillModeNonSolid ||
			!deviceFeatures.samplerAnisotropy)
		{
			return {};
		}

		DevicePicker devicePicker;
		devicePicker.device = physicalDevice;
		devicePicker.deviceName = deviceProperties.deviceName;

		// Discrete GPUs have a significant performance advantage
		if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
		{
			devicePicker.score += 1000;
		}

		return devicePicker;
	}

	bool PhysicalDevice::AreDeviceExtensionsSupported(VkPhysicalDevice physicalDevice) const
	{
		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);

		DynamicArray<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, availableExtensions.Buffer());

		HashSet<String> requiredExtensions;
		for (const char* extension : g_deviceExtensions)
		{
			requiredExtensions.Add(extension);
		}

		for (const auto& extension : availableExtensions)
		{
			requiredExtensions.Erase(extension.extensionName);
		}

		if (!requiredExtensions.IsEmpty())
		{
			for (const String& extension : requiredExtensions)
			{
				JPT_ERROR("Missing required extension: %s", extension.ConstBuffer());
			}
		}

		return requiredExtensions.IsEmpty();
	}
}