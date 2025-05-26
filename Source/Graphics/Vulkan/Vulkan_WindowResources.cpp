// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Core/Validation/Assert.h"
#include "Debugging/Logger.h"

#include <vulkan/vulkan.h>

module jpt.Vulkan.WindowResources;

import jpt.Application;
import jpt.Window;

import jpt.Renderer_Vulkan;
import jpt.Camera;
import jpt.Particle;

import jpt.Vulkan.Utils;
import jpt.Vulkan.Constants;
import jpt.Vulkan.PhysicalDevice;
import jpt.Vulkan.LogicalDevice;
import jpt.Vulkan.SwapChain.SupportDetails;
import jpt.Vulkan.RenderPass;
import jpt.Vulkan.PipelineLayout;
import jpt.Vulkan.GraphicsPipeline;
import jpt.Vulkan.VertexBuffer;
import jpt.Vulkan.IndexBuffer;

import jpt.Constants;
import jpt.Matrix44;
import jpt.Math;
import jpt.Utilities;

namespace jpt::Vulkan
{
    bool WindowResources::Init(Window* pWindow)
    {
        const Renderer_Vulkan* pVulkanRenderer = GetVkRenderer();
        const VkInstance instance = pVulkanRenderer->GetVkInstance();
        const PhysicalDevice& physicalDevice = pVulkanRenderer->GetPhysicalDevice();
        const LogicalDevice& logicalDevice = pVulkanRenderer->GetLogicalDevice();

        // Surface
        m_pOwner = pWindow;
        pWindow->CreateSurface({ instance, &m_surface });

        // Present queue
        const uint32 presentFamilyIndex = physicalDevice.FindPresentFamilyIndex(m_surface);
        vkGetDeviceQueue(logicalDevice.GetHandle(), presentFamilyIndex, 0, &m_presentQueue);

        // SwapChain
        m_swapChain.Init(m_pOwner, m_surface);
        m_swapChain.CreateImageViews();

        CreateColorResources();
        CreateDepthResources();
        m_swapChain.CreateFramebuffers(m_colorImageView, m_depthImageView);

        // Command pool & buffers
        m_commandPool.Init();

        VkCommandBufferAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = m_commandPool.GetHandle();
        allocInfo.commandBufferCount = static_cast<uint32>(m_commandBuffers.Count());
        if (const VkResult result = vkAllocateCommandBuffers(logicalDevice.GetHandle(), &allocInfo, m_commandBuffers.Buffer()); result != VK_SUCCESS)
        {
            JPT_ERROR("Failed to allocate command buffers: %d", result);
            return false;
        }

        // Sync objects
        for (SyncObjects& syncObjects : m_syncObjects)
        {
            if (!syncObjects.Init())
            {
                return false;
            }
        }

        // Uniform Buffers
        for (UniformBuffer& uniformBuffer : m_uniformBuffers)
        {
            if (!uniformBuffer.Init())
            {
                return false;
            }
        }

        // Descriptor sets
        for (size_t i = 0; i < kMaxFramesInFlight; ++i)
        {
            if (!m_descriptorSets[i].Init(m_uniformBuffers[i]))
            {
                return false;
            }
        }

        return true;
    }

    void WindowResources::Update(TimePrecision deltaSeconds)
    {
        if (m_shouldRecreateSwapChain)
        {
            RecreateSwapChain();
        }

        UpdateUniformBuffer(deltaSeconds);
    }

    void WindowResources::DrawFrame()
    {
        if (!CanDraw())
        {
            return;
        }

        const Renderer_Vulkan* pVulkanRenderer = GetVkRenderer();
        const LogicalDevice& logicalDevice = pVulkanRenderer->GetLogicalDevice();

        SyncObjects& syncObjects = m_syncObjects[m_currentFrame];

        // Wait for the previous frame to finish
        vkWaitForFences(logicalDevice.GetHandle(), 1, syncObjects.GetInFlightFencePtr(), VK_TRUE, UINT64_MAX);

        if (Optional<uint32> imageIndex = AcquireNextImage())
        {
            vkResetFences(logicalDevice.GetHandle(), 1, syncObjects.GetInFlightFencePtr());

            Record(imageIndex.Value());
            Submit();
            Present(imageIndex.Value());

            m_currentFrame += 1;
            m_currentFrame %= kMaxFramesInFlight;
        }
    }

    void WindowResources::Shutdown()
    {
        const Renderer_Vulkan* pVulkanRenderer = GetVkRenderer();
        const VkInstance instance = pVulkanRenderer->GetVkInstance();
        const LogicalDevice& logicalDevice = pVulkanRenderer->GetLogicalDevice();

        logicalDevice.WaitIdle();

        DestroyColorResources();
        DestroyDepthResources();

        for (DescriptorSet& descriptorSet : m_descriptorSets)
        {
            descriptorSet.Shutdown();
        }

        for (uint32 i = 0; i < kMaxFramesInFlight; ++i)
        {
            m_uniformBuffers[i].Shutdown();
        }

        for (SyncObjects& syncObjects : m_syncObjects)
        {
            syncObjects.Shutdown();
        }

        for (VkCommandBuffer commandBuffer : m_commandBuffers)
        {
            vkFreeCommandBuffers(logicalDevice.GetHandle(), m_commandPool.GetHandle(), 1, &commandBuffer);
        }

        m_commandPool.Shutdown();
        m_swapChain.Shutdown();

        vkDestroySurfaceKHR(instance, m_surface, nullptr);
        m_surface = VK_NULL_HANDLE;
    }

    Window* WindowResources::GetOwner() const
    {
        return m_pOwner;
    }

    bool WindowResources::CanDraw() const
    {
        bool canDraw = true;

        canDraw &= !m_pOwner->IsMinimized();

        return canDraw;
    }

    void WindowResources::SetShouldRecreateSwapChain()
    {
        m_shouldRecreateSwapChain = true;
    }

    void WindowResources::RecreateSwapChain()
    {
        LogicalDevice::Get().WaitIdle();

        m_swapChain.Shutdown();
        DestroyColorResources();
        DestroyDepthResources();

        m_swapChain.Init(m_pOwner, m_surface);
        m_swapChain.CreateImageViews();
        CreateColorResources();
        CreateDepthResources();
        m_swapChain.CreateFramebuffers(m_colorImageView, m_depthImageView);

        m_shouldRecreateSwapChain = false;
    }

    Optional<uint32> WindowResources::AcquireNextImage()
    {
        const Renderer_Vulkan* pVulkanRenderer = GetVkRenderer();
        const LogicalDevice& logicalDevice = pVulkanRenderer->GetLogicalDevice();

        uint32 imageIndex = 0;
        const VkResult result = vkAcquireNextImageKHR(logicalDevice.GetHandle(), m_swapChain.GetHandle(), UINT64_MAX, m_syncObjects[m_currentFrame].GetImageAvailableSemaphore(), VK_NULL_HANDLE, &imageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR)
        {
            m_shouldRecreateSwapChain = true;
            return {};
        }
        else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        {
            JPT_ERROR("Failed to acquire swap chain image: %d", result);
            return {};
        }

        return imageIndex;
    }

    void WindowResources::Record(uint32 imageIndex)
    {
        Renderer_Vulkan* pVulkanRenderer = GetVkRenderer();
        const RenderPass& renderPass = pVulkanRenderer->GetRenderPass();
        const PipelineLayout& pipelineLayout = pVulkanRenderer->GetPipelineLayout();
        const GraphicsPipeline& graphicsPipeline = pVulkanRenderer->GetGraphicsPipeline();
        VertexBuffer& vertexBuffer = pVulkanRenderer->GetVertexBuffer();
        IndexBuffer& indexBuffer = pVulkanRenderer->GetIndexBuffer();

        const VkCommandBuffer& commandBuffer = m_commandBuffers[m_currentFrame];
        vkResetCommandBuffer(commandBuffer, 0);

        VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        if (const VkResult result = vkBeginCommandBuffer(commandBuffer, &beginInfo); result != VK_SUCCESS)
        {
            JPT_ERROR("Failed to begin recording command buffer: %d", result);
            return;
        }

        VkRenderPassBeginInfo renderPassInfo = {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = renderPass.GetHandle();
        renderPassInfo.framebuffer = m_swapChain.GetFramebuffers()[imageIndex];
        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = m_swapChain.GetExtent();

        VkClearValue clearValues[2];
        clearValues[0].color = { 0.05f, 0.05f, 0.05f, 1.0f };
        clearValues[1].depthStencil = { 1.0f, 0 };
        renderPassInfo.clearValueCount = JPT_ARRAY_COUNT(clearValues);
        renderPassInfo.pClearValues = clearValues;

        // Start recording commands
        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        {
            vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline.GetHandle());

            VkViewport viewport = {};
            viewport.x = 0.0f;
            viewport.y = 0.0f;
            viewport.width = static_cast<float>(m_swapChain.GetExtent().width);
            viewport.height = static_cast<float>(m_swapChain.GetExtent().height);
            viewport.minDepth = 0.0f;
            viewport.maxDepth = 1.0f;
            vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

            VkRect2D scissor = {};
            scissor.offset = { 0, 0 };
            scissor.extent = m_swapChain.GetExtent();
            vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

            VkBuffer vertexBuffers[] = { vertexBuffer.GetBuffer() };
            VkDeviceSize offsets[] = { 0 };
            vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

            if constexpr (AreSameType<uint16, IndexBuffer::IndexType>)
            {
                vkCmdBindIndexBuffer(commandBuffer, indexBuffer.GetBuffer(), 0, VK_INDEX_TYPE_UINT16);
            }
            else if constexpr (AreSameType<uint32, IndexBuffer::IndexType>)
            {
                vkCmdBindIndexBuffer(commandBuffer, indexBuffer.GetBuffer(), 0, VK_INDEX_TYPE_UINT32);
            }
            else
            {
                JPT_ASSERT(false, "Index buffer type not supported");
            }

            PushConstantData pushConstantData = {};
            pushConstantData.value = 1.0f;
            vkCmdPushConstants(commandBuffer, pipelineLayout.GetHandle(), VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(PushConstantData), &pushConstantData);

            vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout.GetHandle(), 0, 1, m_descriptorSets[m_currentFrame].GetHandlePtr(), 0, nullptr);
            vkCmdDrawIndexed(commandBuffer, static_cast<uint32>(indexBuffer.GetCount()), 1, 0, 0, 0);
        }
        vkCmdEndRenderPass(commandBuffer);

        if (const VkResult result = vkEndCommandBuffer(commandBuffer); result != VK_SUCCESS)
        {
            JPT_ERROR("Failed to record command buffer: %d", result);
        }
    }

    void WindowResources::Submit() const
    {
        const SyncObjects& syncObjects = m_syncObjects[m_currentFrame];
        const VkCommandBuffer& commandBuffer = m_commandBuffers[m_currentFrame];

        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = { syncObjects.GetImageAvailableSemaphore() };
        VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        VkSemaphore signalSemaphores[] = { syncObjects.GetRenderFinishedSemaphore() };
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        if (const VkResult result = vkQueueSubmit(LogicalDevice::GetGraphicsVkQueue(), 1, &submitInfo, syncObjects.GetInFlightFence()); result != VK_SUCCESS)
        {
            JPT_ERROR("Failed to submit draw command buffer: %d", result);
        }
    }

    void WindowResources::Present(uint32& imageIndex)
    {
        VkPresentInfoKHR presentInfo = {};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        VkSemaphore signalSemaphores[] = { m_syncObjects[m_currentFrame].GetRenderFinishedSemaphore() };
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapChains[] = { m_swapChain.GetHandle() };
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = &imageIndex;

        const VkResult result = vkQueuePresentKHR(m_presentQueue, &presentInfo);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
        {
            m_shouldRecreateSwapChain = true;
            return;
        }

        // Need to wait for current frame's presentation before next frame can be drawn
        // Otherwise, will crash "vkQueueSubmit(): pSubmits[0].pSignalSemaphores[0] is being signaled by VkQueue, but it may still be in use by VkSwapchainKHR
        vkQueueWaitIdle(m_presentQueue);
    }

    void WindowResources::UpdateUniformBuffer(TimePrecision /*deltaSeconds*/)
    {
        Uniform_MVP mvp = {};

        mvp.view = GetVkRenderer()->GetCamera().GetMatrix();

        mvp.proj = Matrix44::Perspective(ToRadians(45.0f), m_pOwner->GetAspectRatio(), 0.1f, 100.0f);
        mvp.proj[1][1] *= -1;

        UniformBuffer& uniformBuffer = m_uniformBuffers[m_currentFrame];
        uniformBuffer.MapMemory(&mvp, sizeof(mvp));
    }

    void WindowResources::CreateColorResources()
    {
        const VkSampleCountFlagBits msaaSamples = PhysicalDevice::Get().GetMsaaSamples();
        const Vec2i frameSize = m_pOwner->GetFrameSize();
        const VkFormat colorFormat = m_swapChain.GetImageFormat();

        CreateImage(frameSize.x, frameSize.y, 1,
            msaaSamples, colorFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_colorImage, m_colorImageMemory);

        m_colorImageView = CreateImageView(m_colorImage, colorFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
    }

    void WindowResources::DestroyColorResources()
    {
        vkDestroyImageView(LogicalDevice::GetVkDevice(), m_colorImageView, nullptr);
        vkDestroyImage(LogicalDevice::GetVkDevice(), m_colorImage, nullptr);
        vkFreeMemory(LogicalDevice::GetVkDevice(), m_colorImageMemory, nullptr);
    }

    void WindowResources::CreateDepthResources()
    {
        const VkSampleCountFlagBits msaaSamples = PhysicalDevice::Get().GetMsaaSamples();
        const Vec2i frameSize = m_pOwner->GetFrameSize();
        const VkFormat depthFormat = PhysicalDevice::Get().FindDepthFormat();

        CreateImage(frameSize.x, frameSize.y, 1,
            msaaSamples, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            m_depthImage, m_depthImageMemory);

        m_depthImageView = CreateImageView(m_depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, 1);
    }

    void WindowResources::DestroyDepthResources()
    {
        vkDestroyImageView(LogicalDevice::GetVkDevice(), m_depthImageView, nullptr);
        vkDestroyImage(LogicalDevice::GetVkDevice(), m_depthImage, nullptr);
        vkFreeMemory(LogicalDevice::GetVkDevice(), m_depthImageMemory, nullptr);
    }
}