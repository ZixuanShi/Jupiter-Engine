// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Validation/Assert.h"
#include "Debugging/Logger.h"

#include <vulkan/vulkan.h>

export module jpt.Vulkan.PhysicalDevice;

import jpt.Vulkan.Extensions;
import jpt.Vulkan.SwapChain.SupportDetails;

import jpt.DynamicArray;
import jpt.Heap;
import jpt.HashSet;
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

	public:
		bool Init(VkInstance instance);

		uint32 FindPresentFamilyIndex(VkSurfaceKHR surface) const;
		SwapChainSupportDetails QuerySwapChainSupport(VkSurfaceKHR surface) const;
		uint32 FindMemoryType(uint32 typeFilter, VkMemoryPropertyFlags properties) const;

		VkPhysicalDeviceProperties GetProperties() const;
		VkPhysicalDeviceFeatures GetFeatures() const;

	public:
		VkPhysicalDevice GetHandle() const { return m_physicalDevice; }
		uint32 GetGraphicsFamilyIndex() const { return m_grahicsFamilyIndex; }

	private:
		Optional<DevicePicker> ScoreDevice(VkPhysicalDevice physicalDevice) const;
		bool AreDeviceExtensionsSupported(VkPhysicalDevice physicalDevice) const;
	};

	bool PhysicalDevice::Init(VkInstance instance)
	{
		// Find a GPU that supports Vulkan
		uint32 deviceCount = 0;
		vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
		if (deviceCount == 0)
		{
			JPT_ERROR("Failed to find GPUs with Vulkan support!");
			return false;
		}

		DynamicArray<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(instance, &deviceCount, devices.Buffer());

		MaxHeap<DevicePicker> maxHeap;
		for (const VkPhysicalDevice& device : devices)
		{
			if (Optional<DevicePicker> picker = ScoreDevice(device))
			{
				maxHeap.Emplace(picker.Value());
			}
		}

		JPT_ASSERT(!maxHeap.IsEmpty(), "Failed to find a suitable GPU!");

		m_physicalDevice = maxHeap.Top().device;
		JPT_INFO("Setting Vulkan's Physical device to \"%s\"", maxHeap.Top().deviceName.ConstBuffer());

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

		if (!deviceFeatures.geometryShader || !deviceFeatures.fillModeNonSolid)
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