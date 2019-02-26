#version 440
precision highp int;

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 tangent;
layout(location = 4) in float type;

out mat3 TBN0;
out vec2 texCoord0;
out vec3 posWorld0;
out float type0;

uniform mat4 WorldMatrix;
uniform mat4 transformationMatrix;

void main()
{
	// Convert to worldspace
	vec3 N = normalize(vec3(WorldMatrix * vec4(normal, 0.0)));
	vec3 T = normalize(vec3(WorldMatrix * vec4(tangent, 0.0)));

	// Gran-schmidt
	T = normalize(T - dot(T, N) * N);

	// Calculate bitangent
	vec3 B = cross(N, T);

	// Create the matrix that will transform the normalMap to tangent space.
	TBN0 = mat3(T, B, N);

	posWorld0 = (WorldMatrix * vec4(position, 1.0)).xyz;
	texCoord0 = texCoord;
	gl_Position = transformationMatrix * vec4(position,1.0);

	type0 = type;
}