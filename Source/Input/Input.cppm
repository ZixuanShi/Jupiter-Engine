// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Input;

import jpt.EventDispatcher;
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

    public:
        bool PreInit();

    public:
        [[nodiscard]] bool IsKeyDown(KeyCode key) const noexcept;
        [[nodiscard]] bool IsMouseButtonDown(MouseButton button) const noexcept;
        [[nodiscard]] const Vec2& GetMousePosition() const noexcept { return m_mousePosition; }

    public:
        [[nodiscard]] EventDispatcher<KeyEvent>& OnKeyDown() noexcept { return m_onKeyDown; }
        [[nodiscard]] EventDispatcher<KeyEvent>& OnKeyUp() noexcept { return m_onKeyUp; }
        [[nodiscard]] EventDispatcher<MouseButtonEvent>& OnMouseButton() noexcept { return m_onMouseButton; }
        [[nodiscard]] EventDispatcher<MouseMoveEvent>& OnMouseMove() noexcept { return m_onMouseMove; }
        [[nodiscard]] EventDispatcher<MouseScrollEvent>& OnMouseScroll() noexcept { return m_onMouseScroll; }

    public:
        // Platform -> engine.
        void PostKeyDown(KeyCode key, bool isRepeat);
        void PostKeyUp(KeyCode key);
        void PostMouseButton(MouseButton button, bool isDown, const Vec2& position);
        void PostMouseMove(const Vec2& position);
        void PostMouseScroll(const Vec2& delta, bool isPrecise);
    };
}
