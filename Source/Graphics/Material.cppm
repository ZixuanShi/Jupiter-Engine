// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Material;

import jpt.LinearColor;
import jpt.TypeDefs;

export namespace jpt
{
    /** How a surface answers light */
    struct Material
    {
        LinearColor baseColor = LinearColor::White();   // Multiplies the base colour map
        float32 roughness = 0.35f;
        float32 metallic  = 0.0f;
        float32 occlusion = 1.0f;

        /** Which term to show on its own. jpt::ViewMode in Graphics/Shader/ShaderTypes.h owns the
            ordering; a plain index keeps that header, which only the Metal side can include, out
            of every module that carries a Material. */
        uint32 viewMode = 0;
    };
}
