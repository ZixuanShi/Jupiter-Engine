// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Logger.h"

#include <vulkan/vulkan.h>

export module jpt.Vulkan.PhysicalDevice;

import jpt.Vulkan.QueueFamilyIndices;
import jpt.Vulkan.SwapChainSupportDetails;
import jpt.Vulkan.Helpers;
import jpt.Vulkan.ValidationLayers;

import jpt.DynamicArray;
import jpt.TypeDefs;

export namespace jpt::Vulkan
{
	/** Encapsulates Vulkan's physical device (GPU)
	    - Has queue families (just describes what's available)
		- Only tells you what queue families exist and their properties */
	class PhysicalDevice
	{
	private:
		QueueFamilyIndices m_queueFamilyIndices;
		VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;

	public:
		bool Init(VkInstance instance, VkSurfaceKHR surface);

		VkPhysicalDevice Get() const;
		const QueueFamilyIndices& GetQueueFamilyIndices() const;

		// TODO: Move to swapchain abstraction class when implemented
		SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface) const;

	private:
		bool IsDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface) const;
		QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface) const;
	};

	bool PhysicalDevice::Init(VkInstance instance, VkSurfaceKHR surface)
	{
		// Get the number of physical devices
		uint32 deviceCount = 0;
		vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
		if (deviceCount == 0)
		{
			JPT_ERROR("Failed to find GPUs with Vulkan support");
			return false;
		}

		// For each, check if it's suitable then assign
		DynamicArray<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(instance, &deviceCount, devices.Buffer());
		for (const VkPhysicalDevice& device : devices)
		{
			if (IsDeviceSuitable(device, surface))
			{
				m_physicalDevice = device;
				break;
			}
		}

		if (m_physicalDevice == VK_NULL_HANDLE)
		{
			JPT_ERROR("Failed to find a suitable GPU");
			return false;
		}

		m_queueFamilyIndices = FindQueueFamilies(m_physicalDevice, surface);
		return true;
	}

	VkPhysicalDevice PhysicalDevice::Get() const
	{
		return m_physicalDevice;
	}

	const QueueFamilyIndices& PhysicalDevice::GetQueueFamilyIndices() const
	{
		return m_queueFamilyIndices;
	}

	SwapChainSupportDetails PhysicalDevice::QuerySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface) const
	{
		SwapChainSupportDetails details;
		details.Init(device, surface);
		return details;
	}

	bool PhysicalDevice::IsDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface) const
	{
		QueueFamilyIndices indices = FindQueueFamilies(device, surface);

		bool extensionsSupported = CheckDeviceExtensionSupport(device);
		SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(device, surface);

		return indices.IsComplete() && extensionsSupported && swapChainSupport.IsValid();
	}

	QueueFamilyIndices PhysicalDevice::FindQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface) const
	{
		QueueFamilyIndices indices;

		uint32 queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

		DynamicArray<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.Buffer());

		uint32 i = 0;
		for (const VkQueueFamilyProperties& queueFamily : queueFamilies)
		{
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				indices.graphicsFamily = i;
			}

			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
			if (presentSupport)
			{
				indices.presentFamily = i;
			}

			if (indices.IsComplete())
			{
				break;
			}

			++i;
		}

		return indices;
	}
}