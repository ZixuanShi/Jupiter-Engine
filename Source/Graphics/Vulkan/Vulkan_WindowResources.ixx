// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <vulkan/vulkan.h>

export module jpt.Vulkan_WindowResources;

import jpt.Graphics_Constants;

import jpt.Vulkan_SwapChain;
import jpt.Vulkan_DescriptorSet;
import jpt.Vulkan_CommandPool;
import jpt.Vulkan_SyncObjects;
import jpt.Vulkan_UniformBuffer;

import jpt.StaticArray;
import jpt.Optional;
import jpt.TypeDefs;

export namespace jpt
{
    class Window;

    export namespace Vulkan
    {
        /** Per-Window specific Vulkan resource. Each Window should have its own data */
        class WindowResources
        {
        private:
            Window* m_pOwner = nullptr;

        private:
            VkSurfaceKHR m_surface = VK_NULL_HANDLE;
            VkQueue m_presentQueue = VK_NULL_HANDLE;

            SwapChain m_swapChain;
            CommandPool m_commandPool;

            StaticArray<VkCommandBuffer, kMaxFramesInFlight> m_commandBuffers;
            StaticArray<SyncObjects,     kMaxFramesInFlight> m_syncObjects;
            StaticArray<UniformBuffer,   kMaxFramesInFlight> m_uniformBuffers;
            StaticArray<DescriptorSet,   kMaxFramesInFlight> m_descriptorSets;

            // Multisampling anti-aliasing
            VkImage        m_colorImage;
            VkDeviceMemory m_colorImageMemory;
            VkImageView    m_colorImageView;

            // Depth Buffer
            VkImage        m_depthImage;
            VkDeviceMemory m_depthImageMemory;
            VkImageView    m_depthImageView;

            uint32 m_currentFrame = 0;
            bool m_shouldRecreateSwapChain = false;

        public:
            bool Init(Window* pWindow);
            
            /** CPU. Handles per-frame logic & data that aren't directly related to drawing */
            void Update(TimePrecision deltaSeconds);

            /** GPU. Handles rendering commands for drawing a frame */
            void DrawFrame();

            void Terminate();

        public:
            Window* GetOwner() const;
            bool CanDraw() const;

            void SetShouldRecreateSwapChain();

        private:
            void RecreateSwapChain();

            Optional<uint32> AcquireNextImage();
            void Record(uint32 imageIndex);
            void Submit() const;
            void Present(uint32& imageIndex);

            void UpdateUniformBuffer(TimePrecision deltaSeconds);

            void CreateColorResources();
            void DestroyColorResources();

            void CreateDepthResources();
            void DestroyDepthResources();
        };
    }
}