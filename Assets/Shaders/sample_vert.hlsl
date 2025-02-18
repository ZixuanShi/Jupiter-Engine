

cbuffer MVP : register(b0) 
{
    matrix model; // TODO: change to push constant for each model
    matrix view;
    matrix proj;
};

struct PushConstants 
{
    float value;
};

[[vk::push_constant]]
PushConstants push_constants;

struct VSInput 
{
    float4 color : COLOR0;
    float3 position : POSITION0;
    float3 normal : NORMAL0;
    float2 texCoord : TEXCOORD0;
};

struct VSOutput 
{
    float4 position : SV_POSITION;
    float4 fragColor : COLOR0;
    float3 normal : NORMAL0;
    float2 texCoord : TEXCOORD0;
};

VSOutput main(VSInput input)
{
    VSOutput output;

    float4 worldPos = mul(model, float4(input.position, 1.0));
    float4 viewPos = mul(view, worldPos);
    output.position = mul(proj, viewPos);
    
    output.fragColor = input.color;
    output.normal    = input.normal;
    output.texCoord  = input.texCoord;

    return output;
}