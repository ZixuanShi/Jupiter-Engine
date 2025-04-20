// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Core/Validation/Assert.h"
#include "Debugging/Logger.h"

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

module jpt.Window_GLFW;

import jpt.Application;

import jpt.String;
import jpt.Utilities;
import jpt.Any;
import jpt.DynamicArray;

import jpt.Input.Manager;
import jpt.Input.Enums;
import jpt.RawInput;
import jpt.RawInput_GLFW;

import jpt.Time.TypeDefs;

import jpt.Graphics.Enums;

import jpt.Event.Manager;
import jpt.Event.Window.Resize;
import jpt.Event.Window.Close;
import jpt.Event.MouseButton;
import jpt.Event.MouseScroll;
import jpt.Event.MouseMove;
import jpt.Event.Key;

namespace jpt
{
    namespace Callbacks
    {
        void OnWindowClose(GLFWwindow* pGLFWWindow);
        void OnWindowResize(GLFWwindow* pGLFWWindow, int32 width, int32 height);

        void OnMouseButton(GLFWwindow* pGLFWWindow, int32 button, int32 action, int32 mods);
        void OnMouseMove(GLFWwindow* pGLFWWindow, double x, double y);
        void OnMouseScroll(GLFWwindow* pGLFWWindow, double xOffset, double yOffset);
        void OnKey(GLFWwindow* pGLFWWindow, int32 key, int32 scancode, int32 action, int32 mods);
    }

    bool Window_GLFW::Init(const char* title, int32 width, int32 height)
    {
        Super::Init(title, width, height);

        // Create GLFW window
        m_pGLFWWindow = glfwCreateWindow(width, height, title, nullptr, nullptr);
        if (!m_pGLFWWindow)
        {
            JPT_ERROR("Failed to create GLFW window");
            EventManager::GetInstance().Send(Event_Window_Close{ this });
            return false;
        }

        // Set current window callbacks
        glfwSetWindowUserPointer(m_pGLFWWindow, this);

        // Window
        glfwSetWindowCloseCallback(m_pGLFWWindow, Callbacks::OnWindowClose);
        glfwSetFramebufferSizeCallback(m_pGLFWWindow, Callbacks::OnWindowResize);

        // Input
        glfwSetMouseButtonCallback(m_pGLFWWindow, Callbacks::OnMouseButton);
        glfwSetCursorPosCallback(m_pGLFWWindow, Callbacks::OnMouseMove);
        glfwSetScrollCallback(m_pGLFWWindow, Callbacks::OnMouseScroll);
        glfwSetKeyCallback(m_pGLFWWindow, Callbacks::OnKey);

        return true;
    }

    void Window_GLFW::Shutdown()
    {
        glfwDestroyWindow(m_pGLFWWindow);
        m_pGLFWWindow = nullptr;

        Super::Shutdown();
    }

    bool Window_GLFW::CreateSurface(const DynamicArray<Any>& context)
    {
        const Graphics_API api = GetApplication()->GetGraphicsAPI();

        switch (api.Value())
        {
        case Graphics_API::Vulkan:
        {
            VkInstance instance = context[0].As<VkInstance>();
            VkSurfaceKHR* pSurface = context[1].As<VkSurfaceKHR*>();
            return glfwCreateWindowSurface(instance, m_pGLFWWindow, nullptr, pSurface) == VK_SUCCESS;
        }
        default:
            JPT_ASSERT(false, "Un-implemented Graphics API: %s", ToString(api).ConstBuffer());
            return false;
        };
    }

    void Window_GLFW::SetMousePosition(Vec2i position)
    {
        glfwSetCursorPos(m_pGLFWWindow, position.x, position.y);
    }

    void Window_GLFW::SetCursorVisible(bool isVisible)
    {
        glfwSetInputMode(m_pGLFWWindow, GLFW_CURSOR, isVisible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
    }

    bool Window_GLFW::ShouldClose() const
    {
        return glfwWindowShouldClose(m_pGLFWWindow);
    }

    namespace Callbacks
    {
        void OnWindowClose(GLFWwindow* pGLFWWindow)
        {
            Window* pWindow = static_cast<Window*>(glfwGetWindowUserPointer(pGLFWWindow));
            JPT_ASSERT(pWindow, "Couldn't cast window user pointer to jpt::Window");

            Event_Window_Close eventWindowClose = { pWindow };
            EventManager::GetInstance().Send(eventWindowClose);
        }

        void OnWindowResize(GLFWwindow* pGLFWWindow, int32 width, int32 height)
        {
            Window* pWindow = static_cast<Window*>(glfwGetWindowUserPointer(pGLFWWindow));
            JPT_ASSERT(pWindow, "Couldn't cast window user pointer to jpt::Window");

            pWindow->ResizeFrame(Vec2i(width, height));

            Event_Window_Resize eventWindowResize = { pWindow, width, height };
            EventManager::GetInstance().Send(eventWindowResize);
        }

        void OnMouseButton(GLFWwindow* pGLFWWindow, int32 button, int32 action, int32 mods)
        {
            const Input::RawInput* pRawInput = InputManager::GetInstance().GetRawInput();
            const Input::RawInput_GLFW* pRawInputGLFW = static_cast<const Input::RawInput_GLFW*>(pRawInput);
            JPT_ASSERT(pRawInputGLFW, "Couldn't cast raw input to jpt::Input::RawInput_GLFW");

            const Input::MouseButton mouseButton = pRawInputGLFW->ToMouseButton(button);
            const Input::Modifier modifiers = pRawInputGLFW->ParseModifiers(mods);
            const Input::KeyState state = pRawInputGLFW->ParseKeyState(action);

            Window* pWindow = static_cast<Window*>(glfwGetWindowUserPointer(pGLFWWindow));

            double x, y;
            glfwGetCursorPos(pGLFWWindow, &x, &y);

            const Event_Mouse_Button eventMouseButtonPress = { pWindow,
                                                               x,
                                                               y,
                                                               mouseButton,
                                                               state,
                                                               modifiers };

            EventManager::GetInstance().Send(eventMouseButtonPress);
        }

        void OnMouseMove(GLFWwindow* pGLFWWindow, double x, double y)
        {
            Window* pWindow = static_cast<Window*>(glfwGetWindowUserPointer(pGLFWWindow));
            const Event_Mouse_Move eventMouseMove = { pWindow, x, y };

            EventManager::GetInstance().Send(eventMouseMove);
        }

        void OnMouseScroll(GLFWwindow* pGLFWWindow, double xOffset, double yOffset)
        {
            Window* pWindow = static_cast<Window*>(glfwGetWindowUserPointer(pGLFWWindow));
            const Event_Mouse_Scroll eventMouseScroll = { pWindow, xOffset, yOffset };

            EventManager::GetInstance().Send(eventMouseScroll);
        }

        void OnKey(GLFWwindow* pGLFWWindow, int32 key, [[maybe_unused]] int32 scancode, int32 action, int32 mods)
        {
            const Input::RawInput* pRawInput = InputManager::GetInstance().GetRawInput();
            const Input::RawInput_GLFW* pRawInputGLFW = static_cast<const Input::RawInput_GLFW*>(pRawInput);
            JPT_ASSERT(pRawInputGLFW, "Couldn't cast raw input to jpt::Input::RawInput_GLFW");

            const Input::Key keyCode = pRawInputGLFW->ToKey(key);
            const Input::Modifier modifiers = pRawInputGLFW->ParseModifiers(mods);
            const Input::KeyState keyState = pRawInputGLFW->ParseKeyState(action);

            Window* pWindow = static_cast<Window*>(glfwGetWindowUserPointer(pGLFWWindow));
            const Event_Key eventKeyboardKeyPress = { pWindow, keyCode, keyState, modifiers };

            EventManager::GetInstance().Send(eventKeyboardKeyPress);
        }
    }
}