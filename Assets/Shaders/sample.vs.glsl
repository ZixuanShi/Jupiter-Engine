#version 450

layout(binding = 0) uniform MVP
{
    mat4 model; // TODO: change to push constant for each model
    mat4 view;
    mat4 proj;
} uniform_mvp;

layout(location = 0) in vec4 inColor;
layout(location = 1) in vec3 inPosition;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec2 inTexCoord;

layout(location = 0) out vec4 outFragColor;
layout(location = 1) out vec2 outFragTexCoord;

layout(push_constant) uniform PushConstantObject
{
    float value;
} pushconstant;

void main() 
{
    gl_Position = uniform_mvp.proj * uniform_mvp.view * uniform_mvp.model * vec4(inPosition, 1.0);
    outFragColor = inColor;
    outFragTexCoord = inTexCoord;
}