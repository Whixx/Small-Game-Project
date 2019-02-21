#version 440

layout (points) in;
layout (triangle_strip, max_vertices = 6) out;

out vec3 outPosition;
out vec2 outTexCoords;
out vec3 outNormal;
out vec3 outTangent;

uniform int width;
uniform int height;

uniform sampler2D texture;

// This uniform are used to scale the UV-coords along with the walls.
uniform int scaleUVXZ;

void DrawFloor(float i, float j);

void main()
{
	vec3 pixelValue;
	
	vec3 black = vec3(0.0f, 0.0f, 0.0f);

	// Find i and j depending on the vertex ID (outputs from the vertex shader and stored in gl_Position)
	vec4 pointPosition = gl_in[0].gl_Position;
	int i = int(pointPosition.x);
	int j = int(pointPosition.y);

	// Find the pixelColor
	pixelValue = texelFetch(texture, ivec2(i,j), 0).rgb;
	pixelValue *= 255;

	pixelValue.r = floor(pixelValue.r);
	pixelValue.g = floor(pixelValue.g);
	pixelValue.b = floor(pixelValue.b);

	i -= (width / 2);
	j -= (height / 2);
	// Draw floor depending on if the color of the pixel is black
	if(pixelValue == black)
	{
		DrawFloor(i, j);
	}
}
	
void DrawFloor(float i, float j)
{
	// First Triangle
	// Left Bottom vertex
	outPosition = vec3(i - 0.5f, 0, j + 0.5);
	outTexCoords = vec2(0, 0);
	outNormal  = vec3(0.0f, 1.0f, 0.0f);
	outTangent = vec3(1.0f, 0.0f, 0.0f);
	EmitVertex();												  
			
	//Right Top Vertex											  
	outPosition = vec3(i + 0.5f, 0, j - 0.5);
	outTexCoords = vec2(scaleUVXZ, scaleUVXZ);
	outNormal  = vec3(0.0f, 1.0f, 0.0f);
	outTangent = vec3(1.0f, 0.0f, 0.0f);
	EmitVertex();

	// Left Top Vertex											  
	outPosition = vec3(i - 0.5f, 0, j - 0.5);
	outTexCoords = vec2(0, scaleUVXZ);
	outNormal  = vec3(0.0f, 1.0f, 0.0f);
	outTangent = vec3(1.0f, 0.0f, 0.0f);
	EmitVertex();												  		
	EndPrimitive();
		
		

	// Second Triangle
	// Left Bottom vertex
	outPosition = vec3(i - 0.5f, 0, j + 0.5);
	outTexCoords = vec2(0, 0);
	outNormal  = vec3(0.0f, 1.0f, 0.0f);
	outTangent = vec3(1.0f, 0.0f, 0.0f);
	EmitVertex();
		
	// Right Bottom vertex
	outPosition = vec3(i + 0.5f, 0, j + 0.5);
	outTexCoords = vec2(scaleUVXZ, 0);
	outNormal  = vec3(0.0f, 1.0f, 0.0f);
	outTangent = vec3(1.0f, 0.0f, 0.0f);
	EmitVertex();

	// Right Top vertex
	outPosition = vec3(i + 0.5f, 0, j - 0.5);
	outTexCoords = vec2(scaleUVXZ, scaleUVXZ);
	outNormal  = vec3(0.0f, 1.0f, 0.0f);
	outTangent = vec3(1.0f, 0.0f, 0.0f);
	EmitVertex();		
	EndPrimitive();
}