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

        // The drag is kept as angles and the rotation rebuilt from them each time. Composing onto
        // the previous rotation instead lets roll accumulate: two drags about different axes
        // compose to include a twist, and the pyramid tips out of upright and never recovers.
        float32 m_yaw   = 0.0f;
        float32 m_pitch = 0.0f;

    public:
        bool Init();

        /** Held keys are polled, not dispatched: "is W down right now" is one array read, while
            deriving it from down/up pairs makes every consumer track its own state. */
        void Update();

        [[nodiscard]] Camera& GetCamera() noexcept { return m_camera; }
        [[nodiscard]] Transform& GetPyramid() noexcept { return m_pyramid; }

    private:
        void Rotate(const Vec2& deltaPixels);
        void Translate(const Vec2& deltaPixels);
        void Move(const Vec3& worldOffset);
    };
}
