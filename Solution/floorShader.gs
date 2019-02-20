#version 440

layout (points) in;
layout (triangle_strip, max_vertices = 6) out;

out vec3 outPosition;
out vec2 outTexCoords;

uniform sampler2D texture;

uniform int width;
uniform int height;
uniform int scaleUVXZ;

void drawFloor(float i, float j);

void main()
{
	vec3 pixelValue;
	
	vec3 black = vec3(0.0f, 0.0f, 0.0f);

	// Find i and j depending on the vertex ID (outputs from the vertex shader and stored in gl_Position)
	vec4 pointPosition = gl_in[0].gl_Position;
	float i = int(pointPosition.x);
	float j = int(pointPosition.y);

	// Find the pixelColor
	pixelValue = texelFetch(texture, ivec2(i,j), 0).rgb;
	pixelValue *= 255;

	pixelValue.r = floor(pixelValue.r);
	pixelValue.g = floor(pixelValue.g);
	pixelValue.b = floor(pixelValue.b);

	// Draw floor depending on if the color of the pixel is black
	if(pixelValue == black)
	{
		// Draw floor in local space. (Move i and j so that origo is in the middle of the maze)
		i -= (width / 2);
		j -= (height / 2);

		drawFloor(i, j);
	}
}
		 
// Draw the floor
void drawFloor(float i, float j)
{
	// First Triangle
	// Left Bottom vertex
	outPosition = vec3(i - 0.5f, 0, j + 0.5);
	outTexCoords = vec2(0, 0);
	EmitVertex();												  
			
	//Right Top Vertex											  
	outPosition = vec3(i + 0.5f, 0, j - 0.5);
	outTexCoords = vec2(scaleUVXZ, scaleUVXZ);
	EmitVertex();

	// Left Top Vertex											  
	outPosition = vec3(i - 0.5f, 0, j - 0.5);
	outTexCoords = vec2(0, scaleUVXZ);
	EmitVertex();												  		
	EndPrimitive();
		
		

	// Second Triangle
	// Left Bottom vertex
	outPosition = vec3(i - 0.5f, 0, j + 0.5);
	outTexCoords = vec2(0, 0);
	EmitVertex();
		
	// Right Bottom vertex
	outPosition = vec3(i + 0.5f, 0, j + 0.5);
	outTexCoords = vec2(scaleUVXZ, 0);
	EmitVertex();

	// Right Top vertex
	outPosition = vec3(i + 0.5f, 0, j - 0.5);
	outTexCoords = vec2(scaleUVXZ, scaleUVXZ);
	EmitVertex();		
	EndPrimitive();
}