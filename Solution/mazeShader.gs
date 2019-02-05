#version 440

layout (triangles) in;
layout (triangle_strip, max_vertices=256) out;

in vec2 texCoord0[];
out vec2 texCoords;
out vec4 color;

uniform vec3 cameraPos;

uniform mat4 transformationMatrix; 
uniform mat4 WorldMatrix;

uniform sampler2D texture;

void main()
{
	vec3 pixelValue;
	

	vec3 red = vec3(1.0f, 0.0f, 0.0f);
	vec3 white = vec3(1.0f, 1.0f, 1.0f);
	vec3 black = vec3(0.0f, 0.0f, 0.0f);
	vec3 blue = vec3(0.0f, 0.0f, 1.0f);

	for(int i = 0; i < 64; i++)
	{
		for(int j= 5; j < 64; j++)
		{
			float worldX = float(i)/63.0f;
			float worldY = float(j)/63.0f;
	
			pixelValue = texture(texture,vec2(worldX, worldY)).rgb;
	
			if(pixelValue != black)
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
				
				
				//// Left Bottom vertex
				//gl_Position = transformationMatrix * vec4(i - 0.5f, 0, j, 1.0f);
				//EmitVertex();
				//
				//// Right Top vertex
				//gl_Position = transformationMatrix * vec4(i + 0.5f, 1, j, 1.0f);
				//EmitVertex();
				//
				//// Right Bottom vertex
				//gl_Position = transformationMatrix * vec4(i + 0.5, 0, j, 1.0f);
				//EmitVertex();
				//
				//EndPrimitive();
	
	
	
	
	
				//gl_Position = vec4(-0.5,-0.5,0,1.0f);
				//EmitVertex();
				//
				//gl_Position = vec4(0.5,-0.5,0,1.0f);
				//EmitVertex();
				//
				//gl_Position = vec4(0.0, 0.5,0, 1.0f);
				//EmitVertex();
				//
				//EndPrimitive();
	
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