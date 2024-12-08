cbuffer UniformBuffer : register(b0)
{
    matrix model;
    matrix view;
    matrix proj;
};

struct VSInput
{
    float2 position : POSITION0;
    float3 color : COLOR0;
};

struct VSOutput
{
    float4 position : SV_POSITION;
    float3 color : COLOR0;
};

VSOutput main(VSInput input)
{
    VSOutput output;
    
    // Convert 2D position to 3D by adding z=0
    float4 pos = float4(input.position, 0.0f, 1.0f);
    
    // Reverse multiplication order for HLSL row-major order
    output.position = mul(mul(mul(pos, model), view), proj);
    output.color = input.color;
    
    return output;
}