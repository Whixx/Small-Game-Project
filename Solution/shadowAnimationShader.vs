#version 440

layout(location = 0) in vec3 position;
layout(location = 4) in ivec4 boneIDs;
layout(location = 5) in vec4 weights;

uniform mat4 WorldMatrix;
uniform mat4 Bones[50];

void main()
{
	mat4 boneTransform = Bones[boneIDs[0]] * weights[0];
		boneTransform += Bones[boneIDs[1]] * weights[1];
		boneTransform += Bones[boneIDs[2]] * weights[2];
		boneTransform += Bones[boneIDs[3]] * weights[3];

	// Apply animation to the position
	vec4 pos = boneTransform * vec4(position, 1.0);

	// The vertex is transformed to worldspace and then sent to the GS
	gl_Position = WorldMatrix * pos;
}