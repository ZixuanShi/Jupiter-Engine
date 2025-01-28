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

import jpt.Event.Manager;
import jpt.Event.Key;
import jpt.Event.MouseButton;
import jpt.Event.MouseMove;

import jpt.Input.Enums;
import jpt.Math;

namespace jpt
{
	bool Camera::Init()
	{
		// Register input events
		EventManager::GetInstance().Register<Event_Key>(this, &Camera::OnKey);
		EventManager::GetInstance().Register<Event_Mouse_Button>(this, &Camera::OnMouseButton);
		EventManager::GetInstance().Register<Event_Mouse_Move>(this, &Camera::OnMouseMove);

		// Init position and rotation
		const glm::vec3 direction = glm::normalize(glm::vec3(0.0f) - m_position);
		m_yaw = std::atan2(direction.x, direction.z);
		m_pitch = std::asin(direction.y);

		UpdateViewMatrix();

		return true;
	}

	void Camera::Update(TimePrecision deltaSeconds)
	{
		static constexpr float kMoveSpeed = 2.5f;

		const glm::vec3 forward = glm::normalize(glm::vec3(m_matrix[0][2], m_matrix[1][2], m_matrix[2][2]));
		const glm::vec3 right = glm::normalize(glm::vec3(m_matrix[0][0], m_matrix[1][0], m_matrix[2][0]));

		m_position += forward * m_move.z * kMoveSpeed * static_cast<float>(deltaSeconds);
		m_position += right * m_move.x * kMoveSpeed * static_cast<float>(deltaSeconds);

		UpdateViewMatrix();
	}

	void Camera::OnKey(const Event_Key& eventKey)
	{
		Input::Key key = eventKey.GetKey();
		Input::KeyState keyState = eventKey.GetState();

		switch (key.Value())
		{
		case Input::Key::W:
			if (keyState == Input::KeyState::Pressed)
			{
				m_move.z = -1;
			}
			else if (keyState == Input::KeyState::Released)
			{
				m_move.z = 0;
			}
			break;
		case Input::Key::S:
			if (keyState == Input::KeyState::Pressed)
			{
				m_move.z = 1;
			}
			else if (keyState == Input::KeyState::Released)
			{
				m_move.z = 0;
			}
			break;
		case Input::Key::D:
			if (keyState == Input::KeyState::Pressed)
			{
				m_move.x = 1;
			}
			else if (keyState == Input::KeyState::Released)
			{
				m_move.x = 0;
			}
			break;
		case Input::Key::A:
			if (keyState == Input::KeyState::Pressed)
			{
				m_move.x = -1;
			}
			else if (keyState == Input::KeyState::Released)
			{
				m_move.x = 0;
			}
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
				m_isRotating = true;
				m_lastMousePos = glm::vec2(eventMouseButton.GetX(), eventMouseButton.GetY());
			}
			else if (state == Input::KeyState::Released)
			{
				m_isRotating = false;
			}
		}
	}

	void Camera::OnMouseMove(const Event_Mouse_Move& eventMouseMove)
	{
		static constexpr float kSensitivity = 0.001f;

		if (!m_isRotating)
		{
			return;
		}

		const double x = eventMouseMove.GetX();
		const double y = eventMouseMove.GetY();

		const float dx = static_cast<float>(x) - m_lastMousePos.x;
		const float dy = static_cast<float>(y) - m_lastMousePos.y;

		m_yaw   -= dx * kSensitivity;
		m_pitch -= dy * kSensitivity;

		UpdateViewMatrix();

		m_lastMousePos = glm::vec2(x, y);
	}

	void Camera::UpdateViewMatrix()
	{
		// Calculate the new front vector
		glm::vec3 front;
		front.x = std::cos(m_pitch) * std::sin(m_yaw);
		front.y = std::sin(m_pitch);
		front.z = std::cos(m_pitch) * std::cos(m_yaw);
		front = glm::normalize(front);

		// Calculate the new right vector and up vector
		glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));
		glm::vec3 up = glm::normalize(glm::cross(right, front));

		// Update the view matrix
		m_matrix = glm::lookAt(m_position, m_position + front, up);
	}
}