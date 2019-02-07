#version 440

in vec3 color;

out vec4 fragment_color;

void main()
{
	//fragment_color = vec4(1.0f,0.0f,0.0f,1.0f);
	fragment_color = vec4(color.rgb / 255, 1.0f);
}