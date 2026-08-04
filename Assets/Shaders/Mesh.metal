// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include <metal_stdlib>
using namespace metal;

#include "Graphics/Shader/ShaderTypes.h"

// [[attribute(n)]] indices match the MTLVertexDescriptor built in Metal4Renderer::CreatePipeline.
// The descriptor owns the layout, so this struct never needs to agree on offsets or stride --
// and float3 costing 16 bytes here rather than 12 is harmless for the same reason.
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
    float3 worldNormal;
    float2 uv;
    float4 color;
};

// buffer(1), because the vertex descriptor already owns buffer(0).
vertex VertexOut MeshVertex(VertexIn in [[stage_in]],
                            constant jpt::Uniforms& uniforms [[buffer(1)]])
{
    VertexOut out;

    out.position    = uniforms.modelViewProjection * float4(in.position, 1.0);
    out.worldNormal = (uniforms.model * float4(in.normal, 0.0)).xyz;
    out.uv          = in.uv;
    out.color       = in.color;

    return out;
}

// Linear out. The attachment is an _sRGB format, so the hardware encodes this on write -- doing
// it here with a pow() would double-encode, and would put any future blending in the wrong space.
// The base colour map is _sRGB too, so its sample arrives linear for the same reason.
fragment float4 MeshFragment(VertexOut in [[stage_in]],
                             constant jpt::Uniforms& uniforms [[buffer(1)]],
                             texture2d<float> baseColorMap [[texture(0)]],
                             sampler mapSampler [[sampler(0)]])
{
    constexpr float3 kLightDirection = float3(-0.398, 0.697, 0.597);
    constexpr float  kAmbient = 0.12;

    // Renormalize: the rasterizer interpolates the corner normals linearly, which shortens
    // the result everywhere except at the corners themselves.
    const float3 normal  = normalize(in.worldNormal);
    const float  diffuse = saturate(dot(normal, kLightDirection));

    const float4 baseColor = baseColorMap.sample(mapSampler, in.uv) * in.color;
    return float4(baseColor.rgb * (diffuse + kAmbient), baseColor.a);
}
