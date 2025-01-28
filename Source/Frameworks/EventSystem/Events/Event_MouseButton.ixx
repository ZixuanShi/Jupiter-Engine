// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Event.MouseButton;

import jpt.Event;
import jpt.Input.Enums;
import jpt.TypeDefs;
import jpt.Window;

export namespace jpt
{
	class Event_Mouse_Button : public Event
	{
	private:
		Window* m_pWindow = nullptr;	/**< Window that the event occurred */
		double m_x = 0;
		double m_y = 0;
		Input::MouseButton m_button = Input::MouseButton::Left;
		Input::Modifier m_modifiers = Input::Modifier::Invalid;	/**< Modifier keys that were pressed */
		Input::KeyState m_state = Input::KeyState::Invalid;

	public:
		Event_Mouse_Button(Window* pWindow, double x, double y, Input::MouseButton button, Input::KeyState state, Input::Modifier modifiers = Input::Modifier::Invalid)
			: m_pWindow(pWindow)
			, m_x(x)
			, m_y(y)
			, m_button(button)
			, m_state(state)
			, m_modifiers(modifiers)
		{
		}

		Window* GetWindow() const { return m_pWindow; }
		double GetX() const { return m_x; }
		double GetY() const { return m_y; }
		Input::MouseButton GetButton() const { return m_button; }
		Input::KeyState GetState() const { return m_state; }

		bool HasModifier(Input::Modifier modifier) const
		{
			return m_modifiers.Has(modifier);
		}
	};
}