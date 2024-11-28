// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Logger.h"

#include <vulkan/vulkan.h>

export module jpt.Vulkan.Device;

import jpt.Vulkan.QueueFamilyIndices;

import jpt.DynamicArray;
import jpt.TypeDefs;

export namespace jpt::Vulkan
{
	/** Encapsulates Vulkan's physical device */
	class PhysicalDevice
	{
	private:
		VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;

	public:
		bool Init(VkInstance instance);


		//QueueFamilyIndices FindQueueFamilies() const;

		// QuerySwapChainSupport

	private:
		bool IsDeviceSuitable(VkPhysicalDevice ) const { return true; }
	};

	bool PhysicalDevice::Init(VkInstance instance)
	{
		uint32 deviceCount = 0;
		vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

		if (deviceCount == 0)
		{
			JPT_ERROR("Failed to find GPUs with Vulkan support");
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

		JPT_ENSURE(m_physicalDevice != VK_NULL_HANDLE, "Failed to find a suitable GPU");

		return true;
	}
}