// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#if IS_PLATFORM_MACOS || IS_PLATFORM_IOS

// Textual before the header that imports. The Carbon umbrella, because
// <Carbon/HIToolbox/Events.h> does not resolve on its own; ~180 ms per TU.
// macOS only: there is no Carbon in the iOS SDK.
#if IS_PLATFORM_MACOS
    #include <Carbon/Carbon.h>
#endif

#include "AppleCallbacks.h"
#include "Applications/AppClient.h"
#include "Graphics/Renderer.h"

// An import is not re-exported, so AppClient.h's own do not cover these.
import jpt.Input;
import jpt.InputCodes;
import jpt.Vector2;

namespace jpt
{
#if IS_PLATFORM_MACOS
    namespace local
    {
        [[nodiscard]] KeyCode ToKeyCode(std::uint16_t platformKeyCode) noexcept;
        [[nodiscard]] std::uint32_t ModifierMask(KeyCode key) noexcept;
        [[nodiscard]] MouseButton ToMouseButton(std::int32_t buttonNumber) noexcept;
    }

    bool IsVSyncEnabled()
    {
        return GetApplication().GetRenderer().IsVSyncEnabled();
    }
#endif // IS_PLATFORM_MACOS

    bool OnSurfaceReady(CA::MetalLayer* pMetalLayer)
    {
        return GetApplication().OnSurfaceReady(pMetalLayer);
    }

    void OnResize(std::uint32_t pixelWidth, std::uint32_t pixelHeight)
    {
        GetApplication().OnResize(pixelWidth, pixelHeight);
    }

    void OnFrame()
    {
        GetApplication().OnFrame();
    }

    void OnTerminate()
    {
        GetApplication().Terminate();
    }

#if IS_PLATFORM_MACOS
    void OnKeyDown(std::uint16_t platformKeyCode, bool isRepeat)
    {
        GetApplication().GetInput().PostKeyDown(local::ToKeyCode(platformKeyCode), isRepeat);
    }

    void OnKeyUp(std::uint16_t platformKeyCode)
    {
        GetApplication().GetInput().PostKeyUp(local::ToKeyCode(platformKeyCode));
    }

    void OnModifierChanged(std::uint16_t platformKeyCode, std::uint32_t modifierFlags)
    {
        const KeyCode key = local::ToKeyCode(platformKeyCode);
        const std::uint32_t mask = local::ModifierMask(key);
        if (mask == 0)
        {
            return;     // fn, whose flag cannot be trusted.
        }

        Input& input = GetApplication().GetInput();
        if ((modifierFlags & mask) != 0)
        {
            input.PostKeyDown(key, false);
        }
        else
        {
            input.PostKeyUp(key);
        }
    }

    void OnMouseButton(std::int32_t buttonNumber, bool isDown, float x, float y)
    {
        GetApplication().GetInput().PostMouseButton(local::ToMouseButton(buttonNumber), isDown, Vec2(x, y));
    }

    void OnMouseMove(float x, float y)
    {
        GetApplication().GetInput().PostMouseMove(Vec2(x, y));
    }

    void OnMouseScroll(float deltaX, float deltaY, bool isPrecise)
    {
        GetApplication().GetInput().PostMouseScroll(Vec2(deltaX, deltaY), isPrecise);
    }
#endif // IS_PLATFORM_MACOS

    void OnTouchBegan(std::uint64_t touchId, float x, float y, double timeSeconds)
    {
        GetApplication().GetInput().PostTouch(TouchPhase::Began, touchId, Vec2(x, y), timeSeconds);
    }

    void OnTouchMoved(std::uint64_t touchId, float x, float y, double timeSeconds)
    {
        GetApplication().GetInput().PostTouch(TouchPhase::Moved, touchId, Vec2(x, y), timeSeconds);
    }

    void OnTouchEnded(std::uint64_t touchId, float x, float y, double timeSeconds)
    {
        GetApplication().GetInput().PostTouch(TouchPhase::Ended, touchId, Vec2(x, y), timeSeconds);
    }

    void OnTouchCancelled(std::uint64_t touchId, float x, float y, double timeSeconds)
    {
        GetApplication().GetInput().PostTouch(TouchPhase::Cancelled, touchId, Vec2(x, y), timeSeconds);
    }

#if IS_PLATFORM_MACOS
    namespace local
    {
        KeyCode ToKeyCode(std::uint16_t platformKeyCode) noexcept
        {
            switch (platformKeyCode)
            {
                case kVK_ANSI_A: return KeyCode::A;
                case kVK_ANSI_B: return KeyCode::B;
                case kVK_ANSI_C: return KeyCode::C;
                case kVK_ANSI_D: return KeyCode::D;
                case kVK_ANSI_E: return KeyCode::E;
                case kVK_ANSI_F: return KeyCode::F;
                case kVK_ANSI_G: return KeyCode::G;
                case kVK_ANSI_H: return KeyCode::H;
                case kVK_ANSI_I: return KeyCode::I;
                case kVK_ANSI_J: return KeyCode::J;
                case kVK_ANSI_K: return KeyCode::K;
                case kVK_ANSI_L: return KeyCode::L;
                case kVK_ANSI_M: return KeyCode::M;
                case kVK_ANSI_N: return KeyCode::N;
                case kVK_ANSI_O: return KeyCode::O;
                case kVK_ANSI_P: return KeyCode::P;
                case kVK_ANSI_Q: return KeyCode::Q;
                case kVK_ANSI_R: return KeyCode::R;
                case kVK_ANSI_S: return KeyCode::S;
                case kVK_ANSI_T: return KeyCode::T;
                case kVK_ANSI_U: return KeyCode::U;
                case kVK_ANSI_V: return KeyCode::V;
                case kVK_ANSI_W: return KeyCode::W;
                case kVK_ANSI_X: return KeyCode::X;
                case kVK_ANSI_Y: return KeyCode::Y;
                case kVK_ANSI_Z: return KeyCode::Z;

                case kVK_ANSI_0: return KeyCode::Num0;
                case kVK_ANSI_1: return KeyCode::Num1;
                case kVK_ANSI_2: return KeyCode::Num2;
                case kVK_ANSI_3: return KeyCode::Num3;
                case kVK_ANSI_4: return KeyCode::Num4;
                case kVK_ANSI_5: return KeyCode::Num5;
                case kVK_ANSI_6: return KeyCode::Num6;
                case kVK_ANSI_7: return KeyCode::Num7;
                case kVK_ANSI_8: return KeyCode::Num8;
                case kVK_ANSI_9: return KeyCode::Num9;

                case kVK_F1:  return KeyCode::F1;
                case kVK_F2:  return KeyCode::F2;
                case kVK_F3:  return KeyCode::F3;
                case kVK_F4:  return KeyCode::F4;
                case kVK_F5:  return KeyCode::F5;
                case kVK_F6:  return KeyCode::F6;
                case kVK_F7:  return KeyCode::F7;
                case kVK_F8:  return KeyCode::F8;
                case kVK_F9:  return KeyCode::F9;
                case kVK_F10: return KeyCode::F10;
                case kVK_F11: return KeyCode::F11;
                case kVK_F12: return KeyCode::F12;
                case kVK_F13: return KeyCode::F13;
                case kVK_F14: return KeyCode::F14;
                case kVK_F15: return KeyCode::F15;
                case kVK_F16: return KeyCode::F16;
                case kVK_F17: return KeyCode::F17;
                case kVK_F18: return KeyCode::F18;
                case kVK_F19: return KeyCode::F19;
                case kVK_F20: return KeyCode::F20;

                case kVK_LeftArrow:  return KeyCode::LeftArrow;
                case kVK_RightArrow: return KeyCode::RightArrow;
                case kVK_UpArrow:    return KeyCode::UpArrow;
                case kVK_DownArrow:  return KeyCode::DownArrow;

                case kVK_Space:         return KeyCode::Space;
                case kVK_Return:        return KeyCode::Enter;
                case kVK_Escape:        return KeyCode::Escape;
                case kVK_Tab:           return KeyCode::Tab;
                case kVK_Delete:        return KeyCode::Backspace;      // Backspace, despite the name.
                case kVK_ForwardDelete: return KeyCode::Delete;
                case kVK_Home:          return KeyCode::Home;
                case kVK_End:           return KeyCode::End;
                case kVK_PageUp:        return KeyCode::PageUp;
                case kVK_PageDown:      return KeyCode::PageDown;

                case kVK_CapsLock: return KeyCode::CapsLock;
                case kVK_Function: return KeyCode::Function;

                case kVK_Shift:         return KeyCode::LeftShift;
                case kVK_RightShift:    return KeyCode::RightShift;
                case kVK_Control:       return KeyCode::LeftControl;
                case kVK_RightControl:  return KeyCode::RightControl;
                case kVK_Option:        return KeyCode::LeftAlt;
                case kVK_RightOption:   return KeyCode::RightAlt;
                case kVK_Command:       return KeyCode::LeftSuper;
                case kVK_RightCommand:  return KeyCode::RightSuper;

                case kVK_ANSI_Minus:        return KeyCode::Minus;
                case kVK_ANSI_Equal:        return KeyCode::Equal;
                case kVK_ANSI_LeftBracket:  return KeyCode::LeftBracket;
                case kVK_ANSI_RightBracket: return KeyCode::RightBracket;
                case kVK_ANSI_Backslash:    return KeyCode::Backslash;
                case kVK_ANSI_Semicolon:    return KeyCode::Semicolon;
                case kVK_ANSI_Quote:        return KeyCode::Quote;
                case kVK_ANSI_Grave:        return KeyCode::Grave;
                case kVK_ANSI_Comma:        return KeyCode::Comma;
                case kVK_ANSI_Period:       return KeyCode::Period;
                case kVK_ANSI_Slash:        return KeyCode::Slash;

                case kVK_ANSI_Keypad0:        return KeyCode::Keypad0;
                case kVK_ANSI_Keypad1:        return KeyCode::Keypad1;
                case kVK_ANSI_Keypad2:        return KeyCode::Keypad2;
                case kVK_ANSI_Keypad3:        return KeyCode::Keypad3;
                case kVK_ANSI_Keypad4:        return KeyCode::Keypad4;
                case kVK_ANSI_Keypad5:        return KeyCode::Keypad5;
                case kVK_ANSI_Keypad6:        return KeyCode::Keypad6;
                case kVK_ANSI_Keypad7:        return KeyCode::Keypad7;
                case kVK_ANSI_Keypad8:        return KeyCode::Keypad8;
                case kVK_ANSI_Keypad9:        return KeyCode::Keypad9;
                case kVK_ANSI_KeypadDecimal:  return KeyCode::KeypadDecimal;
                case kVK_ANSI_KeypadPlus:     return KeyCode::KeypadPlus;
                case kVK_ANSI_KeypadMinus:    return KeyCode::KeypadMinus;
                case kVK_ANSI_KeypadMultiply: return KeyCode::KeypadMultiply;
                case kVK_ANSI_KeypadDivide:   return KeyCode::KeypadDivide;
                case kVK_ANSI_KeypadEnter:    return KeyCode::KeypadEnter;
                case kVK_ANSI_KeypadEquals:   return KeyCode::KeypadEquals;
                case kVK_ANSI_KeypadClear:    return KeyCode::KeypadClear;

                default: return KeyCode::Unknown;
            }
        }

        std::uint32_t ModifierMask(KeyCode key) noexcept
        {
            switch (key)
            {
                case KeyCode::LeftControl:  return 0x0001;
                case KeyCode::RightControl: return 0x2000;
                case KeyCode::LeftShift:    return 0x0002;
                case KeyCode::RightShift:   return 0x0004;
                case KeyCode::LeftSuper:    return 0x0008;
                case KeyCode::RightSuper:   return 0x0010;
                case KeyCode::LeftAlt:      return 0x0020;
                case KeyCode::RightAlt:     return 0x0040;

                // Lock state, not held: down/up here mean engaged and disengaged.
                case KeyCode::CapsLock:     return 1u << 16;    // NSEventModifierFlagCapsLock
                default: return 0;
            }
        }

        MouseButton ToMouseButton(std::int32_t buttonNumber) noexcept
        {
            switch (buttonNumber)
            {
                case 0:  return MouseButton::Left;
                case 1:  return MouseButton::Right;
                default: return MouseButton::Middle;
            }
        }
    }
#endif // IS_PLATFORM_MACOS
}

#endif // IS_PLATFORM_MACOS || IS_PLATFORM_IOS
