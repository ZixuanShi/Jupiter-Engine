// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module; 

#include <vulkan/vulkan.h>

export module jpt.Vulkan.PipelineLayout;

export namespace jpt::Vulkan
{
    class PipelineLayout
    {
    private:
        VkPipelineLayout m_pipelineLayout = VK_NULL_HANDLE;

    public:
        bool Init();
        void Shutdown();

    public:
        VkPipelineLayout GetHandle() const { return m_pipelineLayout; }
    };
}