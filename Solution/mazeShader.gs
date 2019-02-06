#version 440

layout (triangles) in;
layout (triangle_strip, max_vertices=256) out;

in vec2 texCoord0[];
out vec2 texCoords;
out vec4 color;

uniform usampler2D texture;

uniform vec3 cameraPos;

uniform mat4 transformationMatrix; 
uniform mat4 WorldMatrix;



void main()
{
	uvec3 pixelValue;
	

	vec3 red = vec3(1, 0, 0);
	uvec3 temp = uvec3(255,0,0);
	vec3 white = vec3(1, 1, 1);
	uvec3 whiteBig = uvec3(255,255,255);
	vec3 black = vec3(0.0f, 0.0f, 0.0f);
	vec3 blue = vec3(0.0f, 0.0f, 1.0f);

	for(int i = 0; i < 64; i++)
	{
		for(int j = 0; j < 64; j++)
		{
			pixelValue = texelFetch(texture, ivec2(i,j), 0).rgb;
			
			if(pixelValue == temp)
			{
				// Left Bottom vertex
				gl_Position = transformationMatrix * vec4(i - 0.5f, 0, j, 1.0f);
				color = vec4(pixelValue.rgb,1.0f);
				EmitVertex();
				
				// Left Top Vertex
				gl_Position = transformationMatrix * vec4(i - 0.5f, 1, j, 1.0f);
				color = vec4(pixelValue.rgb,1.0f);
				EmitVertex();
				
				//Right Top Vertex
				gl_Position = transformationMatrix * vec4(i + 0.5f, 1, j, 1.0f);
				color = vec4(pixelValue.rgb,1.0f);
				EmitVertex();
				
				EndPrimitive();
				
				
				// Left Bottom vertex
				gl_Position = transformationMatrix * vec4(i - 0.5f, 0, j, 1.0f);
				EmitVertex();
				
				// Right Top vertex
				gl_Position = transformationMatrix * vec4(i + 0.5f, 1, j, 1.0f);
				EmitVertex();
				
				// Right Bottom vertex
				gl_Position = transformationMatrix * vec4(i + 0.5, 0, j, 1.0f);
				EmitVertex();
				
				EndPrimitive();
			}
		}
	}

	
	//for(int i = 0; i < 3; i++)
	//{
	//	// Pass on the worldPos and lightSpacePos.
	//	gl_Position = transformationMatrix * gl_in[i].gl_Position;
	//	texCoords = texCoord0[i];
	//	EmitVertex();
	//}
	//EndPrimitive();
}