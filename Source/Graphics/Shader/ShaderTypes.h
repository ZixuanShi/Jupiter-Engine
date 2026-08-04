// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

// Compiled by both clang and the Metal compiler, so it must stay free of `import` -- a .metal
// translation unit cannot load a C++-built std.pcm. Keep this file to plain declarations.
//
// simd gives one spelling that means the same layout in both languages. A 3-component vector
// must never appear here: MSL's float3 is 16 bytes while a plain `float x, y, z` is 12, and
// nothing warns when they disagree. Use float4 and pack the spare component, or don't use it.
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
