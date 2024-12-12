// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Logger.h"

#include <vulkan/vulkan.h>

export module jpt.Vulkan.PhysicalDevice;

import jpt.DynamicArray;
import jpt.TypeDefs;

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

		VkPhysicalDevice GetHandle() const { return m_physicalDevice; }
		uint32 GetGraphicsFamilyIndex() const { return m_grahicsFamilyIndex; }
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
			VkPhysicalDeviceProperties deviceProperties;
			vkGetPhysicalDeviceProperties(device, &deviceProperties);

			if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
			{
				m_physicalDevice = device;
				JPT_INFO("Setting Vulkan's Physical device to \"%s\"", deviceProperties.deviceName);
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
}