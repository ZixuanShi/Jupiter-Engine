#version 450

layout(binding = 1) uniform sampler2D texSampler;

layout(location = 0) in vec3 inFragColor;
layout(location = 1) in vec2 inFragTexCoord;

layout(location = 0) out vec4 outColor;

layout(push_constant) uniform PushConstantObject
{
    float value;
} pco;

void main() 
{
    outColor = texture(texSampler, inFragTexCoord);
}
