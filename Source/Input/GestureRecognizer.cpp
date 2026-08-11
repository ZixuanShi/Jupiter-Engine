// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module jpt.GestureRecognizer;

import jpt.Constants;
import jpt.Math;
import jpt.TypeDefs;
import std;

namespace jpt
{
    void GestureRecognizer::PostTouch(TouchPhase phase, uint64 id, const Vec2& position, float64 timeSeconds, TouchDevice device)
    {
        const auto it = std::ranges::find(m_touches, id, &TouchPoint::id);

        switch (phase)
        {
        case TouchPhase::Began:
            if (it == m_touches.end())
            {
                m_touches.emplace_back(id, position, timeSeconds);
                m_device = device;
                ++m_generation;
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
                ++m_generation;
            }
            break;
        }
    }

    void GestureRecognizer::Update()
    {
        const uint32 fingerCount = static_cast<uint32>(m_touches.size());
        if (fingerCount == 0)
        {
            return;
        }

        const Vec2 centroid = Centroid();
        const float32 spread = Spread(centroid);
        const float32 angle = (fingerCount == 2) ? Angle() : 0.0f;

        // A finger landing or lifting jumps all three. Rebase and emit nothing, or the object
        // teleports the moment a second finger touches down.
        if (m_generation != m_lastGeneration)
        {
            m_lastGeneration = m_generation;
            m_lastCentroid = centroid;
            m_lastSpread = spread;
            m_lastAngle = angle;
            return;
        }

        // Rebase and emit nothing, rather than returning before the measurements: a stale
        // baseline would deliver the whole captured excursion as one jump on release.
        if (m_captured)
        {
            m_lastCentroid = centroid;
            m_lastSpread = spread;
            m_lastAngle = angle;
            return;
        }

        // One finger on a trackpad is the cursor, not a gesture. Dragging it *pressed* still
        // works: a trackpad click is a real mouse button. Pinch and twist need two by definition.
        const bool isPointerNotGesture = (m_device != TouchDevice::Direct) && (fingerCount < 2);

        const Vec2 delta = centroid - m_lastCentroid;
        if (!isPointerNotGesture && delta != Vec2::Zero())
        {
            m_onPan.Dispatch(PanEvent{ .fingerCount = fingerCount, .position = centroid, .delta = delta });
        }

        if (fingerCount == 2 && m_lastSpread > kEpsilon<float32> && spread > kEpsilon<float32>)
        {
            // A real dead zone, not an epsilon: two fingers sliding to scroll wobble by a fraction
            // of a millimetre every frame, and at epsilon every one of those is a zoom.
            constexpr float32 kDeadZone = 0.005f;

            const float32 scale = spread / m_lastSpread;
            if (std::abs(scale - 1.0f) > kDeadZone)
            {
                m_onPinch.Dispatch(PinchEvent{ .center = centroid, .scale = scale });
            }

            // remainder wraps to [-pi, pi], which a plain subtraction does not: a twist crossing
            // atan2's branch cut would otherwise read as a full turn in one frame.
            constexpr float32 kTwistDeadZone = ToRadians(0.15f);

            const float32 twist = std::remainder(angle - m_lastAngle, kTwoPi<float32>);
            if (std::abs(twist) > kTwistDeadZone)
            {
                m_onTwist.Dispatch(TwistEvent{ .center = centroid, .radians = twist });
            }
        }

        m_lastCentroid = centroid;
        m_lastSpread = spread;
        m_lastAngle = angle;
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

    float32 GestureRecognizer::Angle() const noexcept
    {
        const Vec2 axis = m_touches[1].position - m_touches[0].position;
        return std::atan2(axis.y, axis.x);
    }
}
