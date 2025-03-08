#version 450

struct Particle
{
	vec2 position;
	vec2 velocity;
	vec4 color;
};

layout(std140, binding = 1) readonly buffer inParticleSSBO
{
	Particle inParticles[];
};

layout(std140, binding = 2)  buffer outParticleSSBO
{
	Particle outParticles[];
};

void main()
{

}