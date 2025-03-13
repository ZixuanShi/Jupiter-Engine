// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <vulkan/vulkan.h>

export module jpt.Vulkan.DescriptorSet;

import jpt.Vulkan.UniformBuffer;

export namespace jpt::Vulkan
{
    /** Descriptor Set is an actual collection of resource bindings:
        - Contains the actual pointers/references to our resources (uniform buffers, textures, etc.)
        - Can be thought of as a "filled out" version of the layout
        - Can be switched efficiently during rendering */
    class DescriptorSet
    {
    private:
        VkDescriptorSet m_descriptorSet = VK_NULL_HANDLE;
        VkDescriptorPool m_descriptorPool = VK_NULL_HANDLE;

    public:
        bool Init(const UniformBuffer& uniformBuffer);
        void Shutdown();

    public:
        VkDescriptorSet GetHandle() const { return m_descriptorSet; }
        VkDescriptorSet* GetHandlePtr() { return &m_descriptorSet; }
    };
}