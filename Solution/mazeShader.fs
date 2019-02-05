#version 440

in vec2 texCoords;
in vec4 color;

out vec4 fragment_color;

//uniform sampler2D texture;

void main()
{
	//fragment_color= vec4(texture2D(texture,texCoords).xyz, 1.0f);
	//fragment_color = vec4(1.0f,1.0f,0.0f,1.0f);
	fragment_color = vec4(color.rgb,1.0f);
}