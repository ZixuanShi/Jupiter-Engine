// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module; 

#include <vulkan/vulkan.h>

export module jpt.Vulkan_UniformBuffer;

import jpt.Vulkan_Buffer;

import jpt.Matrix44;

export namespace jpt::Vulkan
{
    struct alignas(16) Uniform_MVP
    {
    public:
        Matrix44 model;
        Matrix44 view;
        Matrix44 proj;
    };

    /** Holds constant across all shaders. Think of it like a global configuration that all our shaders can read from */
    class UniformBuffer
    {
    private:
        Buffer m_buffer;
        void* m_mappedMemory = nullptr;

    public:
        bool Init();
        void Shutdown();

        void MapMemory(void* pSource, VkDeviceSize size);
        VkBuffer GetHandle() const { return m_buffer.GetHandle(); }
    };
}