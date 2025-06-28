

Texture2D textureSampler : register(t1);
SamplerState samplerState : register(s1);

struct PushConstants 
{
    float value;
};

[[vk::push_constant]]
PushConstants push_constants;

struct PSInput 
{
    float4 fragColor : COLOR0;
    float3 normal : NORMAL0;
    float2 uv : TEXCOORD0;
};

struct PSOutput 
{
    float4 color : SV_Target0;
};

PSOutput main(PSInput input)
{
    PSOutput output;
    output.color = textureSampler.Sample(samplerState, input.uv);
    return output;
}