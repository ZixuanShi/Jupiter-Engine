// Copyright Jupiter Technologies]= Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Logger.h"
#include "Core/Validation/Assert.h"

#include <GLFW/glfw3.h>

export module jpt.RawInput_GLFW;

import jpt.RawInput;
import jpt.Input.KeyCode;

import jpt.Constants;
import jpt.HashMap;
import jpt.ToString;
import jpt.TypeDefs;

export namespace jpt::Input
{
	class RawInput_GLFW : public RawInput
	{
		using Super = RawInput;

	private:
		HashMap<Key, uint32> m_toGLFWKeys;
		HashMap<uint32, Key> m_fromGLFWKeys;

		HashMap<MouseButton, uint32> m_toGLFWMouseButtons;
		HashMap<uint32, MouseButton> m_fromGLFWMouseButtons;

	public:
		virtual bool PreInit() override;

	public:
		virtual uint32 FromKey(Key key) const override;
		virtual Key ToKey(uint32 key) const override;

		virtual uint32 FromMouseButton(MouseButton mouseButton) const override;
		virtual MouseButton ToMouseButton(uint32 mouseButton) const override;
	};

	bool RawInput_GLFW::PreInit()
	{
		m_toGLFWKeys.SetShouldGrow(false);
		m_toGLFWKeys.ResizeBuckets(Key::Count());

		m_fromGLFWKeys.SetShouldGrow(false);
		m_fromGLFWKeys.ResizeBuckets(Key::Count());

		m_toGLFWMouseButtons.SetShouldGrow(false);
		m_toGLFWMouseButtons.ResizeBuckets(MouseButton::Count());

		m_fromGLFWMouseButtons.SetShouldGrow(false);
		m_fromGLFWMouseButtons.ResizeBuckets(MouseButton::Count());

		// Mouse
		m_toGLFWMouseButtons[MouseButton::Left]    = GLFW_MOUSE_BUTTON_LEFT;
		m_toGLFWMouseButtons[MouseButton::Right]   = GLFW_MOUSE_BUTTON_RIGHT;
		m_toGLFWMouseButtons[MouseButton::Wheel]   = GLFW_MOUSE_BUTTON_MIDDLE;
		m_toGLFWMouseButtons[MouseButton::Button4] = GLFW_MOUSE_BUTTON_4;
		m_toGLFWMouseButtons[MouseButton::Button5] = GLFW_MOUSE_BUTTON_5;
		m_toGLFWMouseButtons[MouseButton::Button6] = GLFW_MOUSE_BUTTON_6;
		m_toGLFWMouseButtons[MouseButton::Button7] = GLFW_MOUSE_BUTTON_7;

		// Keyboard
		m_toGLFWKeys[Key::Keyboard_A] = GLFW_KEY_A;
		m_toGLFWKeys[Key::Keyboard_B] = GLFW_KEY_B;
		m_toGLFWKeys[Key::Keyboard_C] = GLFW_KEY_C;
		m_toGLFWKeys[Key::Keyboard_D] = GLFW_KEY_D;
		m_toGLFWKeys[Key::Keyboard_E] = GLFW_KEY_E;
		m_toGLFWKeys[Key::Keyboard_F] = GLFW_KEY_F;
		m_toGLFWKeys[Key::Keyboard_G] = GLFW_KEY_G;
		m_toGLFWKeys[Key::Keyboard_H] = GLFW_KEY_H;
		m_toGLFWKeys[Key::Keyboard_I] = GLFW_KEY_I;
		m_toGLFWKeys[Key::Keyboard_J] = GLFW_KEY_J;
		m_toGLFWKeys[Key::Keyboard_K] = GLFW_KEY_K;
		m_toGLFWKeys[Key::Keyboard_L] = GLFW_KEY_L;
		m_toGLFWKeys[Key::Keyboard_M] = GLFW_KEY_M;
		m_toGLFWKeys[Key::Keyboard_N] = GLFW_KEY_N;
		m_toGLFWKeys[Key::Keyboard_O] = GLFW_KEY_O;
		m_toGLFWKeys[Key::Keyboard_P] = GLFW_KEY_P;
		m_toGLFWKeys[Key::Keyboard_Q] = GLFW_KEY_Q;
		m_toGLFWKeys[Key::Keyboard_R] = GLFW_KEY_R;
		m_toGLFWKeys[Key::Keyboard_S] = GLFW_KEY_S;
		m_toGLFWKeys[Key::Keyboard_T] = GLFW_KEY_T;
		m_toGLFWKeys[Key::Keyboard_U] = GLFW_KEY_U;
		m_toGLFWKeys[Key::Keyboard_V] = GLFW_KEY_V;
		m_toGLFWKeys[Key::Keyboard_W] = GLFW_KEY_W;
		m_toGLFWKeys[Key::Keyboard_X] = GLFW_KEY_X;
		m_toGLFWKeys[Key::Keyboard_Y] = GLFW_KEY_Y;
		m_toGLFWKeys[Key::Keyboard_Z] = GLFW_KEY_Z;
		m_toGLFWKeys[Key::Keyboard_0] = GLFW_KEY_0;
		m_toGLFWKeys[Key::Keyboard_1] = GLFW_KEY_1;
		m_toGLFWKeys[Key::Keyboard_2] = GLFW_KEY_2;
		m_toGLFWKeys[Key::Keyboard_3] = GLFW_KEY_3;
		m_toGLFWKeys[Key::Keyboard_4] = GLFW_KEY_4;
		m_toGLFWKeys[Key::Keyboard_5] = GLFW_KEY_5;
		m_toGLFWKeys[Key::Keyboard_6] = GLFW_KEY_6;
		m_toGLFWKeys[Key::Keyboard_7] = GLFW_KEY_7;
		m_toGLFWKeys[Key::Keyboard_8] = GLFW_KEY_8;
		m_toGLFWKeys[Key::Keyboard_9] = GLFW_KEY_9;
		m_toGLFWKeys[Key::Keyboard_F1] =  GLFW_KEY_F1;
		m_toGLFWKeys[Key::Keyboard_F2] =  GLFW_KEY_F2;
		m_toGLFWKeys[Key::Keyboard_F3] =  GLFW_KEY_F3;
		m_toGLFWKeys[Key::Keyboard_F4] =  GLFW_KEY_F4;
		m_toGLFWKeys[Key::Keyboard_F5] =  GLFW_KEY_F5;
		m_toGLFWKeys[Key::Keyboard_F6] =  GLFW_KEY_F6;
		m_toGLFWKeys[Key::Keyboard_F7] =  GLFW_KEY_F7;
		m_toGLFWKeys[Key::Keyboard_F8] =  GLFW_KEY_F8;
		m_toGLFWKeys[Key::Keyboard_F9] =  GLFW_KEY_F9;
		m_toGLFWKeys[Key::Keyboard_F10] = GLFW_KEY_F10;
		m_toGLFWKeys[Key::Keyboard_F11] = GLFW_KEY_F11;
		m_toGLFWKeys[Key::Keyboard_F12] = GLFW_KEY_F12;

		// Special keys
		m_toGLFWKeys[Key::Keyboard_Escape] = GLFW_KEY_ESCAPE;
		m_toGLFWKeys[Key::Keyboard_Tab] = GLFW_KEY_TAB;
		m_toGLFWKeys[Key::Keyboard_CapsLock] = GLFW_KEY_CAPS_LOCK;
		m_toGLFWKeys[Key::Keyboard_Shift_Left] = GLFW_KEY_LEFT_SHIFT;
		m_toGLFWKeys[Key::Keyboard_Shift_Right] = GLFW_KEY_RIGHT_SHIFT;
		m_toGLFWKeys[Key::Keyboard_Ctrl_Left] = GLFW_KEY_LEFT_CONTROL;
		m_toGLFWKeys[Key::Keyboard_Ctrl_Right] = GLFW_KEY_RIGHT_CONTROL;
		m_toGLFWKeys[Key::Keyboard_Alt_Left] = GLFW_KEY_LEFT_ALT;
		m_toGLFWKeys[Key::Keyboard_Alt_Right] = GLFW_KEY_RIGHT_ALT;
		m_toGLFWKeys[Key::Keyboard_Space] = GLFW_KEY_SPACE;
		m_toGLFWKeys[Key::Keyboard_Enter] = GLFW_KEY_ENTER;
		m_toGLFWKeys[Key::Keyboard_Backspace] = GLFW_KEY_BACKSPACE;
		m_toGLFWKeys[Key::Keyboard_Bracket_Left] = GLFW_KEY_LEFT_BRACKET;
		m_toGLFWKeys[Key::Keyboard_Bracket_Right] = GLFW_KEY_RIGHT_BRACKET;
		m_toGLFWKeys[Key::Keyboard_Semicolon] = GLFW_KEY_SEMICOLON;
		m_toGLFWKeys[Key::Keyboard_Quote] = GLFW_KEY_APOSTROPHE;
		m_toGLFWKeys[Key::Keyboard_Comma] = GLFW_KEY_COMMA;
		m_toGLFWKeys[Key::Keyboard_Period] = GLFW_KEY_PERIOD;
		m_toGLFWKeys[Key::Keyboard_Slash] = GLFW_KEY_SLASH;
		m_toGLFWKeys[Key::Keyboard_Backslash] = GLFW_KEY_BACKSLASH;
		m_toGLFWKeys[Key::Keyboard_Tilde] = GLFW_KEY_GRAVE_ACCENT;
		m_toGLFWKeys[Key::Keyboard_Equal] = GLFW_KEY_EQUAL;
		m_toGLFWKeys[Key::Keyboard_Hyphen] = GLFW_KEY_MINUS;

		// Function Keys
		m_toGLFWKeys[Key::Keyboard_Insert] = GLFW_KEY_INSERT;
		m_toGLFWKeys[Key::Keyboard_Delete] = GLFW_KEY_DELETE;
		m_toGLFWKeys[Key::Keyboard_Home] = GLFW_KEY_HOME;
		m_toGLFWKeys[Key::Keyboard_End] = GLFW_KEY_END;
		m_toGLFWKeys[Key::Keyboard_PageUp] = GLFW_KEY_PAGE_UP;
		m_toGLFWKeys[Key::Keyboard_PageDown] = GLFW_KEY_PAGE_DOWN;
		m_toGLFWKeys[Key::Keyboard_PrintScreen] = GLFW_KEY_PRINT_SCREEN;
		m_toGLFWKeys[Key::Keyboard_ScrollLock] = GLFW_KEY_SCROLL_LOCK;
		m_toGLFWKeys[Key::Keyboard_Pause] = GLFW_KEY_PAUSE;

		// Arrows
		m_toGLFWKeys[Key::Keyboard_Arrow_Up] = GLFW_KEY_UP;
		m_toGLFWKeys[Key::Keyboard_Arrow_Down] = GLFW_KEY_DOWN;
		m_toGLFWKeys[Key::Keyboard_Arrow_Left] = GLFW_KEY_LEFT;
		m_toGLFWKeys[Key::Keyboard_Arrow_Right] = GLFW_KEY_RIGHT;

		// Numpad
		m_toGLFWKeys[Key::Numpad_Lock] = GLFW_KEY_NUM_LOCK;
		m_toGLFWKeys[Key::Numpad_0] = GLFW_KEY_KP_0;
		m_toGLFWKeys[Key::Numpad_1] = GLFW_KEY_KP_1;
		m_toGLFWKeys[Key::Numpad_2] = GLFW_KEY_KP_2;
		m_toGLFWKeys[Key::Numpad_3] = GLFW_KEY_KP_3;
		m_toGLFWKeys[Key::Numpad_4] = GLFW_KEY_KP_4;
		m_toGLFWKeys[Key::Numpad_5] = GLFW_KEY_KP_5;
		m_toGLFWKeys[Key::Numpad_6] = GLFW_KEY_KP_6;
		m_toGLFWKeys[Key::Numpad_7] = GLFW_KEY_KP_7;
		m_toGLFWKeys[Key::Numpad_8] = GLFW_KEY_KP_8;
		m_toGLFWKeys[Key::Numpad_9] = GLFW_KEY_KP_9;
		m_toGLFWKeys[Key::Numpad_Add] = GLFW_KEY_KP_ADD;
		m_toGLFWKeys[Key::Numpad_Subtract] = GLFW_KEY_KP_SUBTRACT;
		m_toGLFWKeys[Key::Numpad_Multiply] = GLFW_KEY_KP_MULTIPLY;
		m_toGLFWKeys[Key::Numpad_Divide] = GLFW_KEY_KP_DIVIDE;
		m_toGLFWKeys[Key::Numpad_Decimal] = GLFW_KEY_KP_DECIMAL;
		m_toGLFWKeys[Key::Numpad_Enter] = GLFW_KEY_KP_ENTER;

		for (const auto& pair : m_toGLFWKeys)
		{
			m_fromGLFWKeys[pair.second] = pair.first;
		}
		for (const auto& pair : m_toGLFWMouseButtons)
		{
			m_fromGLFWMouseButtons[pair.second] = pair.first;
		}

		return true;
	}

	uint32 RawInput_GLFW::FromKey(Key key) const
	{
		if (m_toGLFWKeys.Has(key))
		{
			return m_toGLFWKeys[key];
		}

		JPT_ERROR("Unknown key code: " + key.ToString());
		return kInvalidValue<uint32>;
	}

	Key RawInput_GLFW::ToKey(uint32 key) const
	{
		if (m_fromGLFWKeys.Has(key))
		{
			return m_fromGLFWKeys[key];
		}

		JPT_ERROR("Unknown key code: " + ToString(key));
		return Key::Keyboard_0;
	}

	uint32 RawInput_GLFW::FromMouseButton(MouseButton mouseButton) const
	{
		if (m_toGLFWMouseButtons.Has(mouseButton))
		{
			return m_toGLFWMouseButtons[mouseButton];
		}

		JPT_ERROR("Unknown mouse button: " + mouseButton.ToString());
		return kInvalidValue<uint32>;
	}

	MouseButton RawInput_GLFW::ToMouseButton(uint32 mouseButton) const
	{
		if (m_fromGLFWMouseButtons.Has(mouseButton))
		{
			return m_fromGLFWMouseButtons[mouseButton];
		}

		JPT_ERROR("Unknown mouse button: " + ToString(mouseButton));
		return MouseButton::Left;
	}
}