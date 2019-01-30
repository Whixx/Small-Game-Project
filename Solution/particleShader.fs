#version 440

in vec4 finalColor;

out vec4 fragment_Color;

void main()
{
	fragment_Color = finalColor;
}