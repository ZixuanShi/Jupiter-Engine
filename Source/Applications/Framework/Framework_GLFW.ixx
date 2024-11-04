// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Assert.h"
#include "Debugging/Logger.h"
#include "Applications/App/Application.h"

#include <GLFW/glfw3.h>

export module jpt.Framework_GLFW;

import jpt.Framework;

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

	namespace Callbacks
	{
		void OnError(int32 error, const char* description)
		{
			JPT_ERROR("GLFW Error: " + ToString(error) + " - " + description);
		}
	}
}