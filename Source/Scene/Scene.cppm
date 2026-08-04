// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Scene;

import jpt.Camera;
import jpt.Transform;
import jpt.TypeDefs;
import jpt.Vector2;
import jpt.Vector3;

export namespace jpt
{
    /** The objects in the world. The camera is one of them: it is world-space state, not
        machinery like the window or the renderer. */
    class Scene
    {
    private:
        Camera m_camera;
        Transform m_model;
        float32 m_yaw   = 0.0f;
        float32 m_pitch = 0.0f;

    public:
        bool Init();
        void Update();

        [[nodiscard]] Camera& GetCamera() noexcept { return m_camera; }
        [[nodiscard]] Transform& GetModel() noexcept { return m_model; }

    private:
        void RotateModel(const Vec2& deltaPixels);
        void TranslateModel(const Vec2& deltaPixels);
    };
}
