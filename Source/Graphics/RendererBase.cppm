// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

// Import-free by design, so it costs a module interface unit nothing to size an array from it.
#include "Graphics/GraphicsConstants.h"

export module jpt.RendererBase;

import jpt.Light;
import jpt.LinearColor;
import jpt.Material;
import jpt.Matrix44;
import jpt.TypeDefs;
import jpt.Vector3;
import std;

export namespace jpt
{
    class RendererBase
    {
    protected:
        Mat44 m_model            = Mat44::Identity();
        Mat44 m_viewProjection   = Mat44::Identity();
        LinearColor m_clearColor = LinearColor::Black();
        Vec3 m_cameraPosition    = Vec3::Zero();
        Material m_material;
        AmbientLight m_ambient;
        std::array<PointLight, kMaxPointLights> m_pointLights;
        float32 m_time           = 0.0f;

    public:
        bool PreInit() noexcept;
        void Update();

    public:
        [[nodiscard]] const LinearColor& GetClearColor() const noexcept { return m_clearColor; }
        void SetClearColor(const LinearColor& color) noexcept { m_clearColor = color; }
    };
}
