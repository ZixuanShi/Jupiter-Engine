// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <vulkan/vulkan.h>

export module jpt.Vulkan.DebugMessenger;

export namespace jpt::Vulkan
{
	class DebugMessenger
	{
	private:
		VkDebugUtilsMessengerEXT m_debugMessenger = VK_NULL_HANDLE;

	public:
		bool Init();
		void Shutdown();

		static VkDebugUtilsMessengerCreateInfoEXT MakeCreateInfo();

		static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData);
	};
}