// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <vulkan/vulkan.h>

export module jpt.Vulkan_VertexBuffer;

import jpt.Vulkan_Buffer;

import jpt.DynamicArray;
import jpt.Vertex;

export namespace jpt::Vulkan
{
    class VertexBuffer
    {
    private:
        Buffer m_buffer;

    public:
        bool Init(const DynamicArray<Vertex>& vertices);

        void Terminate();

    public:
        VkBuffer GetBuffer() { return m_buffer.GetHandle(); }
    };
}