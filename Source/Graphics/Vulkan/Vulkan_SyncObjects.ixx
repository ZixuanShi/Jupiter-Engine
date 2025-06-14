// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <vulkan/vulkan.h>

export module jpt.Vulkan_SyncObjects;

export namespace jpt::Vulkan
{
    /** Synchronization objects that are used to coordinate operations between the CPU and the GPU
        - VkDeviceWaitIdle: Blocks the CPU until the GPU has finished all operations. Use cases: Clean up resources, exit the application
        - Semaphore: GPU-GPU. Coordinate queues. i.e. Waiting for a render pass to complete before starting post-processing. Can't be waited by the CPU. Use cases: Synchronization between queues, synchronization between command buffers
        - Fence: GPU-CPU. Signal completion of GPU operations back to CPU. Can be waited by vkWaitForFence. Use cases: Command buffer submission/reuse */
    class SyncObjects
    {
    private:
        VkSemaphore m_imageAvailableSemaphore = VK_NULL_HANDLE;    /**< Signaled when the presentation engine is finished using an image */
        VkSemaphore m_renderFinishedSemaphore = VK_NULL_HANDLE; /**< Signaled when the rendering operation is finished */
        VkFence m_inFlightFence = VK_NULL_HANDLE; /**< Signaled when a command buffer finishes execution */

    public:
        bool Init();
        void Shutdown();

    public:
        VkSemaphore GetImageAvailableSemaphore() const { return m_imageAvailableSemaphore; }
        VkSemaphore GetRenderFinishedSemaphore() const { return m_renderFinishedSemaphore; }
        VkFence GetInFlightFence() const { return m_inFlightFence; }
        VkFence* GetInFlightFencePtr() { return &m_inFlightFence; }
    };
}