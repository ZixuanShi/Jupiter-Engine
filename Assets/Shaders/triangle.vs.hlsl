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
    output.position = float4(input.position, 0.0, 1.0);
    output.color = input.color;
    return output;
}