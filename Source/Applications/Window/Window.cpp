// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <SDL3/SDL.h>

#include "Applications/GetApp.h"
#include "Graphics/ImGui/ImGuiLayer.h"

module jpt.Window;

import jpt.Assert;
import jpt.TypeDefs;
import jpt.Input;
import jpt.InputCodes;
import jpt.Logger;
import jpt.Time;
import jpt.Vector2;

namespace jpt
{
    namespace local
    {
        [[nodiscard]] KeyCode ToKeyCode(SDL_Scancode scancode) noexcept;
        [[nodiscard]] MouseButton ToMouseButton(uint8 button) noexcept;
        [[nodiscard]] TouchPhase ToTouchPhase(uint32 eventType) noexcept;
        [[nodiscard]] TouchDevice ToTouchDevice(SDL_TouchDeviceType type) noexcept;
    }

    bool Window::PreInit()
    {
        // Without this macOS delivers no fingers at all: Cocoa reports a trackpad under
        // SDL_MOUSE_TOUCHID, and SDL_SendTouch discards that id unless MOUSE_TOUCH_EVENTS is set.
        // Setting MOUSE_TOUCH_EVENTS instead would register it as a *direct* device and take the
        // wrong coordinate branch. Read in Cocoa_VideoInit, so it must precede SDL_Init.
        SDL_SetHint(SDL_HINT_TRACKPAD_IS_TOUCH_ONLY, "1");

        // Events come with video. Audio and sensor would cost startup time and, on iOS, prompt
        // for permissions nothing here has asked for.
        if (!SDL_Init(SDL_INIT_VIDEO))
        {
            Debug::Error("SDL_Init failed: {}", SDL_GetError());
            return false;
        }

        Debug::Info("SDL {}.{}.{}", SDL_VERSIONNUM_MAJOR(SDL_GetVersion()), SDL_VERSIONNUM_MINOR(SDL_GetVersion()), SDL_VERSIONNUM_MICRO(SDL_GetVersion()));
        return true;
    }

    bool Window::Init()
    {
        // Not optional: without it SDL hands back a 1x backing store and upscales.
        SDL_WindowFlags flags = SDL_WINDOW_HIGH_PIXEL_DENSITY;
#if IS_PLATFORM_MACOS || IS_PLATFORM_IOS
        flags |= SDL_WINDOW_METAL;
#endif
#if IS_MOBILE
        flags |= SDL_WINDOW_FULLSCREEN;
#else
        flags |= SDL_WINDOW_RESIZABLE;
#endif

        m_pWindow = SDL_CreateWindow(GetAppName(), 1920, 1080, flags);
        if (m_pWindow == nullptr)
        {
            Debug::Error("SDL_CreateWindow failed: {}", SDL_GetError());
            return false;
        }

#if IS_PLATFORM_MACOS || IS_PLATFORM_IOS
        m_metalView = SDL_Metal_CreateView(m_pWindow);
        if (m_metalView == nullptr)
        {
            Debug::Error("SDL_Metal_CreateView failed: {}", SDL_GetError());
            return false;
        }

        // metal-cpp is a typed view over the same object, so the handle crosses as a cast.
        m_surface = static_cast<SurfaceHandle>(SDL_Metal_GetLayer(m_metalView));
#endif

        m_pixelDensity = SDL_GetWindowPixelDensity(m_pWindow);

        // Two quantities, not one: the window is points, the backing store is pixels.
        Vec2i pointSize;
        SDL_GetWindowSize(m_pWindow, &pointSize.x, &pointSize.y);

        Vec2i pixelSize;
        SDL_GetWindowSizeInPixels(m_pWindow, &pixelSize.x, &pixelSize.y);

        // No resize event fires for the initial size, and GetAspectRatio() would spend the first
        // frames on its 1.0 fallback.
        OnResize(static_cast<uint32>(pixelSize.x), static_cast<uint32>(pixelSize.y));

        // The contract every mouse and touch coordinate rests on.
        Debug::Assert(pixelSize.x == static_cast<int32>(pointSize.x * m_pixelDensity + 0.5f),
                      "Pixel density {} does not reconcile {} pt with {} px", m_pixelDensity, pointSize.x, pixelSize.x);

        return true;
    }

    void Window::Terminate()
    {
#if IS_PLATFORM_MACOS || IS_PLATFORM_IOS
        if (m_metalView != nullptr)
        {
            SDL_Metal_DestroyView(m_metalView);
            m_metalView = nullptr;
        }
#endif
        m_surface = nullptr;

        if (m_pWindow != nullptr)
        {
            SDL_DestroyWindow(m_pWindow);
            m_pWindow = nullptr;
        }

        SDL_Quit();
    }

    Status Window::OnEvent(const SDL_Event& event)
    {
        // Unfiltered: acting on ProcessEvent's return would drop the release half of a press
        // that began outside a panel. Input::Update() is what gates the engine.
        ImGuiProcessEvent(&event);

        ApplicationBase& app = GetApp();
        Input& input = app.GetInput();

        switch (event.type)
        {
            case SDL_EVENT_QUIT:
            case SDL_EVENT_TERMINATING:
            case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
                return Status::Succeeded;

            // WILL, not DID: presenting a drawable while backgrounded is what the GPU watchdog
            // kills for, and the OS may give no processing time after DID.
            case SDL_EVENT_WILL_ENTER_BACKGROUND:
                app.SetPaused(true);
                break;

            case SDL_EVENT_WILL_ENTER_FOREGROUND:
                app.SetPaused(false);
                break;

            case SDL_EVENT_LOW_MEMORY:
                Debug::Warn("The OS is asking for memory back.");
                break;

            // Both, not RESIZED: a window can change backing scale without changing size.
            case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
            case SDL_EVENT_WINDOW_DISPLAY_SCALE_CHANGED:
            {
                m_pixelDensity = SDL_GetWindowPixelDensity(m_pWindow);

                Vec2i pixelSize;
                SDL_GetWindowSizeInPixels(m_pWindow, &pixelSize.x, &pixelSize.y);
                app.OnResize(static_cast<uint32>(pixelSize.x), static_cast<uint32>(pixelSize.y));
            } break;

#if IS_PC
            // A drag that leaves with the pointer hidden would strand it.
            case SDL_EVENT_WINDOW_FOCUS_LOST:
                SetCursorCaptured(false);
                break;
#endif

            case SDL_EVENT_KEY_DOWN:
                input.PostKeyDown(local::ToKeyCode(event.key.scancode), event.key.repeat);
                break;

            case SDL_EVENT_KEY_UP:
                input.PostKeyUp(local::ToKeyCode(event.key.scancode));
                break;

            case SDL_EVENT_MOUSE_BUTTON_DOWN:
            case SDL_EVENT_MOUSE_BUTTON_UP:
            {
#if IS_MOBILE
                // iOS synthesizes a mouse from every finger, which ImGui needs. Only the engine's
                // path skips it, or one touch would drive both.
                if (event.button.which == SDL_TOUCH_MOUSEID)
                {
                    break;
                }
#endif
                input.PostMouseButton(local::ToMouseButton(event.button.button),
                                      event.type == SDL_EVENT_MOUSE_BUTTON_DOWN,
                                      Vec2(event.button.x * m_pixelDensity,
                                           event.button.y * m_pixelDensity));
            } break;

            case SDL_EVENT_MOUSE_MOTION:
            {
#if IS_MOBILE
                if (event.motion.which == SDL_TOUCH_MOUSEID)
                {
                    break;
                }
#endif
                if (m_cursorCaptured)
                {
                    m_reportedX += event.motion.xrel * m_pixelDensity;
                    m_reportedY += event.motion.yrel * m_pixelDensity;
                }
                else
                {
                    m_reportedX = event.motion.x * m_pixelDensity;
                    m_reportedY = event.motion.y * m_pixelDensity;
                }

                input.PostMouseMove(Vec2(m_reportedX, m_reportedY));
            } break;

            case SDL_EVENT_MOUSE_WHEEL:
            {
                // SDL has no hasPreciseScrollingDeltas, but Cocoa_HandleMouseWheel ceils a notch
                // to a whole number and passes a trackpad's delta through raw.
                const bool isPrecise = (event.wheel.x != SDL_truncf(event.wheel.x))
                                    || (event.wheel.y != SDL_truncf(event.wheel.y));

                // direction is not un-flipped: the value already accounts for natural scrolling.
                input.PostMouseScroll(Vec2(event.wheel.x, event.wheel.y), isPrecise);
            } break;

            case SDL_EVENT_FINGER_DOWN:
            case SDL_EVENT_FINGER_MOTION:
            case SDL_EVENT_FINGER_UP:
            case SDL_EVENT_FINGER_CANCELED:
            {
                const TouchDevice device = local::ToTouchDevice(SDL_GetTouchDeviceType(event.tfinger.touchID));
                const Vec2 position = ToTouchPixels(event.tfinger.x, event.tfinger.y, device, m_width, m_height);

                input.PostTouch(local::ToTouchPhase(event.type), event.tfinger.fingerID, position,
                                NanoSecsToSeconds(event.tfinger.timestamp), device);
            } break;

            default:
                break;
        }

        return Status::Running;
    }

    void Window::OnResize(uint32 pixelWidth, uint32 pixelHeight) noexcept
    {
        m_width  = pixelWidth;
        m_height = pixelHeight;
    }

    void Window::SetCursorCaptured(bool captured)
    {
#if IS_PC
        if (captured == m_cursorCaptured)
        {
            return;
        }
        m_cursorCaptured = captured;

        if (captured)
        {
            m_anchorX = m_reportedX;
            m_anchorY = m_reportedY;
            SDL_SetWindowRelativeMouseMode(m_pWindow, true);
        }
        else
        {
            // Warp before leaving relative mode -- SDL's documented order. m_reported is already
            // the anchor, so the motion event this generates differences to zero.
            SDL_WarpMouseInWindow(m_pWindow, m_anchorX / m_pixelDensity, m_anchorY / m_pixelDensity);
            SDL_SetWindowRelativeMouseMode(m_pWindow, false);
            m_reportedX = m_anchorX;
            m_reportedY = m_anchorY;
        }
#else
        (void)captured;
#endif
    }

    float32 Window::GetAspectRatio() const noexcept
    {
        return (m_height > 0) ? static_cast<float32>(m_width) / static_cast<float32>(m_height) : 1.0f;
    }

    namespace local
    {
        KeyCode ToKeyCode(SDL_Scancode scancode) noexcept
        {
            // Scancode, not keycode: event.key.key would move W on an AZERTY keyboard.
            switch (scancode)
            {
                case SDL_SCANCODE_A: return KeyCode::A;
                case SDL_SCANCODE_B: return KeyCode::B;
                case SDL_SCANCODE_C: return KeyCode::C;
                case SDL_SCANCODE_D: return KeyCode::D;
                case SDL_SCANCODE_E: return KeyCode::E;
                case SDL_SCANCODE_F: return KeyCode::F;
                case SDL_SCANCODE_G: return KeyCode::G;
                case SDL_SCANCODE_H: return KeyCode::H;
                case SDL_SCANCODE_I: return KeyCode::I;
                case SDL_SCANCODE_J: return KeyCode::J;
                case SDL_SCANCODE_K: return KeyCode::K;
                case SDL_SCANCODE_L: return KeyCode::L;
                case SDL_SCANCODE_M: return KeyCode::M;
                case SDL_SCANCODE_N: return KeyCode::N;
                case SDL_SCANCODE_O: return KeyCode::O;
                case SDL_SCANCODE_P: return KeyCode::P;
                case SDL_SCANCODE_Q: return KeyCode::Q;
                case SDL_SCANCODE_R: return KeyCode::R;
                case SDL_SCANCODE_S: return KeyCode::S;
                case SDL_SCANCODE_T: return KeyCode::T;
                case SDL_SCANCODE_U: return KeyCode::U;
                case SDL_SCANCODE_V: return KeyCode::V;
                case SDL_SCANCODE_W: return KeyCode::W;
                case SDL_SCANCODE_X: return KeyCode::X;
                case SDL_SCANCODE_Y: return KeyCode::Y;
                case SDL_SCANCODE_Z: return KeyCode::Z;

                case SDL_SCANCODE_0: return KeyCode::Num0;
                case SDL_SCANCODE_1: return KeyCode::Num1;
                case SDL_SCANCODE_2: return KeyCode::Num2;
                case SDL_SCANCODE_3: return KeyCode::Num3;
                case SDL_SCANCODE_4: return KeyCode::Num4;
                case SDL_SCANCODE_5: return KeyCode::Num5;
                case SDL_SCANCODE_6: return KeyCode::Num6;
                case SDL_SCANCODE_7: return KeyCode::Num7;
                case SDL_SCANCODE_8: return KeyCode::Num8;
                case SDL_SCANCODE_9: return KeyCode::Num9;

                case SDL_SCANCODE_F1:  return KeyCode::F1;
                case SDL_SCANCODE_F2:  return KeyCode::F2;
                case SDL_SCANCODE_F3:  return KeyCode::F3;
                case SDL_SCANCODE_F4:  return KeyCode::F4;
                case SDL_SCANCODE_F5:  return KeyCode::F5;
                case SDL_SCANCODE_F6:  return KeyCode::F6;
                case SDL_SCANCODE_F7:  return KeyCode::F7;
                case SDL_SCANCODE_F8:  return KeyCode::F8;
                case SDL_SCANCODE_F9:  return KeyCode::F9;
                case SDL_SCANCODE_F10: return KeyCode::F10;
                case SDL_SCANCODE_F11: return KeyCode::F11;
                case SDL_SCANCODE_F12: return KeyCode::F12;
                case SDL_SCANCODE_F13: return KeyCode::F13;
                case SDL_SCANCODE_F14: return KeyCode::F14;
                case SDL_SCANCODE_F15: return KeyCode::F15;
                case SDL_SCANCODE_F16: return KeyCode::F16;
                case SDL_SCANCODE_F17: return KeyCode::F17;
                case SDL_SCANCODE_F18: return KeyCode::F18;
                case SDL_SCANCODE_F19: return KeyCode::F19;
                case SDL_SCANCODE_F20: return KeyCode::F20;

                case SDL_SCANCODE_LEFT:  return KeyCode::LeftArrow;
                case SDL_SCANCODE_RIGHT: return KeyCode::RightArrow;
                case SDL_SCANCODE_UP:    return KeyCode::UpArrow;
                case SDL_SCANCODE_DOWN:  return KeyCode::DownArrow;

                case SDL_SCANCODE_SPACE:     return KeyCode::Space;
                case SDL_SCANCODE_RETURN:    return KeyCode::Enter;
                case SDL_SCANCODE_ESCAPE:    return KeyCode::Escape;
                case SDL_SCANCODE_TAB:       return KeyCode::Tab;
                case SDL_SCANCODE_BACKSPACE: return KeyCode::Backspace;
                case SDL_SCANCODE_DELETE:    return KeyCode::Delete;
                case SDL_SCANCODE_INSERT:    return KeyCode::Insert;
                case SDL_SCANCODE_HOME:      return KeyCode::Home;
                case SDL_SCANCODE_END:       return KeyCode::End;
                case SDL_SCANCODE_PAGEUP:    return KeyCode::PageUp;
                case SDL_SCANCODE_PAGEDOWN:  return KeyCode::PageDown;

                case SDL_SCANCODE_CAPSLOCK:    return KeyCode::CapsLock;
                case SDL_SCANCODE_NUMLOCKCLEAR:return KeyCode::NumLock;
                case SDL_SCANCODE_SCROLLLOCK:  return KeyCode::ScrollLock;
                case SDL_SCANCODE_PRINTSCREEN: return KeyCode::PrintScreen;
                case SDL_SCANCODE_PAUSE:       return KeyCode::Pause;

                case SDL_SCANCODE_LSHIFT: return KeyCode::LeftShift;
                case SDL_SCANCODE_RSHIFT: return KeyCode::RightShift;
                case SDL_SCANCODE_LCTRL:  return KeyCode::LeftControl;
                case SDL_SCANCODE_RCTRL:  return KeyCode::RightControl;
                case SDL_SCANCODE_LALT:   return KeyCode::LeftAlt;
                case SDL_SCANCODE_RALT:   return KeyCode::RightAlt;
                case SDL_SCANCODE_LGUI:   return KeyCode::LeftSuper;
                case SDL_SCANCODE_RGUI:   return KeyCode::RightSuper;

                case SDL_SCANCODE_MINUS:        return KeyCode::Minus;
                case SDL_SCANCODE_EQUALS:       return KeyCode::Equal;
                case SDL_SCANCODE_LEFTBRACKET:  return KeyCode::LeftBracket;
                case SDL_SCANCODE_RIGHTBRACKET: return KeyCode::RightBracket;
                case SDL_SCANCODE_BACKSLASH:    return KeyCode::Backslash;
                case SDL_SCANCODE_SEMICOLON:    return KeyCode::Semicolon;
                case SDL_SCANCODE_APOSTROPHE:   return KeyCode::Quote;
                case SDL_SCANCODE_GRAVE:        return KeyCode::Grave;
                case SDL_SCANCODE_COMMA:        return KeyCode::Comma;
                case SDL_SCANCODE_PERIOD:       return KeyCode::Period;
                case SDL_SCANCODE_SLASH:        return KeyCode::Slash;

                case SDL_SCANCODE_KP_0:        return KeyCode::Keypad0;
                case SDL_SCANCODE_KP_1:        return KeyCode::Keypad1;
                case SDL_SCANCODE_KP_2:        return KeyCode::Keypad2;
                case SDL_SCANCODE_KP_3:        return KeyCode::Keypad3;
                case SDL_SCANCODE_KP_4:        return KeyCode::Keypad4;
                case SDL_SCANCODE_KP_5:        return KeyCode::Keypad5;
                case SDL_SCANCODE_KP_6:        return KeyCode::Keypad6;
                case SDL_SCANCODE_KP_7:        return KeyCode::Keypad7;
                case SDL_SCANCODE_KP_8:        return KeyCode::Keypad8;
                case SDL_SCANCODE_KP_9:        return KeyCode::Keypad9;
                case SDL_SCANCODE_KP_PERIOD:   return KeyCode::KeypadDecimal;
                case SDL_SCANCODE_KP_PLUS:     return KeyCode::KeypadPlus;
                case SDL_SCANCODE_KP_MINUS:    return KeyCode::KeypadMinus;
                case SDL_SCANCODE_KP_MULTIPLY: return KeyCode::KeypadMultiply;
                case SDL_SCANCODE_KP_DIVIDE:   return KeyCode::KeypadDivide;
                case SDL_SCANCODE_KP_ENTER:    return KeyCode::KeypadEnter;
                case SDL_SCANCODE_KP_EQUALS:   return KeyCode::KeypadEquals;
                case SDL_SCANCODE_KP_CLEAR:    return KeyCode::KeypadClear;

                default: return KeyCode::Unknown;
            }
        }

        MouseButton ToMouseButton(uint8 button) noexcept
        {
            switch (button)
            {
                case SDL_BUTTON_LEFT:  return MouseButton::Left;
                case SDL_BUTTON_RIGHT: return MouseButton::Right;
                default:               return MouseButton::Middle;
            }
        }

        TouchPhase ToTouchPhase(uint32 eventType) noexcept
        {
            switch (eventType)
            {
                case SDL_EVENT_FINGER_DOWN:   return TouchPhase::Began;
                case SDL_EVENT_FINGER_MOTION: return TouchPhase::Moved;
                case SDL_EVENT_FINGER_UP:     return TouchPhase::Ended;
                default:                      return TouchPhase::Cancelled;
            }
        }

        TouchDevice ToTouchDevice(SDL_TouchDeviceType type) noexcept
        {
            switch (type)
            {
                case SDL_TOUCH_DEVICE_INDIRECT_ABSOLUTE: return TouchDevice::IndirectAbsolute;
                case SDL_TOUCH_DEVICE_INDIRECT_RELATIVE: return TouchDevice::IndirectRelative;

                // INVALID too: no pad to normalize over means window space is the only mapping.
                default:                                 return TouchDevice::Direct;
            }
        }
    }
}
