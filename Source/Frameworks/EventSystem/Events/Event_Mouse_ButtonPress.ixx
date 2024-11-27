// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Event.Mouse.ButtonPress;

import jpt.Event;
import jpt.Input.KeyCode;
import jpt.TypeDefs;
import jpt.Window;

export namespace jpt
{
	class Event_Mouse_ButtonPress : public Event
	{
	private:
		Window* m_pWindow = nullptr;	/**< Window that the event occurred */
		int32 m_x = 0;
		int32 m_y = 0;
		Input::KeyCode m_button = Input::KeyCode::Invalid;

	public:
		Event_Mouse_ButtonPress(Window* pWindow, int32 x, int32 y, Input::KeyCode button)
			: m_pWindow(pWindow)
			, m_x(x)
			, m_y(y)
			, m_button(button)
		{
		}

		Window* GetWindow() const { return m_pWindow; }
		int32 GetX() const { return m_x; }
		int32 GetY() const { return m_y; }
		Input::KeyCode GetButton() const { return m_button; }
	};
}