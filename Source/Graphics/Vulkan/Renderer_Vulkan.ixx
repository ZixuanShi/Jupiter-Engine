// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Applications/App/Application.h"
#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Logger.h"
#include "Debugging/Assert.h"

#include <vulkan/vulkan.h>

export module jpt.Renderer_Vulkan;

import jpt.Renderer;

#if IS_PLATFORM_WIN64
	import jpt.Framework_GLFW;
#endif

import jpt.Vulkan.Helpers;
import jpt.Vulkan.ValidationLayers;
import jpt.Vulkan.QueueFamilyIndices;
import jpt.Vulkan.SwapChainSupportDetails;

import jpt.TypeDefs;

import jpt.DynamicArray;
import jpt.HashSet;

using namespace jpt::Vulkan;

export namespace jpt
{
	class Renderer_Vulkan final : public Renderer
	{
		using Super = Renderer;

	private:
		QueueFamilyIndices m_queueFamilyIndices;

		DynamicArray<VkImage> m_swapChainImages;
		VkSwapchainKHR m_swapChain;
		VkFormat m_swapChainImageFormat;
		VkExtent2D m_swapChainExtent;

		DynamicArray<VkImageView> m_swapChainImageViews;

		VkInstance m_instance;
		VkSurfaceKHR m_surface;
		
		VkDevice m_logicalDevice;
		VkPhysicalDevice m_physicalDevice;
		
		VkQueue m_graphicsQueue;
		VkQueue m_presentQueue;

#if !IS_RELEASE
		VkDebugUtilsMessengerEXT m_debugMessenger;
#endif

	public:
		virtual bool Init() override;
		virtual void Shutdown() override;

		static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, 
			                                                VkDebugUtilsMessageTypeFlagsEXT messageType, 
			                                                const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, 
			                                                void* pUserData);

	private:
		// Initialization
		bool CreateInstance();
		bool CreateSurface();
		bool PickPhysicalDevice();
		bool CreateLogicalDevice();
		bool CreateSwapChain();
		bool CreateImageViews();

		// Vulkan helpers
		QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
		bool IsDeviceSuitable(VkPhysicalDevice device);
		SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);

		// Debugging
#if !IS_RELEASE
		void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
		bool SetupDebugMessenger();

		VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
			                                  const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
			                                  const VkAllocationCallbacks* pAllocator,
			                                  VkDebugUtilsMessengerEXT* pCallback);

		void DestroyDebugMessenger(VkInstance instance, 
			                       VkDebugUtilsMessengerEXT callback, 
			                       const VkAllocationCallbacks* pAllocator);
#endif
	};

	bool Renderer_Vulkan::Init()
	{
		JPT_ENSURE(Super::Init());

		uint32 glfwExtensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &glfwExtensionCount, nullptr);
		JPT_INFO("GLFW extensions count: %i", glfwExtensionCount);

		bool success = true;
		success &= CreateInstance();
#if !IS_RELEASE
		success &= SetupDebugMessenger();
#endif
		success &= CreateSurface();
		success &= PickPhysicalDevice();
		m_queueFamilyIndices = FindQueueFamilies(m_physicalDevice);
		success &= CreateLogicalDevice();
		success &= CreateSwapChain();
		success &= CreateImageViews();

		if (success)
		{
			JPT_INFO("Vulkan renderer initialized successfully");
		}

		return success;
	}

	void Renderer_Vulkan::Shutdown()
	{
		Super::Shutdown();

		for (VkImageView imageView : m_swapChainImageViews)
		{
			vkDestroyImageView(m_logicalDevice, imageView, nullptr);
		}

		vkDestroySwapchainKHR(m_logicalDevice, m_swapChain, nullptr);
		vkDestroyDevice(m_logicalDevice, nullptr);

#if !IS_RELEASE
		DestroyDebugMessenger(m_instance, m_debugMessenger, nullptr);
#endif

		vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
		vkDestroyInstance(m_instance, nullptr);
	}

	VKAPI_ATTR VkBool32 VKAPI_CALL Renderer_Vulkan::DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		                                                          [[maybe_unused]] VkDebugUtilsMessageTypeFlagsEXT messageType,
		                                                          const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, 
		                                                          [[maybe_unused]] void* pUserData)
	{
		switch (messageSeverity)
		{
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT: 
			[[fallthrough]];
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
			JPT_INFO("Validation layer: %s", pCallbackData->pMessage);
			break;

		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
			JPT_WARN("Validation layer: %s", pCallbackData->pMessage);
			break;

		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
			JPT_ERROR("Validation layer: %s", pCallbackData->pMessage);
			break;

		default:
			break;
		}

		return VK_FALSE;
	}

	bool Renderer_Vulkan::CreateInstance()
	{
#if !IS_RELEASE
		if (!CheckValidationLayerSupport())
		{
			JPT_ERROR("Validation layers requested, but not available");
			return false;
		}
#endif

		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Jupiter";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "Jupiter Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		const DynamicArray<const char*> extensions = GetRequiredExtensions();
		VkInstanceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;
		createInfo.enabledExtensionCount = static_cast<uint32>(extensions.Count());
		createInfo.ppEnabledExtensionNames = extensions.ConstBuffer();

#if !IS_RELEASE
		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
		createInfo.enabledLayerCount = static_cast<uint32>(validationLayers.Count());
		createInfo.ppEnabledLayerNames = validationLayers.ConstBuffer();

		PopulateDebugMessengerCreateInfo(debugCreateInfo);
		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
#else
		createInfo.enabledLayerCount = 0;
		createInfo.pNext = nullptr;
#endif

		const VkResult result = vkCreateInstance(&createInfo, nullptr, &m_instance);
		if (result != VK_SUCCESS)
		{
			JPT_ERROR("Failed to create Vulkan instance! VkResult: %i", static_cast<uint32>(result));
			return false;
		}

		return true;
	}

	bool Renderer_Vulkan::CreateSurface()
	{
		const VkResult result = Framework_GLFW::CreateWindowSurface(m_instance, &m_surface);
		if (result != VK_SUCCESS)
		{
			JPT_ERROR("Failed to create window surface! VkResult: %i", static_cast<uint32>(result));
			return false;
		}

		return true;
	}

	bool Renderer_Vulkan::PickPhysicalDevice()
	{
		uint32 deviceCount = 0;
		vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);

		if (deviceCount == 0)
		{
			JPT_ERROR("Failed to find GPUs with Vulkan support");
			return false;
		}

		DynamicArray<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(m_instance, &deviceCount, devices.Buffer());
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
			JPT_ERROR("Failed to find a suitable GPU");
			return false;
		}

		return true;
	}

	bool Renderer_Vulkan::CreateLogicalDevice()
	{
		DynamicArray<VkDeviceQueueCreateInfo> queueCreateInfos;
		HashSet<uint32> uniqueQueueFamilies = { m_queueFamilyIndices.graphicsFamily.Value(), m_queueFamilyIndices.presentFamily.Value() };

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

		const VkResult result = vkCreateDevice(m_physicalDevice, &createInfo, nullptr, &m_logicalDevice);
		if (result != VK_SUCCESS)
		{
			JPT_ERROR("Failed to create logical device! VkResult: %i", static_cast<uint32>(result));
			return false;
		}

		vkGetDeviceQueue(m_logicalDevice, m_queueFamilyIndices.graphicsFamily.Value(), 0, &m_graphicsQueue);
		vkGetDeviceQueue(m_logicalDevice, m_queueFamilyIndices.presentFamily.Value(), 0, &m_presentQueue);
		return true;
	}

	bool Renderer_Vulkan::CreateSwapChain()
	{
		SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(m_physicalDevice);

		const VkSurfaceFormatKHR surfaceFormat = swapChainSupport.ChooseSwapSurfaceFormat();
		const VkPresentModeKHR presentMode = swapChainSupport.ChooseSwapPresentMode();
		const VkExtent2D extent = swapChainSupport.ChooseSwapExtent();

		uint32 imageCount = swapChainSupport.GetImageCount();

		VkSwapchainCreateInfoKHR createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = m_surface;
		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = extent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		uint32 queueFamilyIndices[] = { m_queueFamilyIndices.graphicsFamily.Value(), m_queueFamilyIndices.presentFamily.Value() };

		if (m_queueFamilyIndices.graphicsFamily != m_queueFamilyIndices.presentFamily)
		{
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = queueFamilyIndices;
		}
		else
		{
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			createInfo.queueFamilyIndexCount = 0;
			createInfo.pQueueFamilyIndices = nullptr;
		}

		createInfo.preTransform = swapChainSupport.GetTransform();
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE;
		createInfo.oldSwapchain = VK_NULL_HANDLE;

		const VkResult result = vkCreateSwapchainKHR(m_logicalDevice, &createInfo, nullptr, &m_swapChain);
		if (result != VK_SUCCESS)
		{
			JPT_ERROR("Failed to create swap chain! VkResult: %i", static_cast<uint32>(result));
			return false;
		}

		vkGetSwapchainImagesKHR(m_logicalDevice, m_swapChain, &imageCount, nullptr);
		m_swapChainImages.Resize(imageCount);
		vkGetSwapchainImagesKHR(m_logicalDevice, m_swapChain, &imageCount, m_swapChainImages.Buffer());

		m_swapChainImageFormat = surfaceFormat.format;
		m_swapChainExtent = extent;

		return true;
	}

	bool Renderer_Vulkan::CreateImageViews()
	{
		m_swapChainImageViews.Resize(m_swapChainImages.Count());

		for (uint32 i = 0; i < m_swapChainImages.Count(); ++i)
		{
			VkImageViewCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			createInfo.image = m_swapChainImages[i];
			createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			createInfo.format = m_swapChainImageFormat;
			createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			createInfo.subresourceRange.baseMipLevel = 0;
			createInfo.subresourceRange.levelCount = 1;
			createInfo.subresourceRange.baseArrayLayer = 0;
			createInfo.subresourceRange.layerCount = 1;

			const VkResult result = vkCreateImageView(m_logicalDevice, &createInfo, nullptr, &m_swapChainImageViews[i]);
			if (result != VK_SUCCESS)
			{
				JPT_ERROR("Failed to create image views! VkResult: %i", static_cast<uint32>(result));
				return false;
			}
		}

		return true;
	}

	QueueFamilyIndices Renderer_Vulkan::FindQueueFamilies(VkPhysicalDevice device)
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
			vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_surface, &presentSupport);
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

	bool Renderer_Vulkan::IsDeviceSuitable(VkPhysicalDevice device)
	{
		QueueFamilyIndices indices = FindQueueFamilies(device);

		bool extensionsSupported = CheckDeviceExtensionSupport(device);
		SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(device);

		return indices.IsComplete() && extensionsSupported && swapChainSupport.IsValid();
	}

	SwapChainSupportDetails Renderer_Vulkan::QuerySwapChainSupport(VkPhysicalDevice device)
	{
		SwapChainSupportDetails details;
		details.Init(device, m_surface);
		return details;
	}

#if !IS_RELEASE
	void Renderer_Vulkan::PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
	{
		createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;

		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

		createInfo.pfnUserCallback = DebugCallback;
		createInfo.pUserData = nullptr; // Optional
	}

	bool Renderer_Vulkan::SetupDebugMessenger()
	{
		VkDebugUtilsMessengerCreateInfoEXT createInfo;
		PopulateDebugMessengerCreateInfo(createInfo);

		const VkResult result = CreateDebugUtilsMessengerEXT(m_instance, &createInfo, nullptr, &m_debugMessenger);
		if (result != VK_SUCCESS)
		{
			JPT_ERROR("Failed to set up debug messenger");
			return false;
		}

		return true;
	}

	VkResult Renderer_Vulkan::CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pCallback)
	{
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_instance, "vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr)
		{
			return func(instance, pCreateInfo, pAllocator, pCallback);
		}
		else
		{
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}

	void Renderer_Vulkan::DestroyDebugMessenger(VkInstance instance, VkDebugUtilsMessengerEXT callback, const VkAllocationCallbacks* pAllocator)
	{
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr)
		{
			func(instance, callback, pAllocator);
		}
	}
#endif
}