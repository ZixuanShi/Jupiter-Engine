// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

// Compiled by clang *and* the Metal compiler, so no `import` and plain declarations only.
// Never a 3-component vector: MSL's float3 is 16 bytes, a plain `float x, y, z` is 12, and
// nothing warns. Use float4 and pack the spare component.
#include <simd/simd.h>

namespace jpt
{
    struct Uniforms
    {
        simd_float4x4 modelViewProjection;
        simd_float4x4 model;
        float time;     // seconds since the application started
    };
}

// The whole reason this header is shared rather than duplicated. Evaluated on both sides.
static_assert(sizeof(jpt::Uniforms) == 144, "Uniforms must match the shader's layout");
