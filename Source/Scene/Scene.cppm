// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Scene;

import jpt.Camera;
import jpt.GestureRecognizer;
import jpt.InputEvents;
import jpt.Material;
import jpt.Transform;
import jpt.TypeDefs;
import jpt.Vector2;
import jpt.Vector3;
import std;

export namespace jpt
{
    /** The objects in the world. The camera is one of them: it is world-space state, not
        machinery like the window or the renderer. */
    class Scene
    {
    private:
        Camera m_camera;
        Transform m_model;
        Material m_material;
        float32 m_yaw   = 0.0f;
        float32 m_pitch = 0.0f;
        float32 m_roll  = 0.0f;

    public:
        bool Init();
        void Update();

        [[nodiscard]] Camera& GetCamera() noexcept { return m_camera; }
        [[nodiscard]] Transform& GetModel() noexcept { return m_model; }
        [[nodiscard]] Material& GetMaterial() noexcept { return m_material; }

    private:
        void RotateModel(const Vec2& deltaPixels);
        void TwistModel(float32 radians);
        void TranslateModel(const Vec2& deltaPixels);
        void ApplyRotation();

        void OnPan(const PanEvent& event);
        void OnTwist(const TwistEvent& event);
        void OnMouseMove(const MouseMoveEvent& event);
    };
}
