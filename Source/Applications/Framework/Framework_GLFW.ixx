// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Assert.h"
#include "Debugging/Logger.h"

#include <GLFW/glfw3.h>

export module jpt.Framework_GLFW;

import jpt.Framework_Base;
import jpt.TypeDefs;
import jpt.ToString;
import jpt.Utilities;

namespace jpt
{
	export class Framework_GLFW : public Framework_Base
	{
		using Super = Framework_Base;

	public:
		virtual bool Init() override;
		virtual void Terminate() override;
	};

	bool Framework_GLFW::Init()
	{
		JPT_ENSURE(Super::Init());
		JPT_ENSURE(glfwInit());

		glfwSetErrorCallback([](int32 error, const char* description)
			{
				JPT_LOG("GLFW Error: " + ToString(error) + " - " + description);
			});

		return true;
	}

	void Framework_GLFW::Terminate()
	{
		Super::Terminate();

		glfwTerminate();
	}
}