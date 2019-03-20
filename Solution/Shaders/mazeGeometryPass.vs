#version 440 

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 tangent;
layout(location = 4) in float type;

out vec3 normal_CS_in;
out vec3 tangent_CS_in;
out vec2 texCoord0_CS_in;
out vec3 posWorld0_CS_in;
out float type0_CS_in;

uniform mat4 WorldMatrix;

void main()
{
	// Convert to worldspace
	normal_CS_in = normalize(vec3(WorldMatrix * vec4(normal, 0.0)));
	tangent_CS_in = normalize(vec3(WorldMatrix * vec4(tangent, 0.0)));

	// Gran-schmidt
	tangent_CS_in = normalize(tangent_CS_in - dot(tangent_CS_in, normal_CS_in) * normal_CS_in);
	posWorld0_CS_in = (WorldMatrix * vec4(position, 1.0)).xyz;
	texCoord0_CS_in = texCoord;

	type0_CS_in = type;
}