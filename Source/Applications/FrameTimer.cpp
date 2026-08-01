// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <chrono>
#include <string>

module jpt.FrameTimer;

import jpt.Logger;

namespace jpt
{
    void FrameTimer::BeginFrame()
    {
        m_frameStartTime = std::chrono::high_resolution_clock::now();
        m_deltaSeconds = std::chrono::duration<float64>(m_frameStartTime - m_lastTime).count();

        ++m_frameCount;
        m_fpsTimer += m_deltaSeconds;
        if (m_fpsTimer >= 1.0)
        {
            m_fps = m_frameCount;
            m_frameCount = 0;
            m_fpsTimer = 0.0;
            Debug("FPS: {}", m_fps);
        }
    }

    void FrameTimer::EndFrame()
    {
        m_lastTime = m_frameStartTime;
    }
}