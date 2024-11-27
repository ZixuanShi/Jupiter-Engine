// Copyright Jupiter Technologies]= Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Logger.h"
#include "Debugging/Assert.h"

#include <GLFW/glfw3.h>

export module jpt.InputManager_GLFW;

import jpt.InputManager;
import jpt.Input.KeyCode;

import jpt.Constants;
import jpt.HashMap;
import jpt.ToString;
import jpt.TypeDefs;

export namespace jpt::Input
{
	class InputManager_GLFW : public InputManager
	{
		using Super = InputManager;

	private:
		HashMap<KeyCode, uint32> m_toGLFW;
		HashMap<uint32, KeyCode> m_fromGLFW;


	public:
		virtual bool PreInit() override;

	public:
		virtual uint32 FromKeyCode(KeyCode) const override;
		virtual KeyCode ToKeyCode(uint32) const override;
	};

	bool InputManager_GLFW::PreInit()
	{
		m_toGLFW.SetShouldGrow(false);
		m_toGLFW.ResizeBuckets(KeyCode::Count() - 1);
		m_fromGLFW.SetShouldGrow(false);
		m_fromGLFW.ResizeBuckets(KeyCode::Count() - 1);

		// Mouse
		m_toGLFW[KeyCode::Mouse_Left]    = GLFW_MOUSE_BUTTON_LEFT;
		m_toGLFW[KeyCode::Mouse_Right]   = GLFW_MOUSE_BUTTON_RIGHT;
		m_toGLFW[KeyCode::Mouse_Wheel]   = GLFW_MOUSE_BUTTON_MIDDLE;
		m_toGLFW[KeyCode::Mouse_Button4] = GLFW_MOUSE_BUTTON_4;
		m_toGLFW[KeyCode::Mouse_Button5] = GLFW_MOUSE_BUTTON_5;
		m_toGLFW[KeyCode::Mouse_Button6] = GLFW_MOUSE_BUTTON_6;
		m_toGLFW[KeyCode::Mouse_Button7] = GLFW_MOUSE_BUTTON_7;

		// Keyboard
		m_toGLFW[KeyCode::Keyboard_A] = GLFW_KEY_A;
		m_toGLFW[KeyCode::Keyboard_B] = GLFW_KEY_B;
		m_toGLFW[KeyCode::Keyboard_C] = GLFW_KEY_C;
		m_toGLFW[KeyCode::Keyboard_D] = GLFW_KEY_D;
		m_toGLFW[KeyCode::Keyboard_E] = GLFW_KEY_E;
		m_toGLFW[KeyCode::Keyboard_F] = GLFW_KEY_F;
		m_toGLFW[KeyCode::Keyboard_G] = GLFW_KEY_G;
		m_toGLFW[KeyCode::Keyboard_H] = GLFW_KEY_H;
		m_toGLFW[KeyCode::Keyboard_I] = GLFW_KEY_I;
		m_toGLFW[KeyCode::Keyboard_J] = GLFW_KEY_J;
		m_toGLFW[KeyCode::Keyboard_K] = GLFW_KEY_K;
		m_toGLFW[KeyCode::Keyboard_L] = GLFW_KEY_L;
		m_toGLFW[KeyCode::Keyboard_M] = GLFW_KEY_M;
		m_toGLFW[KeyCode::Keyboard_N] = GLFW_KEY_N;
		m_toGLFW[KeyCode::Keyboard_O] = GLFW_KEY_O;
		m_toGLFW[KeyCode::Keyboard_P] = GLFW_KEY_P;
		m_toGLFW[KeyCode::Keyboard_Q] = GLFW_KEY_Q;
		m_toGLFW[KeyCode::Keyboard_R] = GLFW_KEY_R;
		m_toGLFW[KeyCode::Keyboard_S] = GLFW_KEY_S;
		m_toGLFW[KeyCode::Keyboard_T] = GLFW_KEY_T;
		m_toGLFW[KeyCode::Keyboard_U] = GLFW_KEY_U;
		m_toGLFW[KeyCode::Keyboard_V] = GLFW_KEY_V;
		m_toGLFW[KeyCode::Keyboard_W] = GLFW_KEY_W;
		m_toGLFW[KeyCode::Keyboard_X] = GLFW_KEY_X;
		m_toGLFW[KeyCode::Keyboard_Y] = GLFW_KEY_Y;
		m_toGLFW[KeyCode::Keyboard_Z] = GLFW_KEY_Z;
		m_toGLFW[KeyCode::Keyboard_0] = GLFW_KEY_0;
		m_toGLFW[KeyCode::Keyboard_1] = GLFW_KEY_1;
		m_toGLFW[KeyCode::Keyboard_2] = GLFW_KEY_2;
		m_toGLFW[KeyCode::Keyboard_3] = GLFW_KEY_3;
		m_toGLFW[KeyCode::Keyboard_4] = GLFW_KEY_4;
		m_toGLFW[KeyCode::Keyboard_5] = GLFW_KEY_5;
		m_toGLFW[KeyCode::Keyboard_6] = GLFW_KEY_6;
		m_toGLFW[KeyCode::Keyboard_7] = GLFW_KEY_7;
		m_toGLFW[KeyCode::Keyboard_8] = GLFW_KEY_8;
		m_toGLFW[KeyCode::Keyboard_9] = GLFW_KEY_9;
		m_toGLFW[KeyCode::Keyboard_F1] =  GLFW_KEY_F1;
		m_toGLFW[KeyCode::Keyboard_F2] =  GLFW_KEY_F2;
		m_toGLFW[KeyCode::Keyboard_F3] =  GLFW_KEY_F3;
		m_toGLFW[KeyCode::Keyboard_F4] =  GLFW_KEY_F4;
		m_toGLFW[KeyCode::Keyboard_F5] =  GLFW_KEY_F5;
		m_toGLFW[KeyCode::Keyboard_F6] =  GLFW_KEY_F6;
		m_toGLFW[KeyCode::Keyboard_F7] =  GLFW_KEY_F7;
		m_toGLFW[KeyCode::Keyboard_F8] =  GLFW_KEY_F8;
		m_toGLFW[KeyCode::Keyboard_F9] =  GLFW_KEY_F9;
		m_toGLFW[KeyCode::Keyboard_F10] = GLFW_KEY_F10;
		m_toGLFW[KeyCode::Keyboard_F11] = GLFW_KEY_F11;
		m_toGLFW[KeyCode::Keyboard_F12] = GLFW_KEY_F12;

		// Special keys
		m_toGLFW[KeyCode::Keyboard_Escape] = GLFW_KEY_ESCAPE;
		m_toGLFW[KeyCode::Keyboard_Tab] = GLFW_KEY_TAB;
		m_toGLFW[KeyCode::Keyboard_CapsLock] = GLFW_KEY_CAPS_LOCK;
		m_toGLFW[KeyCode::Keyboard_Shift_Left] = GLFW_KEY_LEFT_SHIFT;
		m_toGLFW[KeyCode::Keyboard_Shift_Right] = GLFW_KEY_RIGHT_SHIFT;
		m_toGLFW[KeyCode::Keyboard_Ctrl_Left] = GLFW_KEY_LEFT_CONTROL;
		m_toGLFW[KeyCode::Keyboard_Ctrl_Right] = GLFW_KEY_RIGHT_CONTROL;
		m_toGLFW[KeyCode::Keyboard_Alt_Left] = GLFW_KEY_LEFT_ALT;
		m_toGLFW[KeyCode::Keyboard_Alt_Right] = GLFW_KEY_RIGHT_ALT;
		m_toGLFW[KeyCode::Keyboard_Space] = GLFW_KEY_SPACE;
		m_toGLFW[KeyCode::Keyboard_Enter] = GLFW_KEY_ENTER;
		m_toGLFW[KeyCode::Keyboard_Backspace] = GLFW_KEY_BACKSPACE;
		m_toGLFW[KeyCode::Keyboard_Bracket_Left] = GLFW_KEY_LEFT_BRACKET;
		m_toGLFW[KeyCode::Keyboard_Bracket_Right] = GLFW_KEY_RIGHT_BRACKET;
		m_toGLFW[KeyCode::Keyboard_Semicolon] = GLFW_KEY_SEMICOLON;
		m_toGLFW[KeyCode::Keyboard_Quote] = GLFW_KEY_APOSTROPHE;
		m_toGLFW[KeyCode::Keyboard_Comma] = GLFW_KEY_COMMA;
		m_toGLFW[KeyCode::Keyboard_Period] = GLFW_KEY_PERIOD;
		m_toGLFW[KeyCode::Keyboard_Slash] = GLFW_KEY_SLASH;
		m_toGLFW[KeyCode::Keyboard_Backslash] = GLFW_KEY_BACKSLASH;
		m_toGLFW[KeyCode::Keyboard_Tilde] = GLFW_KEY_GRAVE_ACCENT;
		m_toGLFW[KeyCode::Keyboard_Equal] = GLFW_KEY_EQUAL;
		m_toGLFW[KeyCode::Keyboard_Hyphen] = GLFW_KEY_MINUS;

		// Function Keys
		m_toGLFW[KeyCode::Keyboard_Insert] = GLFW_KEY_INSERT;
		m_toGLFW[KeyCode::Keyboard_Delete] = GLFW_KEY_DELETE;
		m_toGLFW[KeyCode::Keyboard_Home] = GLFW_KEY_HOME;
		m_toGLFW[KeyCode::Keyboard_End] = GLFW_KEY_END;
		m_toGLFW[KeyCode::Keyboard_PageUp] = GLFW_KEY_PAGE_UP;
		m_toGLFW[KeyCode::Keyboard_PageDown] = GLFW_KEY_PAGE_DOWN;
		m_toGLFW[KeyCode::Keyboard_PrintScreen] = GLFW_KEY_PRINT_SCREEN;
		m_toGLFW[KeyCode::Keyboard_ScrollLock] = GLFW_KEY_SCROLL_LOCK;
		m_toGLFW[KeyCode::Keyboard_Pause] = GLFW_KEY_PAUSE;

		// Arrows
		m_toGLFW[KeyCode::Keyboard_Arrow_Up] = GLFW_KEY_UP;
		m_toGLFW[KeyCode::Keyboard_Arrow_Down] = GLFW_KEY_DOWN;
		m_toGLFW[KeyCode::Keyboard_Arrow_Left] = GLFW_KEY_LEFT;
		m_toGLFW[KeyCode::Keyboard_Arrow_Right] = GLFW_KEY_RIGHT;

		// Numpad
		m_toGLFW[KeyCode::Keyboard_Numpad_Lock] = GLFW_KEY_NUM_LOCK;
		m_toGLFW[KeyCode::Keyboard_Numpad_0] = GLFW_KEY_KP_0;
		m_toGLFW[KeyCode::Keyboard_Numpad_1] = GLFW_KEY_KP_1;
		m_toGLFW[KeyCode::Keyboard_Numpad_2] = GLFW_KEY_KP_2;
		m_toGLFW[KeyCode::Keyboard_Numpad_3] = GLFW_KEY_KP_3;
		m_toGLFW[KeyCode::Keyboard_Numpad_4] = GLFW_KEY_KP_4;
		m_toGLFW[KeyCode::Keyboard_Numpad_5] = GLFW_KEY_KP_5;
		m_toGLFW[KeyCode::Keyboard_Numpad_6] = GLFW_KEY_KP_6;
		m_toGLFW[KeyCode::Keyboard_Numpad_7] = GLFW_KEY_KP_7;
		m_toGLFW[KeyCode::Keyboard_Numpad_8] = GLFW_KEY_KP_8;
		m_toGLFW[KeyCode::Keyboard_Numpad_9] = GLFW_KEY_KP_9;
		m_toGLFW[KeyCode::Keyboard_Numpad_Add] = GLFW_KEY_KP_ADD;
		m_toGLFW[KeyCode::Keyboard_Numpad_Subtract] = GLFW_KEY_KP_SUBTRACT;
		m_toGLFW[KeyCode::Keyboard_Numpad_Multiply] = GLFW_KEY_KP_MULTIPLY;
		m_toGLFW[KeyCode::Keyboard_Numpad_Divide] = GLFW_KEY_KP_DIVIDE;
		m_toGLFW[KeyCode::Keyboard_Numpad_Decimal] = GLFW_KEY_KP_DECIMAL;
		m_toGLFW[KeyCode::Keyboard_Numpad_Enter] = GLFW_KEY_KP_ENTER;

		for (const auto& pair : m_toGLFW)
		{
			m_fromGLFW[pair.second] = pair.first;
		}

		return true;
	}

	uint32 InputManager_GLFW::FromKeyCode(KeyCode key) const
	{
		if (m_toGLFW.Has(key))
		{
			return m_toGLFW[key];
		}

		JPT_ERROR("Unknown key code: " + key.ToString());
		return kInvalidValue<uint32>;
	}

	KeyCode InputManager_GLFW::ToKeyCode(uint32 key) const
	{
		if (m_fromGLFW.Has(key))
		{
			return m_fromGLFW[key];
		}

		JPT_ERROR("Unknown key code: " + ToString(key));
		return KeyCode::Invalid;
	}
}