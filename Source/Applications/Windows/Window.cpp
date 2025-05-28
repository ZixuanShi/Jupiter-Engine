// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Validation/Assert.h"
#include "Debugging/Logger.h"

module jpt.Window;

import jpt.Event.Manager;
import jpt.Event.Window.Resize;

namespace jpt
{
    bool Window::Init(const char* title, int32 width, int32 height)
    {
        m_title = title;
        m_frameSize = Vec2i(width, height);

        return true;
    }

    void Window::Update(TimePrecision deltaSeconds)
    {
        CalcFPS(deltaSeconds);
    }

    void Window::Shutdown()
    {
        m_title = nullptr;
        m_frameSize = Vec2i(0, 0);
        
        // FPS
        m_fps = 0;
        m_frameCount = 0;
        m_accumulator = 0.0f;
    }

    bool Window::CreateSurface(const DynamicArray<Any>&)
    {
        JPT_ASSERT(false); 
        return true;
    }

    bool Window::ShouldClose() const
    {
        JPT_ASSERT(false); 
        return false;
    }

    void Window::SetMousePosition(Vec2i)
    {
        JPT_ASSERT(false);
    }

    void Window::SetCursorVisible(bool)
    { 
        JPT_ASSERT(false); 
    }

    void Window::Resize(Vec2i size)
    {
        SetFrameSize(size);

        // Inherited class handles the rest
    }

    Vec2i Window::GetFrameSize() const
    {
        return m_frameSize;
    }

    void Window::SetFrameSize(Vec2i size)
    {
        m_frameSize = size;
    }

    float Window::GetAspectRatio() const
    {
        return static_cast<float>(m_frameSize.x) / static_cast<float>(m_frameSize.y);
    }

    bool Window::IsMinimized() const
    {
        return m_frameSize.x == 0 || m_frameSize.y == 0;
    }

    void Window::CalcFPS(TimePrecision deltaSeconds)
    {
        ++m_frameCount;
        m_accumulator += deltaSeconds;

        if (m_accumulator >= 1.0f)
        {
            JPT_LOG("Window \"%s\" FPS: %u", m_title, m_fps);

            m_fps = m_frameCount;
            m_frameCount = 0;
            m_accumulator = 0.0f;
        }
    }
}