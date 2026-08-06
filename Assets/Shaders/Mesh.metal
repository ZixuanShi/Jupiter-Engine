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
    float3 worldPosition;
    float3 modelPosition;   // For noise sampling in model space, so the dissolve effect is consistent across models.
    float3 worldNormal;
    float2 uv;
    float4 color;
};

// buffer(1), because the vertex descriptor already owns buffer(0).
vertex VertexOut MeshVertex(VertexIn in [[stage_in]],
                            constant jpt::Uniforms& uniforms [[buffer(1)]])
{
    VertexOut out;

    out.position      = uniforms.modelViewProjection * float4(in.position, 1.0);
    out.worldPosition = (uniforms.model * float4(in.position, 1.0)).xyz;
    out.modelPosition = in.position;
    out.worldNormal   = (uniforms.model * float4(in.normal, 0.0)).xyz;   // w = 0: a direction does not translate.
    out.uv            = in.uv;
    out.color         = in.color;

    return out;
}

// Every dielectric reflects about 4% head-on whatever it is made of, and a metal reflects its own
// colour and refracts nothing. Those two facts are the whole of the metallic workflow.
constant float kDielectricF0 = 0.04;

struct Surface
{
    float3 albedo;
    float3 normal;
    float3 F0;          // Reflectance at normal incidence
    float roughness;
    float metallic;
    float occlusion;
};

Surface ReadSurface(VertexOut in, constant jpt::Uniforms& uniforms,
                    texture2d<float> baseColorMap, sampler mapSampler)
{
    Surface surface;
    surface.albedo = (baseColorMap.sample(mapSampler, in.uv) * uniforms.baseColor * in.color).rgb;
    surface.normal = normalize(in.worldNormal);
    surface.roughness = clamp(uniforms.roughness, 0.03, 1.0);
    surface.metallic  = uniforms.metallic;
    surface.occlusion = uniforms.occlusion;
    surface.F0        = mix(float3(kDielectricF0), surface.albedo, surface.metallic);

    return surface;
}

// GGX / Trowbridge-Reitz. Its long tail is what gives a rough surface a wide dim highlight
// rather than Phong's hard-edged one.
float DistributionGGX(float NdotH, float roughness)
{
    const float a2 = pow(roughness, 4.0);
    const float d  = NdotH * NdotH * (a2 - 1.0) + 1.0;
    return a2 / max(M_PI_F * d * d, 1e-7);
}

// Smith, with Schlick's k remapped for punctual light -- microfacets shadow each other on the way
// in and mask each other on the way out, so the same term applies twice.
float GeometrySmith(float NdotV, float NdotL, float roughness)
{
    const float k = (roughness + 1.0) * (roughness + 1.0) / 8.0;
    const float shadowing = NdotL / (NdotL * (1.0 - k) + k);
    const float masking   = NdotV / (NdotV * (1.0 - k) + k);
    return shadowing * masking;
}

float3 FresnelSchlick(float cosTheta, float3 F0)
{
    return F0 + (1.0 - F0) * pow(saturate(1.0 - cosTheta), 5.0);
}

float3 ShadePointLight(Surface surface, jpt::GpuPointLight light, float3 worldPosition, float3 viewDirection)
{
    const float3 toLight   = light.position.xyz - worldPosition;
    const float  distance2 = max(dot(toLight, toLight), 1e-4);
    const float3 lightDirection = toLight * rsqrt(distance2);
    const float3 halfway = normalize(lightDirection + viewDirection);

    const float NdotL = saturate(dot(surface.normal, lightDirection));
    const float NdotV = saturate(dot(surface.normal, viewDirection));
    const float NdotH = saturate(dot(surface.normal, halfway));

    const float3 F = FresnelSchlick(saturate(dot(halfway, viewDirection)), surface.F0);
    const float  D = DistributionGGX(NdotH, surface.roughness);
    const float  G = GeometrySmith(NdotV, NdotL, surface.roughness);

    const float3 specular = (D * G) * F / max(4.0 * NdotV * NdotL, 1e-4);

    // What is not reflected is refracted, and a metal absorbs all of it.
    const float3 diffuse = (1.0 - F) * (1.0 - surface.metallic) * surface.albedo / M_PI_F;

    // position.w is the enabled flag, color.w the intensity. Inverse square, because a point
    // light spreads its power over a sphere.
    const float3 radiance = light.color.rgb * (light.color.w * light.position.w / distance2);

    return (diffuse + specular) * radiance * NdotL;
}

// The irradiance of a uniform sky dome over a uniform ground: the exact answer for that
// environment rather than a flat ambient fudge.
float3 HemisphereIrradiance(float3 direction, float3 sky, float3 ground)
{
    return mix(ground, sky, direction.y * 0.5 + 0.5);
}

float3 IndirectLight(Surface surface, float3 viewDirection, float3 sky, float3 ground)
{
    const float3 F = FresnelSchlick(saturate(dot(surface.normal, viewDirection)), surface.F0);
    const float3 reflected = normalize(mix(reflect(-viewDirection, surface.normal), surface.normal, surface.roughness));
    const float3 diffuse  = HemisphereIrradiance(surface.normal, sky, ground) * surface.albedo * (1.0 - surface.metallic);
    const float3 specular = HemisphereIrradiance(reflected, sky, ground);

    return ((1.0 - F) * diffuse + F * specular) * surface.occlusion;
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

float3 Resolve(jpt::ViewMode viewMode, Surface surface, float3 direct, float3 indirect)
{
    switch (viewMode)
    {
        case jpt::ViewMode::BaseColor: return surface.albedo;
        case jpt::ViewMode::Normal:    return surface.normal * 0.5 + 0.5;
        case jpt::ViewMode::Roughness: return float3(surface.roughness);
        case jpt::ViewMode::Metallic:  return float3(surface.metallic);
        default:                       return direct + indirect;
    }
}

// Linear out. The attachment is an _sRGB format, so the hardware encodes this on write -- doing
// it here with a pow() would double-encode, and would put any future blending in the wrong space.
// The base colour map is _sRGB too, so its sample arrives linear for the same reason.
fragment float4 MeshFragment(VertexOut in [[stage_in]],
                             constant jpt::Uniforms& uniforms [[buffer(1)]],
                             texture2d<float> baseColorMap [[texture(0)]],
                             sampler mapSampler [[sampler(0)]])
{
    const Surface surface = ReadSurface(in, uniforms, baseColorMap, mapSampler);
    const float3 viewDirection = normalize(uniforms.cameraPosition.xyz - in.worldPosition);

    float3 direct = float3(0.0);
    for (uint i = 0; i < jpt::kMaxPointLights; ++i)
    {
        direct += ShadePointLight(surface, uniforms.pointLights[i], in.worldPosition, viewDirection);
    }

    const float3 indirect = IndirectLight(surface, viewDirection, uniforms.skyColor.rgb, uniforms.groundColor.rgb);

    const float3 resolved = Resolve(uniforms.viewMode, surface, direct, indirect);
    return float4(ApplyDissolve(resolved, in.modelPosition, uniforms), 1.0);
}
