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
fragment float4 MeshFragment(VertexOut in [[stage_in]],
                             constant jpt::Uniforms& uniforms [[buffer(1)]])
{
    // Deliberately off-axis, so no two faces of the pyramid catch it equally -- a symmetric
    // light would shade the facets identically and hide whether normals arrived at all.
    constexpr float3 kLightDirection = float3(-0.398, 0.697, 0.597);

    constexpr float kBandFrequency = 12.0;   // radians per UV unit: ~3 bands across a side face
    constexpr float kScrollSpeed   = 4.0;    // radians per second: a shade under one band a second

    // Renormalize: the rasterizer interpolates the corner normals linearly, which shortens
    // the result everywhere except at the corners themselves.
    const float3 normal  = normalize(in.worldNormal);
    const float  diffuse = saturate(dot(normal, kLightDirection));

    // Diagonal, so one band crosses both UV axes -- a UV wrong on only one axis still shows.
    // The trough only dims the vertex colour rather than erasing it, but it has to dim it hard:
    // this is multiplied by the diffuse term, which is already below 0.3 on the shaded faces.
    const float wave  = sin((in.uv.x + in.uv.y) * kBandFrequency - uniforms.time * kScrollSpeed);
    const float bands = mix(0.45, 1.0, wave * 0.5 + 0.5);

    // Ambient as a fraction of albedo, so an unlit face is a dark version of the surface
    // rather than a colour of its own.
    const float3 albedo = in.color.rgb * bands;
    return float4(albedo * (diffuse + 0.12), in.color.a);
}
