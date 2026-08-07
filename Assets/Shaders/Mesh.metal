// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include <metal_stdlib>
using namespace metal;

#include "Graphics/Shader/ShaderTypes.h"

struct VertexIn
{
    float3 position [[attribute(0)]];
    float3 normal   [[attribute(1)]];
    float2 uv       [[attribute(2)]];
    float4 color    [[attribute(3)]];
};

struct VertexOut
{
    float4 position [[position]];
    float3 modelPosition;   // For noise sampling in model space, so the dissolve effect is consistent across models.
    float2 uv;
    float4 color;
};

// buffer(1), because the vertex descriptor already owns buffer(0).
vertex VertexOut MeshVertex(VertexIn in [[stage_in]],
                            constant jpt::Uniforms& uniforms [[buffer(1)]])
{
    VertexOut out;

    out.position      = uniforms.modelViewProjection * float4(in.position, 1.0);
    out.modelPosition = in.position;
    out.uv            = in.uv;
    out.color         = in.color;

    return out;
}

float Hash(float3 cell)
{
    cell  = fract(cell * 0.1031);
    cell += dot(cell, cell.zyx + 31.32);
    return fract((cell.x + cell.y) * cell.z);
}

float ValueNoise(float3 position)
{
    const float3 cell = floor(position);
    const float3 t = fract(position);
    const float3 f = t * t * (3.0 - 2.0 * t);   // Hermite, so cell boundaries have no creases

    return mix(mix(mix(Hash(cell + float3(0, 0, 0)), Hash(cell + float3(1, 0, 0)), f.x),
                   mix(Hash(cell + float3(0, 1, 0)), Hash(cell + float3(1, 1, 0)), f.x), f.y),
               mix(mix(Hash(cell + float3(0, 0, 1)), Hash(cell + float3(1, 0, 1)), f.x),
                   mix(Hash(cell + float3(0, 1, 1)), Hash(cell + float3(1, 1, 1)), f.x), f.y), f.z);
}

constant float kDissolveScale = 6.0;    // Noise cells per model unit; the mug is one unit tall
constant float kDissolveGlow  = 4.0;    // Overdriven past 1 so the rim reads as emissive

// Model-space noise against a threshold: below it the fragment is gone, just above it burns.
float3 ApplyDissolve(float3 color, float3 modelPosition, constant jpt::Uniforms& uniforms)
{
    const float3 samplePosition = modelPosition * kDissolveScale;
    const float noise = 0.7 * ValueNoise(samplePosition) + 0.3 * ValueNoise(samplePosition * 3.07);
    const float edge = uniforms.dissolveEdge;
    const float threshold = uniforms.dissolveColor.w * (1.0 + edge) - edge;
    if (noise < threshold)
    {
        discard_fragment();
    }

    const float rim = 1.0 - smoothstep(0.0, edge, noise - threshold);
    return mix(color, uniforms.dissolveColor.rgb * kDissolveGlow, rim);
}

// Linear out. The attachment is an _sRGB format, so the hardware encodes this on write -- doing
// it here with a pow() would double-encode, and would put any future blending in the wrong space.
// The base colour map is _sRGB too, so its sample arrives linear for the same reason.
fragment float4 MeshFragment(VertexOut in [[stage_in]],
                             constant jpt::Uniforms& uniforms [[buffer(1)]],
                             texture2d<float> baseColorMap [[texture(0)]],
                             sampler mapSampler [[sampler(0)]])
{
    const float4 color = baseColorMap.sample(mapSampler, in.uv) * uniforms.baseColor * in.color;
    return float4(ApplyDissolve(color.rgb, in.modelPosition, uniforms), 1.0);
}
