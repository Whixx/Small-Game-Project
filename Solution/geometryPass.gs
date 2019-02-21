#version 440

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in mat3 TBN0[];
in vec2 texCoord0[];
in vec3 normal0[];
in vec3 posWorld0[];

out vec2 texCoords;
out vec3 posWorld1;
out mat3 TBN;

uniform mat4 transformationMatrix;

void main()
{
	for(int i = 0; i < 3; i++)
	{
		gl_Position = transformationMatrix * gl_in[i].gl_Position;
		texCoords = texCoord0[i];
		posWorld1 = posWorld0[i];
		TBN = TBN0[i];
		EmitVertex();
	}
	EndPrimitive();
}