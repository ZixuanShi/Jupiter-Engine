// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

#include <simd/simd.h>

namespace jpt
{
    struct Uniforms
    {
        simd_float4x4 modelViewProjection;
        simd_float4   baseColor;        // Tint, multiplies the base colour map
        simd_float4   dissolveColor;    // rgb burn rim, w = dissolve progress 0..1
        float dissolveEdge;             // Burning rim width, in noise units
    };
}

// The whole reason this header is shared rather than duplicated. Evaluated on both sides.
static_assert(sizeof(jpt::Uniforms) == 112, "Uniforms must match the shader's layout");
