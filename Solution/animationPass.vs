#version 440

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 tangent;
layout(location = 4) in ivec4 boneIDs;
layout(location = 5) in vec4 weights;

out mat3 TBN0;
out vec2 texCoord0;
out vec3 posWorld0;

uniform mat4 WorldMatrix;
uniform mat4 transformationMatrix;
uniform mat4 Bones[50];

void main()
{
	mat4 boneTransform = Bones[boneIDs[0]] * weights[0];
		boneTransform += Bones[boneIDs[1]] * weights[1];
		boneTransform += Bones[boneIDs[2]] * weights[2];
		boneTransform += Bones[boneIDs[3]] * weights[3];

	// Convert to worldspace
	vec3 N = normalize(vec3(WorldMatrix * vec4(normal, 0.0)));
	vec3 T = normalize(vec3(WorldMatrix * vec4(tangent, 0.0)));

	// Gran-schmidt
	T = normalize(T - dot(T, N) * N);

	// Calculate bitangent
	vec3 B = cross(N, T);

	// Create the matrix that will transform the normalMap to tangent space.
	TBN0 = mat3(T, B, N);

	// Apply animation to the position
	vec4 pos = boneTransform * vec4(position, 1.0);
	//vec4 pos = vec4(position, 1.0);

	// Transform to worldspace
	posWorld0 = (WorldMatrix * pos).xyz;
	texCoord0 = texCoord;
	gl_Position = transformationMatrix * vec4(position,1.0);
}