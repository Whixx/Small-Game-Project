#version 440

in vec3 inValue;

uniform mat4 viewProjection; 

void main()
{
	gl_Position =  viewProjection * vec4(inValue, 1.0f);
}