// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Validation/Assert.h"
#include "Debugging/Logger.h"

#include <vulkan/vulkan.h>

export module jpt.Vulkan.WindowResources;

import jpt.Window;

import jpt.Graphics.Constants;

import jpt.Vulkan.PhysicalDevice;
import jpt.Vulkan.LogicalDevice;

export namespace jpt::Vulkan
{
	/** Per-Window specific Vulkan resource. Each Window should have its own data */
	class WindowResources
	{
	private:
		Window* m_pOwner = nullptr;

	private:
		VkSurfaceKHR m_surface = VK_NULL_HANDLE;
		VkQueue m_presentQueue = VK_NULL_HANDLE;

	public:
		bool Init(Window* pWindow, VkInstance instance, const PhysicalDevice& physicalDevice, const LogicalDevice& logicalDevice);
		void Shutdown(VkInstance instance);

	public:
		Window* GetOwner() const { return m_pOwner; }
	};

	bool WindowResources::Init(Window* pWindow, VkInstance instance, const PhysicalDevice& physicalDevice, const LogicalDevice& logicalDevice)
	{
		// Surface
		m_pOwner = pWindow;
		pWindow->CreateSurface({ instance, &m_surface });

		// Present queue
		uint32 presentFamilyIndex = physicalDevice.FindPresentFamilyIndex(m_surface);
		JPT_ASSERT(presentFamilyIndex != UINT32_MAX);
		vkGetDeviceQueue(logicalDevice.GetHandle(), presentFamilyIndex, 0, &m_presentQueue);

		return true;
	}

	void WindowResources::Shutdown(VkInstance instance)
	{
		vkDestroySurfaceKHR(instance, m_surface, nullptr);
	}
}