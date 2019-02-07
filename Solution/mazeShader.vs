#version 440


//void main()
//{
//	// Find i
//	int i = gl_VertexID / 64;
//	// Find j
//	int j = gl_VertexID % 64;
//
//	gl_Position = vec4(i, j, 0.0, 1.0);
//}


in vec3 position;
in vec2 texCoord;

out vec2 texCoord0;

void main()
{
	gl_Position = vec4(position, 1.0f);
	texCoord0 = texCoord;
}