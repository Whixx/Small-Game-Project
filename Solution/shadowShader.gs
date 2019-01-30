#version 440

layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

uniform mat4 shadowMatrices[6];

out vec4 worldPos;

void main()
{
	for(int face = 0; face < 6; ++face)
	{
		gl_Layer = face;	// Specifies to which face of the cubemap we render to
		for(int i = 0; i < 3; ++i)
		{
			// Pass on the worldPos and lightSpacePos.
			worldPos = gl_in[i].gl_Position;
			gl_Position = shadowMatrices[face] * worldPos;
			EmitVertex();
		}
		EndPrimitive();
	}
}