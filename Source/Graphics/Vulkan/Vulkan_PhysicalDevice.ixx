// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Logger.h"

#include <vulkan/vulkan.h>

export module jpt.Vulkan.PhysicalDevice;

import jpt.Vulkan.Extensions;

import jpt.DynamicArray;
import jpt.HashSet;
import jpt.TypeDefs;
import jpt.String;

export namespace jpt::Vulkan
{
	/** Actual GPU unit hardware 
		- Query for swapchain support, memory properties, etc.	*/
	class PhysicalDevice
	{
	private:
		VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
		uint32 m_grahicsFamilyIndex = UINT32_MAX;

	public:
		bool Init(VkInstance instance);

		uint32 FindPresentFamilyIndex(VkSurfaceKHR surface) const;

	public:
		VkPhysicalDevice GetHandle() const { return m_physicalDevice; }
		uint32 GetGraphicsFamilyIndex() const { return m_grahicsFamilyIndex; }

	private:
		bool IsDeviceSuitable(VkPhysicalDevice physicalDevice) const;
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

		for (const VkPhysicalDevice& device : devices)
		{
			if (IsDeviceSuitable(device))
			{
				m_physicalDevice = device;
				break;
			}
		}

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

		return UINT32_MAX;
	}

	bool PhysicalDevice::IsDeviceSuitable(VkPhysicalDevice physicalDevice) const
	{
		VkPhysicalDeviceProperties deviceProperties;
		vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);

		const bool isDiscreteGPU = (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU);
		const bool areDeviceExtensionsSupported = AreDeviceExtensionsSupported(physicalDevice);

		const bool isSuitable = isDiscreteGPU && areDeviceExtensionsSupported;
		if (isSuitable)
		{
			JPT_INFO("Setting Vulkan's Physical device to \"%s\"", deviceProperties.deviceName);
		}

		return isSuitable;
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