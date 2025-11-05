// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Logger.h"

module jpt.Camera;

import jpt.Window;
import jpt.Framework;

import jpt.EventManager;

import jpt.Input_Enums;
import jpt.Math;

namespace jpt
{
    static constexpr Precision kMoveSpeed   = 2.5f;     // Camera movement speed
    static constexpr Precision kScrollSpeed = 0.1f;     // Camera movement speed
    static constexpr Precision kSensitivity = 0.001f;   // Camera rotation sensitivity
    static constexpr Precision kPitchLimit  = ToRadians(89.0f);  // Prevent camera flipping

    bool Camera::Init()
    {
        // Register input events
        EventManager::GetInstance().Register<Event_Key>(this, &Camera::OnKey);
        EventManager::GetInstance().Register<Event_Mouse_Button>(this, &Camera::OnMouseButton);
        EventManager::GetInstance().Register<Event_Mouse_Move>(this, &Camera::OnMouseMove);
        EventManager::GetInstance().Register<Event_Mouse_Scroll>(this, &Camera::OnMouseScroll);

        // Init position and rotation
        m_forward = (Vec3(0.0f) - m_positionWS).Normalized();
        m_pitch   = Asin(m_forward.y);
        m_yaw     = Atan2(m_forward.x, m_forward.z);

        return true;
    }

    void Camera::Update(TimePrecision deltaSeconds)
    {
        const Vec3 right = Vec3::Up().Cross(m_forward);

        m_positionWS +=      right * m_mover.x * kMoveSpeed * deltaSeconds;
        m_positionWS += Vec3::Up() * m_mover.y * kMoveSpeed * deltaSeconds;
        m_positionWS +=  m_forward * m_mover.z * kMoveSpeed * deltaSeconds;
    }

    void Camera::OnKey(const Event_Key& eventKey)
    {
        const Input::Key key = eventKey.GetKey();
        const Input::KeyState keyState = eventKey.GetState();
        const bool keyDown = (keyState == Input::KeyState::Pressed) || (keyState == Input::KeyState::Held);

        switch (key.Value())
        {
            // Left/Right
            case Input::Key::A:
            case Input::Key::D:
                m_mover.x = keyDown ? (key == Input::Key::A ? 1.0f : -1.0f) : 0.0f;
                break;

            // Up/Down
            case Input::Key::E:
            case Input::Key::Q:
                m_mover.y = keyDown ? (key == Input::Key::E ? 1.0f : -1.0f) : 0.0f;
                break;

            // Fowrard/Backward
            case Input::Key::W:
            case Input::Key::S:
                m_mover.z = keyDown ? (key == Input::Key::W ? 1.0f : -1.0f) : 0.0f;
                break;
        }
    }

    void Camera::OnMouseButton(const Event_Mouse_Button& eventMouseButton)
    {
        const Input::MouseButton button = eventMouseButton.GetButton();
        const Input::KeyState state     = eventMouseButton.GetState();

        // Right mouse button. FPS camera rotation
        if (button == Input::MouseButton::Right)
        {
            m_mouseMode = MouseMode::Orbit;
        }
        // Wheel button. Move camera up/down and left/right
        else if (button == Input::MouseButton::Wheel)
        {
            m_mouseMode = MouseMode::Pan;
        }
        else
        {
            return;
        }

        if (state == Input::KeyState::Pressed)
        {
            const double x = eventMouseButton.GetX();
            const double y = eventMouseButton.GetY();
            m_lockMousePos = Vec2i(static_cast<int32>(x), static_cast<int32>(y));

            m_pWindow = eventMouseButton.GetWindow();
            m_pWindow->SetCursorVisible(false);
        }
        else if (state == Input::KeyState::Released)
        {
            m_lockMousePos = Vec2i(Constants<int32>::kMax);

            m_pWindow->SetCursorVisible(true);
            m_pWindow = nullptr;
        }
    }

    void Camera::OnMouseMove(const Event_Mouse_Move& eventMouseMove)
    {
        // If the last mouse position is invalid, means the right mouse button is not pressed and shouldn't rotate by mouse axis. See OnMouseButton()
        if (m_lockMousePos == Vec2i(Constants<int32>::kMax))
        {
            return;
        }

        // Calculate the change in mouse position
        const double x = eventMouseMove.GetX();
        const double y = eventMouseMove.GetY();
        const Precision dx = static_cast<Precision>(m_lockMousePos.x) - static_cast<Precision>(x);
        const Precision dy = static_cast<Precision>(m_lockMousePos.y) - static_cast<Precision>(y);

        switch (m_mouseMode)
        {
            case MouseMode::Orbit:
            {
                // Update the yaw and pitch relative to the mouse movement
                m_pitch += dy * kSensitivity;
                m_yaw   += dx * kSensitivity;

                // Clamp the pitch and yaw
                m_pitch = Clamp(m_pitch, -kPitchLimit, kPitchLimit);
                m_yaw   = Modf(m_yaw, TwoPi);

                // Calculate the new forward vector
                m_forward.x = Cos(m_pitch) * Sin(m_yaw);
                m_forward.y = Sin(m_pitch);
                m_forward.z = Cos(m_pitch) * Cos(m_yaw);
                m_forward.Normalize();

                break;
            }
            case MouseMode::Pan:
            {
                // Move the camera up/down and left/right
                m_positionWS += Vec3::Up().Cross(m_forward) * dx * kSensitivity;
                m_positionWS += Vec3::Up() * dy * kSensitivity;

                break;
            }
            default:
            {
                JPT_ERROR("Unknown mouse mode");
                break;
            }
        }

        // Update the last mouse position
        m_pWindow->SetMousePosition(m_lockMousePos);
    }

    void Camera::OnMouseScroll(const Event_Mouse_Scroll& eventMouseScroll)
    {
        const double y = eventMouseScroll.GetY();

        m_positionWS += m_forward * static_cast<Precision>(y) * kScrollSpeed;
    }

    Matrix44 Camera::CalcMatrix() const
    {
        return Matrix44::LookAt(m_positionWS, m_positionWS + m_forward);
    }

    Vec3 Camera::GetForward() const
    {
        return m_forward;
    }
}