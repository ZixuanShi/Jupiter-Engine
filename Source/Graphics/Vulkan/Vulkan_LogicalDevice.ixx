// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <vulkan/vulkan.h>

export module jpt.Vulkan.LogicalDevice;

export namespace jpt::Vulkan
{
	/** Logical device that interfaces with the physical device
		- Create and use queues	*/
	class LogicalDevice
	{
	private:
		VkDevice m_device = VK_NULL_HANDLE;
		VkQueue m_graphicsQueue = VK_NULL_HANDLE;

	public:
		bool Init();
		void Shutdown();
		void WaitIdle() const;

		VkDevice GetHandle() const { return m_device; }
		VkQueue GetGraphicsQueue() const { return m_graphicsQueue; }

		static LogicalDevice& Get();
		static VkDevice GetVkDevice();
		static VkQueue GetGraphicsVkQueue();
	};
}