// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Camera;

import jpt.Event_Key;
import jpt.Event_Mouse_Button;
import jpt.Event_Mouse_Move;
import jpt.Event_Mouse_Scroll;

import jpt.Constants;
import jpt.TypeDefs;

import jpt.Vector2;
import jpt.Vector3;
import jpt.Matrix44;

export namespace jpt
{
    class Window;

    class Camera
    {
    private:
        enum class MouseMode : uint8
        {
            Pan,    // Horizontal/Vertical movement
            Orbit,  // Rotate
        };

    private:
        // Position and Rotation
        Vec3 m_positionWS = Vec3(2.0f, 2.0f, 2.0f);   /**< Current world position */
        Precision m_pitch = 0.0f;   /**< (-Pi/2, Pi/2) */
        Precision m_yaw   = 0.0f;   /**< (-Pi, Pi) */

        // Updaters with delta time
        Vec3 m_mover;    /**< x = left/right, y = up/down, z = forward/backward */

        // Current window the camera is controlling. TODO: Support multiple windows
        Window* m_pWindow = nullptr;

        // Controls
        Vec2i m_lockMousePos  = Vec2i(Constants<int32>::kMax);
        MouseMode m_mouseMode = MouseMode::Pan;

        // Cached
        Vec3 m_forward;  /**< Current heading direction, normalized */

    public:
        bool Init();
        void Update(TimePrecision deltaSeconds);

        void OnKey(const Event_Key& eventKey);
        void OnMouseButton(const Event_Mouse_Button& eventMouseButton);
        void OnMouseMove(const Event_Mouse_Move& eventMouseMove);
        void OnMouseScroll(const Event_Mouse_Scroll& eventMouseScroll);

    public:
        Matrix44 CalcMatrix() const;
        Vec3 GetForward() const;
    };
}