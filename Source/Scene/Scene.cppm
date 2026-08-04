// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Scene;

import jpt.Camera;
import jpt.Input;
import jpt.Transform;
import jpt.TypeDefs;
import jpt.Vector2;
import jpt.Vector3;

export namespace jpt
{
    /** WASD and the arrows as a -1..1 pair, x right and y up. Free and taking Input rather than
        reading it inside Scene, so the key mapping is testable without an Application. */
    [[nodiscard]] Vec2 GetMoveAxis(const Input& input) noexcept;

    /** The objects in the world. The camera is one of them: it is world-space state, not
        machinery like the window or the renderer. */
    class Scene
    {
    private:
        Camera m_camera;
        Transform m_pyramid;
        float32 m_yaw   = 0.0f;
        float32 m_pitch = 0.0f;

    public:
        bool Init();
        void Update();

        [[nodiscard]] Camera& GetCamera() noexcept { return m_camera; }
        [[nodiscard]] Transform& GetPyramid() noexcept { return m_pyramid; }

    private:
        void Rotate(const Vec2& deltaPixels);
        void Translate(const Vec2& deltaPixels);
        void Move(const Vec3& worldOffset);
    };
}
