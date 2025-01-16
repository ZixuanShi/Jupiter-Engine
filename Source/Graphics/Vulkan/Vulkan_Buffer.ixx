// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <vulkan/vulkan.h>

export module jpt.Vulkan.Buffer;

export namespace jpt::Vulkan
{
	class Buffer
	{
	private:
		VkBuffer m_buffer = VK_NULL_HANDLE;
		VkDeviceMemory m_bufferMemory = VK_NULL_HANDLE;

	public:
		VkResult Create(const VkBufferCreateInfo& createInfo, VkMemoryPropertyFlags properties);
		void Copy(VkBuffer srcBuffer, VkDeviceSize size);
		void MapMemory(const void* pPtr, VkDeviceSize size);

		void Shutdown();

	public:
		VkBuffer GetHandle() const { return m_buffer; }
		VkDeviceMemory GetMemory() const { return m_bufferMemory; }
	};
}