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
		Input::Key m_key;				/**< Key that was pressed */
		Input::Modifier m_modifiers = Input::Modifier::Invalid;	/**< Modifier keys that were pressed */

	public:
		Event_Keyboard_KeyPress(Window* pWindow, Input::Key key, Input::Modifier modifiers = Input::Modifier::Invalid)
			: m_pWindow(pWindow)
			, m_key(key)
			, m_modifiers(modifiers)
		{
		}

		Window* GetWindow() const { return m_pWindow; }
		Input::Key GetKey() const { return m_key; }
		
		bool HasModifier(Input::Modifier modifier) const 
		{ 
			return m_modifiers.Has(modifier);
		}
	};
}