#version 440

in vec3 inPosition;
in vec2 inTexCoords;

uniform mat4 viewProjection; 

out vec2 outTexCoords;
void main()
{
	gl_Position =  viewProjection * vec4(inPosition, 1.0f);
	outTexCoords = inTexCoords;
}