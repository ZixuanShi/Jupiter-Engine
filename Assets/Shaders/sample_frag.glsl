#version 450

layout(binding = 1) uniform sampler2D uniform_texSampler;


layout(push_constant) uniform PushConstantObject
{
    float value;
} push_constants;






layout(location = 0) in vec4 inFragColor;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;




layout(location = 0) out vec4 outColor;


void main() 
{

    outColor = texture(uniform_texSampler, inTexCoord);
    
}
