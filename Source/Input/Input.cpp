// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module jpt.Input;

import jpt.Logger;
import jpt.TypeDefs;

namespace jpt
{
    bool Input::PreInit()
    {
#if !IS_CONFIG_RELEASE
        // Delete once something real listens.
        m_onKeyDown.Add([](const KeyEvent& event)
            {
                Debug::Log("Key down: {}{}", ToString(event.key), event.isRepeat ? " (repeat)" : "");
            });

        m_onKeyUp.Add([](const KeyEvent& event)
            {
                Debug::Log("Key up: {}", ToString(event.key));
            });

        m_onMouseButton.Add([](const MouseButtonEvent& event)
            {
                Debug::Log("Mouse {} {} at ({:.1f}, {:.1f})", ToString(event.button),
                           event.isDown ? "down" : "up", event.position.x, event.position.y);
            });

        m_onMouseScroll.Add([](const MouseScrollEvent& event)
            {
                Debug::Log("Scroll: ({:.3f}, {:.3f}){}", event.delta.x, event.delta.y,
                           event.isPrecise ? " precise" : "");
            });

        // Mouse move is not logged: it would bury everything else.
#endif

        return true;
    }

    void Input::Update()
    {
        m_gestures.Update();
    }

    bool Input::IsKeyDown(KeyCode key) const noexcept
    {
        return m_keysDown[static_cast<usize>(key)];
    }

    bool Input::IsMouseButtonDown(MouseButton button) const noexcept
    {
        return m_buttonsDown[static_cast<usize>(button)];
    }

    void Input::PostKeyDown(KeyCode key, bool isRepeat)
    {
        m_keysDown.set(static_cast<usize>(key));
        m_onKeyDown.Dispatch(KeyEvent{ .key = key, .isRepeat = isRepeat });
    }

    void Input::PostKeyUp(KeyCode key)
    {
        m_keysDown.reset(static_cast<usize>(key));
        m_onKeyUp.Dispatch(KeyEvent{ .key = key });
    }

    void Input::PostMouseButton(MouseButton button, bool isDown, const Vec2& position)
    {
        m_buttonsDown.set(static_cast<usize>(button), isDown);
        m_mousePosition = position;
        m_onMouseButton.Dispatch(MouseButtonEvent{ .button = button, .isDown = isDown, .position = position });
    }

    void Input::PostMouseMove(const Vec2& position)
    {
        const Vec2 delta = position - m_mousePosition;
        m_mousePosition = position;
        m_onMouseMove.Dispatch(MouseMoveEvent{ .position = position, .delta = delta });
    }

    void Input::PostMouseScroll(const Vec2& delta, bool isPrecise)
    {
        m_onMouseScroll.Dispatch(MouseScrollEvent{ .delta = delta, .isPrecise = isPrecise });
    }

    void Input::PostTouch(TouchPhase phase, uint64 id, const Vec2& position, float64 timeSeconds)
    {
        m_gestures.PostTouch(phase, id, position, timeSeconds);
    }
}
