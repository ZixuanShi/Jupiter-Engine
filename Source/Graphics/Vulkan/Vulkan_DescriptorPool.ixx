// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <vulkan/vulkan.h>

export module jpt.Vulkan_DescriptorPool;

export namespace jpt::Vulkan
{
    /** Memory Pool for descriptors */
    class DescriptorPool
    {
    private:
        VkDescriptorPool m_descriptorPool = VK_NULL_HANDLE;

    public:
        bool Init();
        void Shutdown();

    public:
        VkDescriptorPool GetHandle() const { return m_descriptorPool; }
    };
}