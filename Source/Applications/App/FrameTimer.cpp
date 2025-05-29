// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

module jpt.FrameTimer;

import jpt.Thread.Utils;
import jpt.Renderer;
import jpt.Graphics.Settings;
import jpt.ProjectSettings;

namespace jpt
{
    TimePrecision FrameTimer::CalcDeltaSeconds()
    {
        m_frameStartTime = StopWatch::Now();
        const TimePrecision deltaSeconds = StopWatch::GetSecondsBetween(m_lastTime, m_frameStartTime);
        return deltaSeconds;
    }

    void FrameTimer::EndFrame()
    {
        // Cap FPS if necessary
        const GraphicsSettings& graphicsSettings = GetGraphicsSettings();
        if (graphicsSettings.ShouldCapFPS() && !graphicsSettings.GetVSyncOn())
        {
            const StopWatch::Point frameEndTime = StopWatch::Now();
            const TimePrecision actualFrameTime = StopWatch::GetSecondsBetween(m_frameStartTime, frameEndTime);
            const TimePrecision targetFrameTime = 1.0f / graphicsSettings.GetTargetFPS();

            if (actualFrameTime < targetFrameTime)
            {
                const TimePrecision sleepTime = targetFrameTime - actualFrameTime;
                SleepMs(static_cast<int32>(sleepTime * 1000.0f));
            }
        }

        m_lastTime = m_frameStartTime;
    }
}
