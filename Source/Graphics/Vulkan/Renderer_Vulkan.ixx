// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Applications/App/Application.h"
#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Logger.h"
#include "Core/Validation/Assert.h"

#include <vulkan/vulkan.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

export module jpt.Renderer_Vulkan;

import jpt.Renderer;
import jpt.Graphics.Constants;

import jpt.Vulkan.Vertex;
import jpt.Vulkan.Utils;

import jpt.Vulkan.Constants;
import jpt.Vulkan.WindowResources;
import jpt.Vulkan.Extensions;
import jpt.Vulkan.ValidationLayers;
import jpt.Vulkan.DebugMessenger;
import jpt.Vulkan.PhysicalDevice;
import jpt.Vulkan.LogicalDevice;
import jpt.Vulkan.CommandPool;
import jpt.Vulkan.RenderPass;
import jpt.Vulkan.DescriptorSetLayout;
import jpt.Vulkan.DescriptorPool;
import jpt.Vulkan.PipelineLayout;
import jpt.Vulkan.GraphicsPipeline;
import jpt.Vulkan.VertexBuffer;
import jpt.Vulkan.IndexBuffer;

import jpt.DynamicArray;
import jpt.TypeDefs;
import jpt.Vector2;
import jpt.Vector3;

import jpt.Window;
import jpt.Framework;
import jpt.Event.Window.Resize;
import jpt.Event.Window.Close;

import jpt.File.IO;
import jpt.File.Path;
import jpt.File.Path.Utils;

using namespace jpt::Vulkan;

export namespace jpt
{
	class Renderer_Vulkan final : public Renderer
	{
		using Super = Renderer;

	private:
		VkInstance m_instance = VK_NULL_HANDLE;
#if !IS_RELEASE
		DebugMessenger m_debugMessenger;
#endif
		PhysicalDevice m_physicalDevice;
		LogicalDevice m_logicalDevice;

		CommandPool m_memoryTransferCommandPool;

		DescriptorSetLayout m_descriptorSetLayout;
		DescriptorPool m_descriptorPool;

		RenderPass m_renderPass;
		PipelineLayout m_pipelineLayout;
		GraphicsPipeline m_graphicsPipeline;

		VertexBuffer m_vertexBuffer;
		IndexBuffer m_indexBuffer;

		// Texture
		VkImage m_textureImage;
		VkDeviceMemory m_textureImageMemory;
		VkImageView m_textureImageView;
		VkSampler m_textureSampler;

		DynamicArray<WindowResources> m_windowResources;

	public:
		virtual bool Init() override;
		virtual void Update(TimePrecision deltaSeconds) override;
		virtual void Shutdown() override;

		virtual void DrawFrame() override;

		virtual void RegisterWindow(Window* pWindow) override;
		virtual void OnWindowClose(const Event_Window_Close& eventWindowClose) override;

	private:
		bool CreateInstance();

		bool CreateTextureImage();
		bool CreateTextureImageImageView();
		bool CreateTextureSampler();
	};

	bool Renderer_Vulkan::Init()
	{
		JPT_ENSURE(Super::Init());

		bool success = true;

		//GenerateSierpinski(5, { 0.5f, 0.5f }, { -0.5f, 0.5f }, { 0.0f, -0.5f });

		success &= CreateInstance();
#if !IS_RELEASE
		success &= m_debugMessenger.Init(m_instance);
#endif
		success &= m_physicalDevice.Init(m_instance);
		success &= m_logicalDevice.Init(m_physicalDevice);

		success &= m_memoryTransferCommandPool.Init(m_logicalDevice, m_physicalDevice.GetGraphicsFamilyIndex());

		success &= m_renderPass.Init(m_physicalDevice, m_logicalDevice, kFormat);

		success &= m_descriptorSetLayout.Init(m_logicalDevice);
		success &= m_pipelineLayout.Init(m_logicalDevice, m_descriptorSetLayout);
		success &= m_graphicsPipeline.Init(m_logicalDevice, m_pipelineLayout, m_renderPass);

		success &= m_vertexBuffer.Init(m_physicalDevice, m_logicalDevice, m_memoryTransferCommandPool);
		success &= m_indexBuffer.Init(m_physicalDevice, m_logicalDevice, m_memoryTransferCommandPool);

		success &= m_descriptorPool.Init(m_logicalDevice);

		// Texture
		success &= CreateTextureImage();
		success &= CreateTextureImageImageView();
		success &= CreateTextureSampler();

		// Main window
		Window* pMainWindow = GetApplication()->GetMainWindow();
		RegisterWindow(pMainWindow);

		if (!success)
		{
			JPT_ERROR("Failed to initialize Vulkan renderer");
		}

		return success;
	}

	void Renderer_Vulkan::Update(TimePrecision deltaSeconds)
	{
		Super::Update(deltaSeconds);

		for (WindowResources& resources : m_windowResources)
		{
			if (resources.ShouldRecreateSwapChain())
			{
				resources.RecreateSwapChain(m_physicalDevice, m_logicalDevice, m_renderPass);
			}
		}
	}

	void Renderer_Vulkan::Shutdown()
	{
		m_logicalDevice.WaitIdle();

		for (WindowResources& resources : m_windowResources)
		{
			resources.Shutdown(m_instance, m_logicalDevice);
		}

		// Texture
		vkDestroySampler(m_logicalDevice.GetHandle(), m_textureSampler, nullptr);
		vkDestroyImageView(m_logicalDevice.GetHandle(), m_textureImageView, nullptr);
		vkDestroyImage(m_logicalDevice.GetHandle(), m_textureImage, nullptr);
		vkFreeMemory(m_logicalDevice.GetHandle(), m_textureImageMemory, nullptr);

		m_descriptorPool.Shutdown(m_logicalDevice);
		m_descriptorSetLayout.Shutdown(m_logicalDevice);

		m_vertexBuffer.Shutdown(m_logicalDevice);
		m_indexBuffer.Shutdown(m_logicalDevice);

		m_graphicsPipeline.Shutdown(m_logicalDevice);
		m_pipelineLayout.Shutdown(m_logicalDevice);
		m_renderPass.Shutdown(m_logicalDevice);

		m_memoryTransferCommandPool.Shutdown(m_logicalDevice);

		m_logicalDevice.Shutdown();

#if !IS_RELEASE
		m_debugMessenger.Shutdown(m_instance);
#endif

		vkDestroyInstance(m_instance, nullptr);

		Super::Shutdown();
	}

	void Renderer_Vulkan::DrawFrame()
	{
		Super::DrawFrame();

		for (WindowResources& resources : m_windowResources)
		{
			if (resources.CanDraw())
			{
				resources.DrawFrame(m_logicalDevice, m_renderPass, m_pipelineLayout, m_graphicsPipeline, 
					m_vertexBuffer, m_indexBuffer);
			}
		}
	}

	void Renderer_Vulkan::RegisterWindow(Window* pWindow)
	{
		WindowResources& resources = m_windowResources.EmplaceBack();

		resources.Init(pWindow, m_instance, 
			m_physicalDevice, m_logicalDevice, m_renderPass,
			m_descriptorSetLayout, m_descriptorPool,
			m_textureImageView, m_textureSampler);

		JPT_INFO("Window registered with Vulkan renderer: %lu", pWindow);
	}

	void Renderer_Vulkan::OnWindowClose(const Event_Window_Close& eventWindowClose)
	{
		const Window* pWindow = eventWindowClose.GetWindow();

		for (auto itr = m_windowResources.begin(); itr != m_windowResources.end(); ++itr)
		{
			if (itr->GetOwner() == pWindow)
			{
				itr->Shutdown(m_instance, m_logicalDevice);
				m_windowResources.Erase(itr);
				break;
			}
		}

		JPT_INFO("Window unregistered with Vulkan renderer: %lu", pWindow);
	}

	bool Renderer_Vulkan::CreateInstance()
	{
#if !IS_RELEASE
		if (!CheckValidationLayerSupport())
		{
			JPT_ERROR("Validation layers requested, but not available!");
			return false;
		}
#endif

		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = GetApplication()->GetName();
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "Jupiter Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		DynamicArray<const char*> extensions = GetRequiredExtensions();
		VkInstanceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;
		createInfo.enabledExtensionCount = static_cast<uint32>(extensions.Count());
		createInfo.ppEnabledExtensionNames = extensions.ConstBuffer();

#if !IS_RELEASE
		createInfo.enabledLayerCount = static_cast<uint32>(g_validationLayers.Count());
		createInfo.ppEnabledLayerNames = g_validationLayers.ConstBuffer();

		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = m_debugMessenger.MakeCreateInfo();
		createInfo.pNext = &debugCreateInfo;
#else
		createInfo.enabledLayerCount = 0;
		createInfo.pNext = nullptr;
#endif

		if (const VkResult result = vkCreateInstance(&createInfo, nullptr, &m_instance); result != VK_SUCCESS)
		{
			JPT_ERROR("Failed to create Vulkan instance: %d", result);
			return false;
		}

		return true;
	}

	bool Renderer_Vulkan::CreateTextureImage()
	{
		int32 texWidth, texHeight, texChannels;
		const File::Path texturePath = File::FixDependencies("Assets/Jupiter_Common/Textures/T_Default.jpg");
		unsigned char* pixels = stbi_load(texturePath.ToString().ConstBuffer(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
		JPT_ASSERT(pixels, "Failed to load texture image");

		const VkDeviceSize imageSize = texWidth * texHeight * 4;
		VkBufferCreateInfo bufferInfo = {};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = imageSize;
		bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VkMemoryPropertyFlags properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

		Buffer stagingBuffer;
		stagingBuffer.Create(bufferInfo, properties, m_logicalDevice, m_physicalDevice);
		stagingBuffer.MapMemory(m_logicalDevice, pixels, imageSize);

		stbi_image_free(pixels);

		CreateImage(m_logicalDevice, m_physicalDevice,
			texWidth, texHeight, 
			VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, 
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
			m_textureImage, m_textureImageMemory);

		TransitionImageLayout(m_logicalDevice, m_memoryTransferCommandPool,
			m_textureImage, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
		{
			CopyBufferToImage(m_logicalDevice, m_memoryTransferCommandPool,
				stagingBuffer.GetHandle(), m_textureImage, static_cast<uint32>(texWidth), static_cast<uint32>(texHeight));
		}
		TransitionImageLayout(m_logicalDevice, m_memoryTransferCommandPool,
			m_textureImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

		stagingBuffer.Shutdown(m_logicalDevice);

		return true;
	}

	bool Renderer_Vulkan::CreateTextureImageImageView()
	{
		m_textureImageView = CreateImageView(m_logicalDevice, m_textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);

		return true;
	}

	bool Renderer_Vulkan::CreateTextureSampler()
	{
		VkPhysicalDeviceProperties properties = m_physicalDevice.GetProperties();

		VkSamplerCreateInfo samplerInfo = {};
		samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerInfo.magFilter = VK_FILTER_LINEAR;
		samplerInfo.minFilter = VK_FILTER_LINEAR;
		samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.anisotropyEnable = VK_TRUE;
		samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
		samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		samplerInfo.unnormalizedCoordinates = VK_FALSE;
		samplerInfo.compareEnable = VK_FALSE;
		samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
		samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		samplerInfo.mipLodBias = 0.0f;
		samplerInfo.minLod = 0.0f;
		samplerInfo.maxLod = 0.0f;

		if (vkCreateSampler(m_logicalDevice.GetHandle(), &samplerInfo, nullptr, &m_textureSampler) != VK_SUCCESS)
		{
			JPT_ERROR("Failed to create texture sampler");
			return false;
		}

		return true;
	}
}