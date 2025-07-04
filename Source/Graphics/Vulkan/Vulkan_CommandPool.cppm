// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <vulkan/vulkan.h>

export module jpt.Vulkan_CommandPool;

import jpt.TypeDefs;

export namespace jpt::Vulkan
{
    /** A command pool is a memory pool that holds command buffers */
    class CommandPool
    {
    private:
        VkCommandPool m_commandPool = VK_NULL_HANDLE;

    public:
        bool Init();
        void Terminate();

    public:
        VkCommandPool GetHandle() const { return m_commandPool; }
    };
}