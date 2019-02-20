#version 440

uniform int width;
uniform int height;

void main()
{
	// Find i
	int i = gl_VertexID / width;
	// Find j
	int j = gl_VertexID % height;

	gl_Position = vec4(i, j, 0.0, 1.0);
}