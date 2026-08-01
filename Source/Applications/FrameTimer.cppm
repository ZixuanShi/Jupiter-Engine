// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <chrono>

export module jpt.FrameTimer;

import jpt.TypeDefs;

export namespace jpt
{
    class FrameTimer
    {
    private:
        // Delta Seconds
        std::chrono::high_resolution_clock::time_point m_lastTime = std::chrono::high_resolution_clock::now();
        std::chrono::high_resolution_clock::time_point m_frameStartTime = std::chrono::high_resolution_clock::now();
        float64 m_deltaSeconds = 0.0;

        // FPS
        uint32 m_frameCount = 0;
        uint32 m_fps = 0;
        float64 m_fpsTimer = 0.0;

    public:
        void BeginFrame();
        void EndFrame();

        float64 GetDeltaSeconds() const { return m_deltaSeconds; }
        uint32 GetFPS() const { return m_fps; }
    };
}