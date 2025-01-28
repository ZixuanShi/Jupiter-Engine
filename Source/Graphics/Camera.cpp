// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

module jpt.Camera;

import jpt.Event.Manager;
import jpt.Event.Key;
import jpt.Event.MouseButton;
import jpt.Event.MouseMove;

import jpt.Input.KeyCode;
import jpt.Input.Enums;

namespace jpt
{
	bool Camera::Init()
	{
		EventManager::GetInstance().Register<Event_Key>(this, &Camera::OnKey);
		EventManager::GetInstance().Register<Event_Mouse_Button>(this, &Camera::OnMouseButton);
		EventManager::GetInstance().Register<Event_Mouse_Move>(this, &Camera::OnMouseMove);

		m_matrix = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		return true;
	}

	void Camera::Update(TimePrecision deltaSeconds)
	{
		static constexpr float kCameraSpeed = 2.5f;

		const glm::vec3 forward = glm::normalize(glm::vec3(m_matrix[0][2], m_matrix[1][2], m_matrix[2][2]));
		const glm::vec3 right = glm::normalize(glm::vec3(m_matrix[0][0], m_matrix[1][0], m_matrix[2][0]));

		m_matrix = glm::translate(m_matrix, forward * m_move.z * kCameraSpeed * static_cast<float>(deltaSeconds));
		m_matrix = glm::translate(m_matrix, right * m_move.x * kCameraSpeed * static_cast<float>(deltaSeconds));
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
				m_move.z = 1;
			}
			else if (keyState == Input::KeyState::Released)
			{
				m_move.z = 0;
			}
			break;
		case Input::Key::S:
			if (keyState == Input::KeyState::Pressed)
			{
				m_move.z = -1;
			}
			else if (keyState == Input::KeyState::Released)
			{
				m_move.z = 0;
			}
			break;
		case Input::Key::D:
			if (keyState == Input::KeyState::Pressed)
			{
				m_move.x = -1;
			}
			else if (keyState == Input::KeyState::Released)
			{
				m_move.x = 0;
			}
			break;
		case Input::Key::A:
			if (keyState == Input::KeyState::Pressed)
			{
				m_move.x = 1;
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
			m_isRotating = (state == Input::KeyState::Pressed);
		}
	}

	void Camera::OnMouseMove(const Event_Mouse_Move& eventMouseMove)
	{
	}
}