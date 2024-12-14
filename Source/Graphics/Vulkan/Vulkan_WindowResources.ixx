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

import jpt.Vulkan.SwapChain;
import jpt.Vulkan.SwapChain.SupportDetails;
import jpt.Vulkan.CommandPool;

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
		SwapChain m_swapChain;
		CommandPool m_commandPool;

	public:
		bool Init(Window* pWindow, VkInstance instance, 
			const PhysicalDevice& physicalDevice, const LogicalDevice& logicalDevice, const RenderPass& renderPass);

		void Shutdown(VkInstance instance, const LogicalDevice& logicalDevice);

	public:
		Window* GetOwner() const { return m_pOwner; }
	};

	bool WindowResources::Init(Window* pWindow, VkInstance instance, 
		const PhysicalDevice& physicalDevice, const LogicalDevice& logicalDevice, const RenderPass& renderPass)
	{
		// Surface
		m_pOwner = pWindow;
		pWindow->CreateSurface({ instance, &m_surface });

		// Present queue
		const uint32 presentFamilyIndex = physicalDevice.FindPresentFamilyIndex(m_surface);
		vkGetDeviceQueue(logicalDevice.GetHandle(), presentFamilyIndex, 0, &m_presentQueue);

		// SwapChain
		m_swapChain.Init(m_pOwner, physicalDevice, logicalDevice, m_surface);
		m_swapChain.CreateImageViews(logicalDevice);
		m_swapChain.CreateFramebuffers(logicalDevice, renderPass);

		// Command pool
		m_commandPool.Init(logicalDevice, physicalDevice.GetGraphicsFamilyIndex());

		return true;
	}

	void WindowResources::Shutdown(VkInstance instance, const LogicalDevice& logicalDevice)
	{
		m_commandPool.Shutdown(logicalDevice);

		m_swapChain.Shutdown(logicalDevice);
		
		vkDestroySurfaceKHR(instance, m_surface, nullptr);
		m_surface = VK_NULL_HANDLE;
	}
}