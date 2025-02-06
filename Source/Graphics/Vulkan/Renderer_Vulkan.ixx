// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <glm/glm.hpp>

#include <vulkan/vulkan.h>

export module jpt.Renderer_Vulkan;

import jpt.Application;

import jpt.Renderer;
import jpt.Graphics.Constants;

import jpt.Vertex;
import jpt.Vulkan.Data;
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
import jpt.Vulkan.Texture;
import jpt.Vulkan.Texture.Sampler;

import jpt.DynamicArray;
import jpt.Matrix44;

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

		DynamicArray<WindowResources> m_windowResources;

	public:
		virtual bool Init() override;
		virtual void Update(TimePrecision deltaSeconds) override;
		virtual void Shutdown() override;

		virtual void DrawFrame() override;

		virtual void RegisterWindow(Window* pWindow) override;
		virtual void OnWindowClose(const Event_Window_Close& eventWindowClose) override;

	public:
		VkInstance GetVkInstance()                    { return m_instance;                  }
		PhysicalDevice& GetPhysicalDevice()           { return m_physicalDevice;            }
		LogicalDevice& GetLogicalDevice()             { return m_logicalDevice;             }
		CommandPool& GetMemoryTransferCommandPool()   { return m_memoryTransferCommandPool; }
		RenderPass& GetRenderPass()                   { return m_renderPass;                }
		DescriptorSetLayout& GetDescriptorSetLayout() { return m_descriptorSetLayout;       }
		DescriptorPool& GetDescriptorPool()           { return m_descriptorPool;            }
		PipelineLayout& GetPipelineLayout()           { return m_pipelineLayout;            }
		GraphicsPipeline& GetGraphicsPipeline()       { return m_graphicsPipeline;          }
		VertexBuffer& GetVertexBuffer()               { return m_vertexBuffer;              }
		IndexBuffer& GetIndexBuffer()                 { return m_indexBuffer;               }

		const VkInstance GetVkInstance()                    const { return m_instance;                  }
		const PhysicalDevice& GetPhysicalDevice()           const { return m_physicalDevice;            }
		const LogicalDevice& GetLogicalDevice()             const { return m_logicalDevice;             }
		const CommandPool& GetMemoryTransferCommandPool()   const { return m_memoryTransferCommandPool; }
		const RenderPass& GetRenderPass()                   const { return m_renderPass;                }
		const DescriptorSetLayout& GetDescriptorSetLayout() const { return m_descriptorSetLayout;       }
		const DescriptorPool& GetDescriptorPool()           const { return m_descriptorPool;            }
		const PipelineLayout& GetPipelineLayout()           const { return m_pipelineLayout;            }
		const GraphicsPipeline& GetGraphicsPipeline()       const { return m_graphicsPipeline;          }
		const VertexBuffer& GetVertexBuffer()               const { return m_vertexBuffer;              }
		const IndexBuffer& GetIndexBuffer()                 const { return m_indexBuffer;               }

	private:
		bool CreateInstance();
	};

	Renderer_Vulkan* GetVkRenderer();
}