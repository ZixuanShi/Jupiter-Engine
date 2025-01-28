// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <glm/glm.hpp>

export module jpt.Camera;

import jpt.Event.Key;
import jpt.Event.MouseButton;
import jpt.Event.MouseMove;

import jpt.Constants;

import jpt.Time.TypeDefs;

export namespace jpt
{
	class Camera
	{
	private:
		glm::mat4 m_matrix;
		glm::vec3 m_move = glm::vec3(0.0f);

		glm::vec3 m_position = glm::vec3(2.0f, 2.0f, 2.0f);

		glm::vec2 m_lastMousePos = glm::vec2(Constants<float>::kMax);
		float m_yaw = 0.0f;
		float m_pitch = 0.0f;

		bool m_isRotating = false;

	public:
		bool Init();
		void Update(TimePrecision deltaSeconds);

		void OnKey(const Event_Key& eventKey);
		void OnMouseButton(const Event_Mouse_Button& eventMouseButton);
		void OnMouseMove(const Event_Mouse_Move& eventMouseMove);

	public:
		const auto& GetMatrix() const { return m_matrix; }

	private:
		void UpdateViewMatrix();
	};
}