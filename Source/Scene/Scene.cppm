// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Scene;

import jpt.Transform;

export namespace jpt
{
    /** The objects in the world. */
    class Scene
    {
    private:
        Transform m_pyramid;

    public:
        void Update();

        [[nodiscard]] Transform& GetPyramid() noexcept { return m_pyramid; }
    };
}
