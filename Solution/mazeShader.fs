#version 440

in vec2 texCoords;

out vec4 fragment_color;

uniform sampler2D texture;

void main()
{
	fragment_color= vec4(texture2D(texture,texCoords).xyz, 1.0f);
}