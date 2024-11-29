// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Logger.h"

#include <vulkan/vulkan.h>

export module jpt.Vulkan.LogicalDevice;

import jpt.Vulkan.PhysicalDevice;
import jpt.Vulkan.QueueFamilyIndices;
import jpt.Vulkan.Helpers;
import jpt.Vulkan.ValidationLayers;

import jpt.DynamicArray;
import jpt.HashSet;

import jpt.TypeDefs;

export namespace jpt::Vulkan
{
	/** Vulkan's Logical Device
		- Create and own queue */
	class LogicalDevice
	{
	private:
		VkDevice m_logicalDevice = VK_NULL_HANDLE;
		VkQueue m_graphicsQueue = VK_NULL_HANDLE;
		VkQueue m_presentQueue = VK_NULL_HANDLE;

	public:
		bool Init(PhysicalDevice physicalDevice);
		void Shutdown();

	public:
		void WaitIdle() const;

	public:
		VkDevice Get() const;
		VkQueue GetGraphicsQueue() const;
		VkQueue GetPresentQueue() const;
	};

	bool LogicalDevice::Init(PhysicalDevice physicalDevice)
	{
		const QueueFamilyIndices& queueFamilyIndices = physicalDevice.GetQueueFamilyIndices();
		HashSet<uint32> uniqueQueueFamilies = { queueFamilyIndices.graphicsFamily.Value(), queueFamilyIndices.presentFamily.Value() };

		DynamicArray<VkDeviceQueueCreateInfo> queueCreateInfos;
		float queuePriority = 1.0f;
		for (uint32 queueFamily : uniqueQueueFamilies)
		{
			VkDeviceQueueCreateInfo queueCreateInfo = {};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;

			queueCreateInfos.Add(queueCreateInfo);
		}

		VkPhysicalDeviceFeatures deviceFeatures = {};

		VkDeviceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.queueCreateInfoCount = static_cast<uint32>(queueCreateInfos.Count());
		createInfo.pQueueCreateInfos = queueCreateInfos.ConstBuffer();
		createInfo.pEnabledFeatures = &deviceFeatures;
		createInfo.enabledExtensionCount = static_cast<uint32>(deviceExtensions.Count());
		createInfo.ppEnabledExtensionNames = deviceExtensions.ConstBuffer();

#if !IS_RELEASE
		createInfo.enabledLayerCount = static_cast<uint32>(validationLayers.Count());
		createInfo.ppEnabledLayerNames = validationLayers.ConstBuffer();
#else
		createInfo.enabledLayerCount = 0;
#endif

		if (const VkResult result = vkCreateDevice(physicalDevice.Get(), &createInfo, nullptr, &m_logicalDevice); result != VK_SUCCESS)
		{
			JPT_ERROR("Failed to create logical device! VkResult: %i", static_cast<uint32>(result));
			return false;
		}

		vkGetDeviceQueue(m_logicalDevice, queueFamilyIndices.graphicsFamily.Value(), 0, &m_graphicsQueue);
		vkGetDeviceQueue(m_logicalDevice, queueFamilyIndices.presentFamily.Value(), 0, &m_presentQueue);
		return true;
	}

	void LogicalDevice::Shutdown()
	{
		vkDestroyDevice(m_logicalDevice, nullptr);
	}

	void LogicalDevice::WaitIdle() const
	{
		vkDeviceWaitIdle(m_logicalDevice);
	}

	VkDevice LogicalDevice::Get() const
	{
		return m_logicalDevice;
	}

	VkQueue LogicalDevice::GetGraphicsQueue() const
	{
		return m_graphicsQueue;
	}

	VkQueue LogicalDevice::GetPresentQueue() const
	{
		return m_presentQueue;
	}
}