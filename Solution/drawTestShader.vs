#version 440

in vec3 position;
in vec2 texCoord;

//uniform mat4 transformationMatrix;
//uniform mat4 WorldMatrix;
uniform mat4 viewProjection; 

out vec2 outTexCoords;
void main()
{
	gl_Position =  viewProjection * vec4(position, 1.0f);
	outTexCoords = texCoord;
}