// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Assert.h"
#include "Debugging/Logger.h"
#include "Applications/App/Application.h"

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

export module jpt.Framework_GLFW;

import jpt.Framework;

import jpt.Window_GLFW;

import jpt.String;
import jpt.ToString;

import jpt.TypeDefs;
import jpt.Time.TypeDefs;

namespace jpt
{
	namespace Callbacks
	{
		void OnError(int32 error, const char* description);
	}

	export class Framework_GLFW final : public Framework
	{
		using Super = Framework;

	public:
		virtual bool Init() override;
		virtual void Update(TimePrecision deltaSeconds) override;
		virtual void Shutdown() override;

		virtual const char** GetRequiredExtensions(uint32& extensionCount) override;

		// Vulkan
		static VkResult CreateWindowSurface(VkInstance instance, VkSurfaceKHR* pSurface);
	};

	bool Framework_GLFW::Init()
	{
		JPT_ENSURE(Super::Init());

		JPT_ENSURE(glfwInit());
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		glfwSetErrorCallback(Callbacks::OnError);

		return true;
	}

	void Framework_GLFW::Update(TimePrecision deltaSeconds)
	{
		Super::Update(deltaSeconds);

		glfwPollEvents();
	}

	void Framework_GLFW::Shutdown()
	{
		Super::Shutdown();

		glfwTerminate();
	}

	const char** Framework_GLFW::GetRequiredExtensions(uint32& extensionCount)
	{
		return glfwGetRequiredInstanceExtensions(&extensionCount);
	}

	VkResult Framework_GLFW::CreateWindowSurface(VkInstance instance, VkSurfaceKHR* pSurface)
	{
		Application* pApp = GetApplication();
		Window* pMainWindow = pApp->GetMainWindow();
		Window_GLFW* pGLFWWindow = static_cast<Window_GLFW*>(pMainWindow);
		JPT_ASSERT(pGLFWWindow);

		return glfwCreateWindowSurface(instance, pGLFWWindow->GetGLFWWindow(), nullptr, pSurface);
	}

	namespace Callbacks
	{
		void OnError(int32 error, const char* description)
		{
			JPT_ERROR("GLFW Error: " + ToString(error) + " - " + description);
		}
	}
}