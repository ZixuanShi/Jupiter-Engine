// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Scene;

import jpt.Camera;
import jpt.Transform;
import jpt.Vector2;

export namespace jpt
{
    /** The objects in the world. The camera is one of them: it is world-space state, not
        machinery like the window or the renderer. */
    class Scene
    {
    private:
        Camera m_camera;
        Transform m_pyramid;

    public:
        bool Init();

        [[nodiscard]] Camera& GetCamera() noexcept { return m_camera; }
        [[nodiscard]] Transform& GetPyramid() noexcept { return m_pyramid; }

    private:
        void Rotate(const Vec2& deltaPixels);
        void Translate(const Vec2& deltaPixels);
    };
}
