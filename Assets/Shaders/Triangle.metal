// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include <metal_stdlib>
using namespace metal;

#include "Graphics/ShaderTypes.h"

// [[attribute(n)]] indices match the MTLVertexDescriptor built in MetalRenderer::CreatePipeline.
// The descriptor owns the layout, so this struct never needs to agree on offsets or stride.
struct VertexIn
{
    float2 position [[attribute(0)]];
    float4 color    [[attribute(1)]];
};

struct VertexOut
{
    float4 position [[position]];
    float4 color;
};

vertex VertexOut TriangleVertex(VertexIn in [[stage_in]])
{
    VertexOut out;

    // Already in clip space: no matrices yet, so nothing here can be the reason for a blank screen.
    out.position = float4(in.position, 0.0, 1.0);
    out.color    = in.color;

    return out;
}

// Linear out. The attachment is an _sRGB format, so the hardware encodes this on write -- doing
// it here with a pow() would double-encode, and would put any future blending in the wrong space.
fragment float4 TriangleFragment(VertexOut in [[stage_in]])
{
    return in.color;
}
