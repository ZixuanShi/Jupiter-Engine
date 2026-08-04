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

    struct TwistEvent
    {
        Vec2 center = Vec2::Zero();
        float32 radians = 0.0f;             // This frame's change, so near 0. Positive is clockwise on screen.
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
        float32 m_lastAngle = 0.0f;

        // Bumped whenever a touch is added or removed. Swapping one finger for another inside a
        // frame holds the count at 2 while jumping everything measured from them.
        uint32 m_generation = 0;
        uint32 m_lastGeneration = 0;

        EventDispatcher<PanEvent> m_onPan;
        EventDispatcher<PinchEvent> m_onPinch;
        EventDispatcher<TwistEvent> m_onTwist;

    public:
        void PostTouch(TouchPhase phase, uint64 id, const Vec2& position, float64 timeSeconds);
        void Update();

    public:
        [[nodiscard]] EventDispatcher<PanEvent>& OnPan() noexcept { return m_onPan; }
        [[nodiscard]] EventDispatcher<PinchEvent>& OnPinch() noexcept { return m_onPinch; }
        [[nodiscard]] EventDispatcher<TwistEvent>& OnTwist() noexcept { return m_onTwist; }

    private:
        [[nodiscard]] Vec2 Centroid() const noexcept;
        [[nodiscard]] float32 Spread(const Vec2& centroid) const noexcept;

        /** The two-finger axis. Pixels are y-down, so this increases clockwise on screen. */
        [[nodiscard]] float32 Angle() const noexcept;
    };
}
