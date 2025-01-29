// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <glm/glm.hpp>

export module jpt.Camera;

import jpt.Event.Key;
import jpt.Event.MouseButton;
import jpt.Event.MouseMove;

import jpt.Constants;
import jpt.TypeDefs;

import jpt.Vector2;
import jpt.TVector3;
import jpt.Matrix44;

export namespace jpt
{
	class Camera
	{
	private:
		Matrix44 m_matrix;
		Vec3 m_move;

		Vec3 m_position = Vec3(2.0f, 2.0f, 2.0f);
		Vec3 m_forward;

		Vec2i m_lastMousePos = Vec2i(Constants<int32>::kMax);
		float m_yaw = 0.0f;
		float m_pitch = 0.0f;

	public:
		bool Init();
		void Update(TimePrecision deltaSeconds);

		void OnKey(const Event_Key& eventKey);
		void OnMouseButton(const Event_Mouse_Button& eventMouseButton);
		void OnMouseMove(const Event_Mouse_Move& eventMouseMove);

	public:
		const auto& GetMatrix() const { return m_matrix; }
	};
}