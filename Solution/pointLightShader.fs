#version 440

in vec3 colorOut;

out vec4 fragment_color;
out vec4 bright_color;

void main()
{
	fragment_color = vec4(colorOut.rgb, 1.0f);
	bright_color = fragment_color;
}