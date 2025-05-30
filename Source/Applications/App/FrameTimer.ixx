// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.FrameTimer;

import jpt.TypeDefs;
import jpt.StopWatch;

export namespace jpt
{
    class FrameTimer
    {
    private:
        StopWatch::Point m_lastTime = StopWatch::Now();
        StopWatch::Point m_frameStartTime = StopWatch::Now();
        TimePrecision m_deltaSeconds = 0.0f;

    public:
        void BeginFrame();
        void EndFrame();
        TimePrecision GetDeltaSeconds() const { return m_deltaSeconds; }
    };
}