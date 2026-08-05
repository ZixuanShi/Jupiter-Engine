// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <chrono>

module jpt.FrameTimer;

namespace jpt
{
    void FrameTimer::Update()
    {
        const std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();

        m_deltaSeconds = std::chrono::duration<float64>(now - m_lastTime).count();
        m_lastTime = now;
        m_elapsedSeconds += m_deltaSeconds;

        ++m_frameCount;
        m_fpsTimer += m_deltaSeconds;
        if (m_fpsTimer >= 1.0)
        {
            m_fps = m_frameCount;
            m_frameCount = 0;
            m_fpsTimer = 0.0;
        }
    }

    void FrameTimer::EndFrame()
    {
        // m_lastTime is this frame's start, stamped by Update() above.
        m_cpuSeconds = std::chrono::duration<float64>(std::chrono::high_resolution_clock::now() - m_lastTime).count();
    }
}