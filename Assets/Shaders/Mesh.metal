// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include <metal_stdlib>
using namespace metal;

#include "Graphics/Shader/ShaderTypes.h"

// [[attribute(n)]] indices match the MTLVertexDescriptor built in MetalRenderer::CreatePipeline.
// The descriptor owns the layout, so this struct never needs to agree on offsets or stride.
struct VertexIn
{
    float3 position [[attribute(0)]];
    float3 normal   [[attribute(1)]];
};

struct VertexOut
{
    float4 position [[position]];
    float3 worldNormal;
};

// buffer(1), because the vertex descriptor already owns buffer(0).
vertex VertexOut MeshVertex(VertexIn in [[stage_in]],
                            constant jpt::Uniforms& uniforms [[buffer(1)]])
{
    VertexOut out;

    out.position = uniforms.modelViewProjection * float4(in.position, 1.0);

    // w = 0 marks a direction, so the model matrix's translation column cannot apply.
    out.worldNormal = (uniforms.model * float4(in.normal, 0.0)).xyz;

    return out;
}

// Linear out. The attachment is an _sRGB format, so the hardware encodes this on write -- doing
// it here with a pow() would double-encode, and would put any future blending in the wrong space.
fragment float4 MeshFragment(VertexOut in [[stage_in]])
{
    // Deliberately off-axis, so no two faces of the pyramid catch it equally -- a symmetric
    // light would shade the facets identically and hide whether normals arrived at all.
    constexpr float3 kLightDirection = float3(-0.398, 0.697, 0.597);
    constexpr float3 kAlbedo         = float3(0.80, 0.42, 0.12);

    // Renormalize: the rasterizer interpolates the corner normals linearly, which shortens
    // the result everywhere except at the corners themselves.
    const float3 normal  = normalize(in.worldNormal);
    const float  diffuse = saturate(dot(normal, kLightDirection));

    // Ambient as a fraction of albedo, so an unlit face is a dark version of the surface
    // rather than a colour of its own.
    return float4(kAlbedo * (diffuse + 0.12), 1.0);
}
