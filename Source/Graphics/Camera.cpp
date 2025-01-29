// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Logger.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>

module jpt.Camera;

import jpt.Window;
import jpt.Framework;

import jpt.Event.Manager;
import jpt.Event.Key;
import jpt.Event.MouseButton;
import jpt.Event.MouseMove;

import jpt.Input.Enums;
import jpt.Math;

namespace jpt
{
	static constexpr Precision kMoveSpeed   = 2.5f;		// Camera movement speed
	static constexpr Precision kSensitivity = 0.001f;	// Camera rotation sensitivity
	static constexpr Precision kPitchLimit  = glm::radians(89.0f);  // Prevent camera flipping

	bool Camera::Init()
	{
		// Register input events
		EventManager::GetInstance().Register<Event_Key>(this, &Camera::OnKey);
		EventManager::GetInstance().Register<Event_Mouse_Button>(this, &Camera::OnMouseButton);
		EventManager::GetInstance().Register<Event_Mouse_Move>(this, &Camera::OnMouseMove);

		// Init position and rotation
		m_forward = (Vec3(0.0f) - m_position).Normalized();
		m_yaw   = std::atan2(m_forward.x, m_forward.z);
		m_pitch = std::asin(m_forward.y);

		return true;
	}

	void Camera::Update(TimePrecision deltaSeconds)
	{
		const Vec3 right = Vec3::Cross(m_forward, Vec3::Up());

		m_position += m_forward * m_move.z * kMoveSpeed * deltaSeconds;
		m_position +=     right * m_move.x * kMoveSpeed * deltaSeconds;

		m_matrix = Matrix44::LookAt(m_position, m_position + m_forward, Vec3::Up());
	}

	void Camera::OnKey(const Event_Key& eventKey)
	{
		const Input::Key key = eventKey.GetKey();
		const Input::KeyState keyState = eventKey.GetState();
		const bool keyDown = (keyState == Input::KeyState::Pressed) || (keyState == Input::KeyState::Held);

		switch (key.Value())
		{
		case Input::Key::W:
		case Input::Key::S:
			m_move.z = keyDown ? (key == Input::Key::W ? 1.0f : -1.0f) : 0.0f;
			break;

		case Input::Key::D:
		case Input::Key::A:
			m_move.x = keyDown ? (key == Input::Key::D ? 1.0f : -1.0f) : 0.0f;
			break;
		}
	}

	void Camera::OnMouseButton(const Event_Mouse_Button& eventMouseButton)
	{
		Input::MouseButton button = eventMouseButton.GetButton();
		Input::KeyState state = eventMouseButton.GetState();

		if (button == Input::MouseButton::Right)
		{
			if (state == Input::KeyState::Pressed)
			{
				m_pWindow = eventMouseButton.GetWindow();

				double x = eventMouseButton.GetX();
				double y = eventMouseButton.GetY();
				m_lockMousePos = Vec2i(static_cast<int32>(x), static_cast<int32>(y));

				m_pWindow->SetCursorVisible(false);
			}
			else if (state == Input::KeyState::Released)
			{
				m_lockMousePos = Vec2i(Constants<glm::i32>::kMax);
				m_pWindow->SetCursorVisible(true);
			}
		}
	}

	void Camera::OnMouseMove(const Event_Mouse_Move& eventMouseMove)
	{
		// If the last mouse position is invalid, means the right mouse button is not pressed and shouldn't rotate by mouse axis. See OnMouseButton()
		if (m_lockMousePos == Vec2i(Constants<glm::i32>::kMax))
		{
			return;
		}

		// Calculate the change in mouse position
		const double x = eventMouseMove.GetX();
		const double y = eventMouseMove.GetY();
		const Precision dx = static_cast<Precision>(m_lockMousePos.x) - static_cast<Precision>(x);
		const Precision dy = static_cast<Precision>(m_lockMousePos.y) - static_cast<Precision>(y);

		// Update the yaw and pitch relative to the mouse movement
		m_yaw   += dx * kSensitivity;
		m_pitch += dy * kSensitivity;

		// Clamp the pitch to prevent the camera from flipping
		m_pitch = Clamp(m_pitch, -kPitchLimit, kPitchLimit);

		// Calculate the new forward vector
		m_forward.x = std::cos(m_pitch) * std::sin(m_yaw);
		m_forward.y = std::sin(m_pitch);
		m_forward.z = std::cos(m_pitch) * std::cos(m_yaw);
		m_forward.Normalize();

		// Update the last mouse position
		m_pWindow->SetMousePosition(m_lockMousePos);
	}
}