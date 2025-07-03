// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module; 

#include <vulkan/vulkan.h>

export module jpt.Vulkan_PipelineLayout;

export namespace jpt::Vulkan
{
    class PipelineLayout
    {
    private:
        VkPipelineLayout m_pipelineLayout = VK_NULL_HANDLE;

    public:
        bool Init();
        void Terminate();

    public:
        VkPipelineLayout GetHandle() const { return m_pipelineLayout; }
    };
}