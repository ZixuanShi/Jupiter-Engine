// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Event.Key;

import jpt.Event;
import jpt.Input.Enums;
import jpt.Input.KeyCode;
import jpt.TypeDefs;
import jpt.Window;

export namespace jpt
{
	class Event_Key : public Event
	{
	private:
		Window* m_pWindow = nullptr;	/**< Window that the event occurred */
		Input::Key m_key;				/**< Key that was pressed */
		Input::Modifier m_modifiers = Input::Modifier::Invalid;	/**< Modifier keys that were pressed */
		Input::KeyState m_state = Input::KeyState::Invalid;		/**< State of the key */

	public:
		Event_Key(Window* pWindow, Input::Key key, Input::KeyState state, Input::Modifier modifiers = Input::Modifier::Invalid)
			: m_pWindow(pWindow)
			, m_key(key)
			, m_state(state)
			, m_modifiers(modifiers)
		{
		}

		Window* GetWindow() const { return m_pWindow; }
		Input::Key GetKey() const { return m_key; }
		Input::KeyState GetState() const { return m_state; }
		
		bool HasModifier(Input::Modifier modifier) const 
		{ 
			return m_modifiers.Has(modifier);
		}
	};
}