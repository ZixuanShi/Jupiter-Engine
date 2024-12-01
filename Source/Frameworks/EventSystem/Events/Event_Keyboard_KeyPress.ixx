// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Event.Keyboard.KeyPress;

import jpt.Event;
import jpt.Input.KeyCode;
import jpt.TypeDefs;
import jpt.Window;

export namespace jpt
{
	class Event_Keyboard_KeyPress : public Event
	{
	private:
		Window* m_pWindow = nullptr;	/**< Window that the event occurred */
		Input::Key m_key; /**< Key that was pressed */

	public:
		Event_Keyboard_KeyPress(Window* pWindow, Input::Key key)
			: m_pWindow(pWindow)
			, m_key(key)
		{
		}

		Window* GetWindow() const { return m_pWindow; }
		Input::Key GetKey() const { return m_key; }
	};
}