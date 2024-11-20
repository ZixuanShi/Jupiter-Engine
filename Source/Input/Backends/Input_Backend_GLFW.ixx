// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Logger.h"

#include <GLFW/glfw3.h>

export module jpt.Input.Backend_GLFW;

import jpt.Input.Backend;
import jpt.Input.KeyCode;

import jpt.TypeDefs;
import jpt.Constants;
import jpt.HashMap;
import jpt.ToString;

export namespace jpt::Input
{
	class Backend_GLFW final : public Backend
	{
	private:
		HashMap<KeyCode, uint32> m_toGLFW;
		HashMap<uint32, KeyCode> m_fromGLFW;

	public:
		Backend_GLFW();

	public:
		virtual bool IsPressed(KeyCode key) const override;
		virtual bool IsReleased(KeyCode key) const override;

		virtual uint32 FromKeyCode(KeyCode key) const override;
		virtual KeyCode ToKeyCode(uint32 key) const override;
	};

	Backend_GLFW::Backend_GLFW()
	{
		m_toGLFW =
		{
			// Mouse
			{ KeyCode::Mouse_Left,    GLFW_MOUSE_BUTTON_LEFT              },
			{ KeyCode::Mouse_Right,   GLFW_MOUSE_BUTTON_RIGHT             },
			{ KeyCode::Mouse_Wheel,   GLFW_MOUSE_BUTTON_MIDDLE            },
			{ KeyCode::Mouse_Button4, GLFW_MOUSE_BUTTON_4                 },
			{ KeyCode::Mouse_Button5, GLFW_MOUSE_BUTTON_5                 },
			{ KeyCode::Mouse_Button6, GLFW_MOUSE_BUTTON_6                 },
			{ KeyCode::Mouse_Button7, GLFW_MOUSE_BUTTON_7                 },

			// Keyboard
			{ KeyCode::Keyboard_A,    GLFW_KEY_A                          },
			{ KeyCode::Keyboard_B,    GLFW_KEY_B                          },
			{ KeyCode::Keyboard_C,    GLFW_KEY_C                          },
			{ KeyCode::Keyboard_D,    GLFW_KEY_D                          },
			{ KeyCode::Keyboard_E,    GLFW_KEY_E                          },
			{ KeyCode::Keyboard_F,    GLFW_KEY_F                          },
			{ KeyCode::Keyboard_G,    GLFW_KEY_G                          },
			{ KeyCode::Keyboard_H,    GLFW_KEY_H                          },
			{ KeyCode::Keyboard_I,    GLFW_KEY_I                          },
			{ KeyCode::Keyboard_J,    GLFW_KEY_J                          },
			{ KeyCode::Keyboard_K,    GLFW_KEY_K                          },
			{ KeyCode::Keyboard_L,    GLFW_KEY_L                          },
			{ KeyCode::Keyboard_M,    GLFW_KEY_M                          },
			{ KeyCode::Keyboard_N,    GLFW_KEY_N                          },
			{ KeyCode::Keyboard_O,    GLFW_KEY_O                          },
			{ KeyCode::Keyboard_P,    GLFW_KEY_P                          },
			{ KeyCode::Keyboard_Q,    GLFW_KEY_Q                          },
			{ KeyCode::Keyboard_R,    GLFW_KEY_R                          },
			{ KeyCode::Keyboard_S,    GLFW_KEY_S                          },
			{ KeyCode::Keyboard_T,    GLFW_KEY_T                          },
			{ KeyCode::Keyboard_U,    GLFW_KEY_U                          },
			{ KeyCode::Keyboard_V,    GLFW_KEY_V                          },
			{ KeyCode::Keyboard_W,    GLFW_KEY_W                          },
			{ KeyCode::Keyboard_X,    GLFW_KEY_X                          },
			{ KeyCode::Keyboard_Y,    GLFW_KEY_Y                          },
			{ KeyCode::Keyboard_Z,    GLFW_KEY_Z                          },
			{ KeyCode::Keyboard_0,    GLFW_KEY_0                          },
			{ KeyCode::Keyboard_1,    GLFW_KEY_1                          },
			{ KeyCode::Keyboard_2,    GLFW_KEY_2                          },
			{ KeyCode::Keyboard_3,    GLFW_KEY_3                          },
			{ KeyCode::Keyboard_4,    GLFW_KEY_4                          },
			{ KeyCode::Keyboard_5,    GLFW_KEY_5                          },
			{ KeyCode::Keyboard_6,    GLFW_KEY_6                          },
			{ KeyCode::Keyboard_7,    GLFW_KEY_7                          },
			{ KeyCode::Keyboard_8,    GLFW_KEY_8                          },
			{ KeyCode::Keyboard_9,    GLFW_KEY_9                          },
			{ KeyCode::Keyboard_F1,   GLFW_KEY_F1                         },
			{ KeyCode::Keyboard_F2,   GLFW_KEY_F2                         },
			{ KeyCode::Keyboard_F3,   GLFW_KEY_F3                         },
			{ KeyCode::Keyboard_F4,   GLFW_KEY_F4                         },
			{ KeyCode::Keyboard_F5,   GLFW_KEY_F5                         },
			{ KeyCode::Keyboard_F6,   GLFW_KEY_F6                         },
			{ KeyCode::Keyboard_F7,   GLFW_KEY_F7                         },
			{ KeyCode::Keyboard_F8,   GLFW_KEY_F8                         },
			{ KeyCode::Keyboard_F9,   GLFW_KEY_F9                         },
			{ KeyCode::Keyboard_F10,  GLFW_KEY_F10                        },
			{ KeyCode::Keyboard_F11,  GLFW_KEY_F11                        },
			{ KeyCode::Keyboard_F12,  GLFW_KEY_F12                        },

			// Special keys
			{ KeyCode::Keyboard_Escape,        GLFW_KEY_ESCAPE            },
			{ KeyCode::Keyboard_Tab,           GLFW_KEY_TAB               },
			{ KeyCode::Keyboard_CapsLock,      GLFW_KEY_CAPS_LOCK         },
			{ KeyCode::Keyboard_Shift_Left,    GLFW_KEY_LEFT_SHIFT        },
			{ KeyCode::Keyboard_Shift_Right,   GLFW_KEY_RIGHT_SHIFT       },
			{ KeyCode::Keyboard_Ctrl_Left,     GLFW_KEY_LEFT_CONTROL      },
			{ KeyCode::Keyboard_Ctrl_Right,    GLFW_KEY_RIGHT_CONTROL     },
			{ KeyCode::Keyboard_Alt_Left,      GLFW_KEY_LEFT_ALT          },
			{ KeyCode::Keyboard_Alt_Right,     GLFW_KEY_RIGHT_ALT         },
			{ KeyCode::Keyboard_Space,         GLFW_KEY_SPACE             },
			{ KeyCode::Keyboard_Enter,         GLFW_KEY_ENTER             },
			{ KeyCode::Keyboard_Backspace,     GLFW_KEY_BACKSPACE         },
			{ KeyCode::Keyboard_Bracket_Left,  GLFW_KEY_LEFT_BRACKET      },
			{ KeyCode::Keyboard_Bracket_Right, GLFW_KEY_RIGHT_BRACKET     },
			{ KeyCode::Keyboard_Semicolon,     GLFW_KEY_SEMICOLON         },
			{ KeyCode::Keyboard_Quote,         GLFW_KEY_APOSTROPHE        },
			{ KeyCode::Keyboard_Comma,         GLFW_KEY_COMMA             },
			{ KeyCode::Keyboard_Period,        GLFW_KEY_PERIOD            },
			{ KeyCode::Keyboard_Slash,         GLFW_KEY_SLASH             },
			{ KeyCode::Keyboard_Backslash,     GLFW_KEY_BACKSLASH         },
			{ KeyCode::Keyboard_Tilde,         GLFW_KEY_GRAVE_ACCENT      },
			{ KeyCode::Keyboard_Equal,         GLFW_KEY_EQUAL             },
			{ KeyCode::Keyboard_Hyphen,        GLFW_KEY_MINUS             }, 

			// Function Keys
			{ KeyCode::Keyboard_Insert,        GLFW_KEY_INSERT            },
			{ KeyCode::Keyboard_Delete,        GLFW_KEY_DELETE            },
			{ KeyCode::Keyboard_Home,          GLFW_KEY_HOME              },
			{ KeyCode::Keyboard_End,           GLFW_KEY_END               },
			{ KeyCode::Keyboard_PageUp,        GLFW_KEY_PAGE_UP           },
			{ KeyCode::Keyboard_PageDown,      GLFW_KEY_PAGE_DOWN         },
			{ KeyCode::Keyboard_PrintScreen,   GLFW_KEY_PRINT_SCREEN      },
			{ KeyCode::Keyboard_ScrollLock,    GLFW_KEY_SCROLL_LOCK       },
			{ KeyCode::Keyboard_Pause,         GLFW_KEY_PAUSE             },

			// Arrows
			{ KeyCode::Keyboard_Arrow_Up,      GLFW_KEY_UP                },
			{ KeyCode::Keyboard_Arrow_Down,    GLFW_KEY_DOWN              },
			{ KeyCode::Keyboard_Arrow_Left,    GLFW_KEY_LEFT              },
			{ KeyCode::Keyboard_Arrow_Right,   GLFW_KEY_RIGHT             },

			// Numpad
			{ KeyCode::Keyboard_Numpad_Lock,     GLFW_KEY_NUM_LOCK        },
			{ KeyCode::Keyboard_Numpad_0,        GLFW_KEY_KP_0            },
			{ KeyCode::Keyboard_Numpad_1,        GLFW_KEY_KP_1            },
			{ KeyCode::Keyboard_Numpad_2,        GLFW_KEY_KP_2            },
			{ KeyCode::Keyboard_Numpad_3,        GLFW_KEY_KP_3            },
			{ KeyCode::Keyboard_Numpad_4,        GLFW_KEY_KP_4            },
			{ KeyCode::Keyboard_Numpad_5,        GLFW_KEY_KP_5            },
			{ KeyCode::Keyboard_Numpad_6,        GLFW_KEY_KP_6            },
			{ KeyCode::Keyboard_Numpad_7,        GLFW_KEY_KP_7            },
			{ KeyCode::Keyboard_Numpad_8,        GLFW_KEY_KP_8            },
			{ KeyCode::Keyboard_Numpad_9,        GLFW_KEY_KP_9            },
			{ KeyCode::Keyboard_Numpad_Add,      GLFW_KEY_KP_ADD          },
			{ KeyCode::Keyboard_Numpad_Subtract, GLFW_KEY_KP_SUBTRACT     },
			{ KeyCode::Keyboard_Numpad_Multiply, GLFW_KEY_KP_MULTIPLY     },
			{ KeyCode::Keyboard_Numpad_Divide,   GLFW_KEY_KP_DIVIDE       },
			{ KeyCode::Keyboard_Numpad_Decimal,  GLFW_KEY_KP_DECIMAL      },
			{ KeyCode::Keyboard_Numpad_Enter,    GLFW_KEY_KP_ENTER        },
		};

		for (const auto& pair : m_toGLFW)
		{
			m_fromGLFW[pair.second] = pair.first;
		}
	}

	bool Backend_GLFW::IsPressed(KeyCode key) const
	{
		const uint32 glfwKey = FromKeyCode(key);
		if (glfwKey == kInvalidValue<uint32>)
		{
			JPT_ERROR("Unknown key code: " + key.ToString());
			return false;
		}

		GLFWwindow* pGLFWWindow = glfwGetCurrentContext();
		if (!pGLFWWindow)
		{
			return false;
		}

		return glfwGetKey(pGLFWWindow, glfwKey) == GLFW_PRESS;
	}

	bool Backend_GLFW::IsReleased(KeyCode key) const
	{
		const uint32 glfwKey = FromKeyCode(key);
		if (glfwKey == kInvalidValue<uint32>)
		{
			JPT_ERROR("Unknown key code: " + key.ToString());
			return false;
		}

		GLFWwindow* pGLFWWindow = glfwGetCurrentContext();
		if (!pGLFWWindow)
		{
			return false;
		}

		return glfwGetKey(pGLFWWindow, glfwKey) == GLFW_RELEASE;
	}

	uint32 Backend_GLFW::FromKeyCode(KeyCode key) const
	{
		if (m_toGLFW.Has(key))
		{
			return m_toGLFW[key];
		}

		JPT_ERROR("Unknown key code: " + key.ToString());
		return kInvalidValue<uint32>;
	}

	KeyCode Backend_GLFW::ToKeyCode(uint32 key) const
	{
		if (m_fromGLFW.Has(key))
		{
			return m_fromGLFW[key];
		}

		JPT_ERROR("Unknown key code: " + ToString(key));
		return KeyCode::Invalid;
	}
}