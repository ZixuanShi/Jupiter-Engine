// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Utilities.h"
#include "Core/Validation/Assert.h"
#include "Debugging/Logger.h"

#include <GLFW/glfw3.h>

module jpt.Framework_GLFW;

import jpt.Window;
import jpt.Window_GLFW;
import jpt.WindowManager;

import jpt.String;
import jpt.ToString;

namespace jpt
{
    namespace Callbacks
    {
        void OnError(int32 error, const char* description)
        {
            JPT_ERROR("GLFW Error: " + ToString(error) + " - " + description);
        }
    }

    bool Framework_GLFW::PreInit()
    {
        JPT_ENSURE(Super::PreInit());

        JPT_ENSURE(glfwInit());
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        glfwSetErrorCallback(Callbacks::OnError);

        return true;
    }

    void Framework_GLFW::Update(TimePrecision deltaSeconds)
    {
        Super::Update(deltaSeconds);

        glfwPollEvents();
    }

    void Framework_GLFW::Terminate()
    {
        glfwTerminate();

        Super::Terminate();
    }

    const char** Framework_GLFW::GetRequiredExtensions(uint32& extensionCount)
    {
        return glfwGetRequiredInstanceExtensions(&extensionCount);
    }
}
