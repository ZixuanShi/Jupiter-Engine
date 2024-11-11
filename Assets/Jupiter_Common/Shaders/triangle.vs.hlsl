struct VSOutput 
{
    float4 position : SV_POSITION;
    float3 color : COLOR0;
};

static const float2 positions[3] = 
{
    float2(0.0, -0.5),
    float2(0.5, 0.5),
    float2(-0.5, 0.5)
};

static const float3 colors[3] = 
{
    float3(1.0, 0.0, 0.0),    // Red
    float3(0.0, 1.0, 0.0),    // Green
    float3(0.0, 0.0, 1.0)     // Blue
};

VSOutput main(uint vertexID : SV_VertexID) 
{
    VSOutput output;
    output.position = float4(positions[vertexID], 0.0, 1.0);
    output.color = colors[vertexID];
    return output;
}