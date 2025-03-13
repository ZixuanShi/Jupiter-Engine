#version 450

layout(binding = 0) uniform MVP
{
    mat4 model; // TODO: change to push constant for each model
    mat4 view;
    mat4 proj;
} uniform_mvp;

layout(push_constant) uniform PushConstantObject
{
    float value;
} push_constants;






layout(location = 0) in vec4 inColor;
layout(location = 1) in vec3 inPosition;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec2 inUV;




layout(location = 0) out vec4 outFragColor;
layout(location = 1) out vec3 outNormal;
layout(location = 2) out vec2 outUV;



void main() 
{


    vec4 worldPos = uniform_mvp.model * vec4(inPosition, 1.0);
    vec4 viewPos  = uniform_mvp.view * worldPos;
    gl_Position   = uniform_mvp.proj * viewPos;
    
    outFragColor = inColor;
    outNormal    = inNormal;
    outUV        = inUV;


}