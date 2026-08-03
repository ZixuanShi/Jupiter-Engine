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
        float64 m_deltaSeconds = 0.0;

        // Sum of the deltas, so it does not advance while frames are not being driven.
        float64 m_elapsedSeconds = 0.0;

        // FPS
        float64 m_fpsTimer = 0.0;
        uint32 m_frameCount = 0;
        uint32 m_fps = 0;

    public:
        void Update();

        [[nodiscard]] float64 GetDeltaSeconds() const noexcept { return m_deltaSeconds; }
        [[nodiscard]] float64 GetElapsedSeconds() const noexcept { return m_elapsedSeconds; }
        [[nodiscard]] uint32 GetFPS() const noexcept { return m_fps; }
    };
}