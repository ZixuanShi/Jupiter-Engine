// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <vulkan/vulkan.h>

export module jpt.Vulkan.DescriptorSetLayout;

export namespace jpt::Vulkan
{
    /** Descriptor Set Layout defines the "interface" or structure of resources your shader expects:
        - It specifies what types of resources (uniform buffers, textures, etc.) will be bound
        - Where they'll be bound (binding numbers)
        - What shader stages can access them
        - How many resources of each type */
    class DescriptorSetLayout
    {
    private:
        VkDescriptorSetLayout m_descriptorSetLayout = VK_NULL_HANDLE;

    public:
        bool Init();
        void Shutdown();

    public:
        VkDescriptorSetLayout GetHandle() const { return m_descriptorSetLayout; }
        const VkDescriptorSetLayout* GetHandlePtr() const { return &m_descriptorSetLayout; }
    };
}