// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Core/Validation/Assert.h"
#include "Debugging/Logger.h"

#include <GLFW/glfw3.h>

export module jpt.Framework_GLFW;

import jpt.Application;
import jpt.Framework;

import jpt.Window;
import jpt.Window_GLFW;
import jpt.Window.Manager;

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
		virtual bool PreInit() override;
		virtual bool Init() override;
		virtual void Update(TimePrecision deltaSeconds) override;
		virtual void Shutdown() override;

		virtual const char** GetRequiredExtensions(uint32& extensionCount) override;
	};

	bool Framework_GLFW::PreInit()
	{
		JPT_ENSURE(Super::PreInit());

		JPT_ENSURE(glfwInit());
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		glfwSetErrorCallback(Callbacks::OnError);

		return true;
	}

	bool Framework_GLFW::Init()
	{
		JPT_ENSURE(Super::Init());

		return true;
	}

	void Framework_GLFW::Update(TimePrecision deltaSeconds)
	{
		Super::Update(deltaSeconds);

		glfwPollEvents();
	}

	void Framework_GLFW::Shutdown()
	{
		glfwTerminate();

		Super::Shutdown();
	}

	const char** Framework_GLFW::GetRequiredExtensions(uint32& extensionCount)
	{
		return glfwGetRequiredInstanceExtensions(&extensionCount);
	}

	namespace Callbacks
	{
		void OnError(int32 error, const char* description)
		{
			JPT_ERROR("GLFW Error: " + ToString(error) + " - " + description);
		}
	}
}