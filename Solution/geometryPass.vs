#version 440

in vec3 position;
in vec2 texCoord;

out vec2 texCoord0;

void main()
{
	gl_Position = vec4(position,1.0);
	texCoord0 = texCoord;
}