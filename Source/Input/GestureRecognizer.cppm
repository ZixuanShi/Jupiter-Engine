// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.GestureRecognizer;

import jpt.EventDispatcher;
import jpt.InputCodes;
import jpt.TypeDefs;
import jpt.Vector2;
import std;

export namespace jpt
{
    /** Tracked state, not a dispatched payload -- hence no Event suffix. */
    struct TouchPoint
    {
        uint64 id = 0;                      // UITouch* on iOS, pointerId on Android.
        Vec2 position = Vec2::Zero();
        float64 startTime = 0.0;
    };

    struct PanEvent
    {
        uint32 fingerCount = 0;
        Vec2 position = Vec2::Zero();       // Centroid.
        Vec2 delta = Vec2::Zero();
    };

    struct PinchEvent
    {
        Vec2 center = Vec2::Zero();
        float32 scale = 1.0f;               // This frame over last, so around 1.
    };

    /** Platform-agnostic: it sees only ids, positions, phases and timestamps, which is what UIKit
        and Android's MotionEvent both provide.

        Recognition runs once per frame rather than per event, because touchesMoved: delivers only
        the touches that moved -- recognising there would work from a half-updated centroid. */
    class GestureRecognizer
    {
    private:
        std::vector<TouchPoint> m_touches;

        Vec2 m_lastCentroid = Vec2::Zero();
        float32 m_lastSpread = 0.0f;
        uint32 m_lastFingerCount = 0;

        EventDispatcher<PanEvent> m_onPan;
        EventDispatcher<PinchEvent> m_onPinch;

    public:
        void PostTouch(TouchPhase phase, uint64 id, const Vec2& position, float64 timeSeconds);
        void Update();

    public:
        [[nodiscard]] EventDispatcher<PanEvent>& OnPan() noexcept { return m_onPan; }
        [[nodiscard]] EventDispatcher<PinchEvent>& OnPinch() noexcept { return m_onPinch; }

        [[nodiscard]] usize GetTouchCount() const noexcept { return m_touches.size(); }

    private:
        [[nodiscard]] Vec2 Centroid() const noexcept;
        [[nodiscard]] float32 Spread(const Vec2& centroid) const noexcept;
    };
}
