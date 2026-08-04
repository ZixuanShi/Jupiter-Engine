// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module jpt.GestureRecognizer;

import jpt.Constants;
import jpt.Math;
import jpt.TypeDefs;
import std;

namespace jpt
{
    void GestureRecognizer::PostTouch(TouchPhase phase, uint64 id, const Vec2& position, float64 timeSeconds)
    {
        const auto it = std::ranges::find(m_touches, id, &TouchPoint::id);

        switch (phase)
        {
        case TouchPhase::Began:
            if (it == m_touches.end())
            {
                m_touches.emplace_back(id, position, timeSeconds);
            }
            break;

        case TouchPhase::Moved:
            if (it != m_touches.end())
            {
                it->position = position;
            }
            break;

        // Cancelled is not an error path to ignore: a touch interrupted by a call never gets
        // Ended, and a finger left in the table holds the gesture open forever.
        case TouchPhase::Ended:
        case TouchPhase::Cancelled:
            if (it != m_touches.end())
            {
                m_touches.erase(it);
            }
            break;
        }
    }

    void GestureRecognizer::Update()
    {
        const uint32 fingerCount = static_cast<uint32>(m_touches.size());
        if (fingerCount == 0)
        {
            m_lastFingerCount = 0;
            return;
        }

        const Vec2 centroid = Centroid();
        const float32 spread = Spread(centroid);

        // A finger landing or lifting jumps both quantities. Rebase and emit nothing, or the
        // object teleports the moment a second finger touches down.
        if (fingerCount != m_lastFingerCount)
        {
            m_lastFingerCount = fingerCount;
            m_lastCentroid = centroid;
            m_lastSpread = spread;
            return;
        }

        const Vec2 delta = centroid - m_lastCentroid;
        if (delta != Vec2::Zero())
        {
            m_onPan.Dispatch(PanEvent{ .fingerCount = fingerCount, .position = centroid, .delta = delta });
        }

        if (fingerCount == 2 && m_lastSpread > kEpsilon<float32> && spread > kEpsilon<float32>)
        {
            const float32 scale = spread / m_lastSpread;
            if (!AreValuesClose(scale, 1.0f))
            {
                m_onPinch.Dispatch(PinchEvent{ .center = centroid, .scale = scale });
            }
        }

        m_lastCentroid = centroid;
        m_lastSpread = spread;
    }

    Vec2 GestureRecognizer::Centroid() const noexcept
    {
        Vec2 sum = Vec2::Zero();
        for (const TouchPoint& touch : m_touches)
        {
            sum += touch.position;
        }
        return sum / static_cast<float32>(m_touches.size());
    }

    float32 GestureRecognizer::Spread(const Vec2& centroid) const noexcept
    {
        float32 sum = 0.0f;
        for (const TouchPoint& touch : m_touches)
        {
            sum += (touch.position - centroid).Length();
        }
        return sum / static_cast<float32>(m_touches.size());
    }
}
