// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

#include <simd/simd.h>

#include "Graphics/GraphicsConstants.h"

namespace jpt
{
    /** jpt::PointLight packed for the GPU -- a distinct name because this header and jpt.Light are
        both visible wherever the uniforms are filled. The spare component of each float4 carries
        what would otherwise be a scalar paying 12 bytes of padding for its place in the array. */
    struct GpuPointLight
    {
        simd_float4 position;   // xyz world, w = enabled
        simd_float4 color;      // rgb, w = intensity
    };

    /** Debug isolation, indexed by Material::viewMode and switched on by Mesh.metal's Resolve. */
    enum class ViewMode : unsigned int
    {
        Final,
        BaseColor,
        Normal,
        Roughness,
        Metallic,

        Count,
    };

    struct Uniforms
    {
        simd_float4x4 modelViewProjection;
        simd_float4x4 model;
        simd_float4   cameraPosition;
        simd_float4   baseColor;
        simd_float4   skyColor;         // Hemisphere irradiance, straight up
        simd_float4   groundColor;      // ... and straight down
        GpuPointLight pointLights[kMaxPointLights];
        float roughness;
        float metallic;
        float occlusion;
        float time;                     // Seconds since launch, for experimental effects
        ViewMode viewMode;
    };
}

// The whole reason this header is shared rather than duplicated. Evaluated on both sides.
static_assert(sizeof(jpt::Uniforms) == 320, "Uniforms must match the shader's layout");
