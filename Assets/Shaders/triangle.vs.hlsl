cbuffer UniformBufferObject : register(b0)
{
    matrix model;
    matrix view;
    matrix proj;
};

struct VertexInput
{
    float2 position : POSITION;
    float3 color : COLOR;
};

struct VertexOutput
{
    float4 position : SV_POSITION;
    float3 color : COLOR;
};

VertexOutput main(VertexInput input)
{
    VertexOutput output;
    
    float4 positionWorld = mul(float4(input.position, 0.0f, 1.0f), model);
    float4 positionView = mul(positionWorld, view);
    output.position = mul(positionView, proj);
    
    output.color = input.color;
    
    return output;
}