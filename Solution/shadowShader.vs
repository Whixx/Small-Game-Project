#version 440

in vec3 position;

uniform mat4 WorldMatrix;

void main()
{
	// The vertex is transformed to worldspace and then sent to the GS
	gl_Position = WorldMatrix * vec4(position, 1.0f);
}