#version 440

layout (triangles) in;
layout (triangle_strip, max_vertices=3) out;

in vec2 texCoord0[];
out vec2 texCoords;

uniform vec3 cameraPos;

void main()
{
	//for(int i = 0; i < 64; i++)
	//{
	//	for(int j= 0; j < 64; j++)
	//	{
	//		pixelValue = texture(texture,vec2(i,j)).rgb;
	//		if(pixelValue == vec3(0,0,0))
	//		{
	//			gl_Position = vec3(i*64, 5, j*64);
	//			EmitVertex();
	//
	//			gl_Position = vec3(i*64, 5, j*64);
	//			EmitVertex();
	//
	//			gl_Position = vec3(i*64, 5, j*64);
	//			EmitVertex();
	//		}
	//	}
	//}

	for(int i = 0; i < 3; ++i)
	{
		// Pass on the worldPos and lightSpacePos.
		gl_Position = gl_in[i].gl_Position;
		texCoords = texCoord0[i];
		EmitVertex();
	}
	EndPrimitive();
}