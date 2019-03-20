#version 440

out vec4 fragment_color;

in vec2 texCoord0;

uniform sampler2D texture;

void main()
{
	// Sample from the texture
	vec4 materialColor = texture2D(texture, texCoord0);
	
	fragment_color = materialColor;
}