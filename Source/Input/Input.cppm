// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Input;

import jpt.EventDispatcher;
import jpt.GestureRecognizer;
import jpt.InputCodes;
import jpt.InputEvents;
import jpt.TypeDefs;
import jpt.Vector2;
import std;

export namespace jpt
{
    /** Continuous state is polled, discrete transitions are dispatched. */
    class Input
    {
    private:
        std::bitset<static_cast<usize>(KeyCode::Count)> m_keysDown;
        std::bitset<static_cast<usize>(MouseButton::Count)> m_buttonsDown;
        Vec2 m_mousePosition = Vec2::Zero();

        EventDispatcher<KeyEvent> m_onKeyDown;
        EventDispatcher<KeyEvent> m_onKeyUp;
        EventDispatcher<MouseButtonEvent> m_onMouseButton;
        EventDispatcher<MouseMoveEvent> m_onMouseMove;
        EventDispatcher<MouseScrollEvent> m_onMouseScroll;

        GestureRecognizer m_gestures;

        bool m_keyboardCaptured = false;
        bool m_mouseCaptured = false;

    public:
        bool PreInit();
        void Update();

    public:
        [[nodiscard]] bool IsKeyDown(KeyCode key) const noexcept;
        [[nodiscard]] bool IsMouseButtonDown(MouseButton button) const noexcept;
        [[nodiscard]] const Vec2& GetMousePosition() const noexcept { return m_mousePosition; }
        void SetCaptured(bool keyboard, bool mouse) noexcept;

    public:
        [[nodiscard]] EventDispatcher<KeyEvent>& OnKeyDown() noexcept { return m_onKeyDown; }
        [[nodiscard]] EventDispatcher<KeyEvent>& OnKeyUp() noexcept { return m_onKeyUp; }
        [[nodiscard]] EventDispatcher<MouseButtonEvent>& OnMouseButton() noexcept { return m_onMouseButton; }
        [[nodiscard]] EventDispatcher<MouseMoveEvent>& OnMouseMove() noexcept { return m_onMouseMove; }
        [[nodiscard]] EventDispatcher<MouseScrollEvent>& OnMouseScroll() noexcept { return m_onMouseScroll; }
        [[nodiscard]] EventDispatcher<PanEvent>& OnPan() noexcept { return m_gestures.OnPan(); }
        [[nodiscard]] EventDispatcher<PinchEvent>& OnPinch() noexcept { return m_gestures.OnPinch(); }
        [[nodiscard]] EventDispatcher<TwistEvent>& OnTwist() noexcept { return m_gestures.OnTwist(); }

    public:
        // Platform -> engine.
        void PostKeyDown(KeyCode key, bool isRepeat);
        void PostKeyUp(KeyCode key);
        void PostMouseButton(MouseButton button, bool isDown, const Vec2& position);
        void PostMouseMove(const Vec2& position);
        void PostMouseScroll(const Vec2& delta, bool isPrecise);
        void PostTouch(TouchPhase phase, uint64 id, const Vec2& position, float64 timeSeconds, TouchDevice device);

#if !IS_CONFIG_RELEASE
    private:
        void LogKeyDown(const KeyEvent& event);
        void LogKeyUp(const KeyEvent& event);
        void LogMouseButton(const MouseButtonEvent& event);
        void LogMouseScroll(const MouseScrollEvent& event);
#endif
    };
}
