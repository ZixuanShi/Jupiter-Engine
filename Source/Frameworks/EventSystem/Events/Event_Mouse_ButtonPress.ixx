// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Event.Mouse.ButtonPress;

import jpt.Event;
import jpt.Input.KeyCode;
import jpt.TypeDefs;

export namespace jpt
{
	class Event_Mouse_ButtonPress : public Event
	{
	private:
		int32 m_x;
		int32 m_y;
		Input::KeyCode m_button;

	public:
		Event_Mouse_ButtonPress(int32 x, int32 y, Input::KeyCode button)
			: m_x(x)
			, m_y(y)
			, m_button(button)
		{
		}

		int32 GetX() const { return m_x; }
		int32 GetY() const { return m_y; }
		Input::KeyCode GetButton() const { return m_button; }
	};
}