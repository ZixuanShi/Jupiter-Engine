cbuffer UniformBufferObject : register(b0)
{
    matrix model;
    matrix view;
    matrix proj;
};

struct VSInput
{
    float2 position : POSITION0;    // Matches layout(location = 0)
    float3 color : COLOR0;         // Matches layout(location = 1)
};

struct VSOutput
{
    float4 position : SV_POSITION;  // Required for vertex shader output
    float3 color : COLOR0;         // Must match your pixel shader input
};

VSOutput main(VSInput input)
{
    VSOutput output;
    
    // HLSL matrix multiplication order is different from GLSL
    float4 worldPos = mul(float4(input.position, 0.0, 1.0), model);
    float4 viewPos = mul(worldPos, view);
    output.position = mul(viewPos, proj);
    
    output.color = input.color;
    return output;
}