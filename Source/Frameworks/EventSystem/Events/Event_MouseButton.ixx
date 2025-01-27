// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Event.MouseButton;

import jpt.Event;
import jpt.Input.KeyCode;
import jpt.Input.Enums;
import jpt.TypeDefs;
import jpt.Window;

export namespace jpt
{
	class Event_Mouse_Button : public Event
	{
	private:
		Window* m_pWindow = nullptr;	/**< Window that the event occurred */
		int32 m_x = 0;
		int32 m_y = 0;
		Input::MouseButton m_button = Input::MouseButton::Left;
		Input::Modifier m_modifiers = Input::Modifier::Invalid;	/**< Modifier keys that were pressed */
		Input::KeyState m_state = Input::KeyState::Invalid;

	public:
		Event_Mouse_Button(Window* pWindow, int32 x, int32 y, Input::MouseButton button, Input::KeyState state, Input::Modifier modifiers = Input::Modifier::Invalid)
			: m_pWindow(pWindow)
			, m_x(x)
			, m_y(y)
			, m_button(button)
			, m_state(state)
			, m_modifiers(modifiers)
		{
		}

		Window* GetWindow() const { return m_pWindow; }
		int32 GetX() const { return m_x; }
		int32 GetY() const { return m_y; }
		Input::MouseButton GetButton() const { return m_button; }
		Input::KeyState GetState() const { return m_state; }

		bool HasModifier(Input::Modifier modifier) const
		{
			return m_modifiers.Has(modifier);
		}
	};
}