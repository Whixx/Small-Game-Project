#version 440

layout (points) in;
layout (triangle_strip, max_vertices = 18) out;

out vec3 outPosition;
out vec2 outTexCoords;
out vec3 outNormal;
out vec3 outTangent;

// outType is used to check if the primitive is a wall or a floor
// 0 = floor     1 = wall
out float outType;



uniform sampler2D texture;

uniform int width;
uniform int height;

// These uniforms are used to scale the UV-coords along with the walls and floor.
uniform int scaleUVXZ;
uniform int scaleUVY;

// Floor function
void DrawFloor(float i, float j);

// Single wall functions
void DrawSW_n(float i, float j);
void DrawSW_s(float i, float j);
void DrawSW_e(float i, float j);
void DrawSW_w(float i, float j);

// Corner wall functions
void DrawCW_ne(float i, float j);
void DrawCW_nw(float i, float j);
void DrawCW_sw(float i, float j);
void DrawCW_se(float i, float j);

// End wall functions
void DrawEW_n(float i, float j);
void DrawEW_s(float i, float j);
void DrawEW_e(float i, float j);
void DrawEW_w(float i, float j);

// Double wall functions
void DrawDW_v(float i, float j);
void DrawDW_h(float i, float j);

void main()
{
	vec3 pixelValue;
	
	// Black pixels represent floor in the texture
	vec3 floor = vec3(0.0f, 0.0f, 0.0f);

	// CW = Corner Wall
	// SW = Single Wall
	// EW = End Wall
	// DW = Double Wall

	// n = north
	// s = south
	// e = east
	// w = west

	// v = vertical
	// h = horizontal

	// The following vectors are used to define the colors of the texture.
	// Each color represents a wall-type.

	// Single walls
	vec3 SW_n = vec3(100, 0, 255);
	vec3 SW_s = vec3(100, 255, 0);
	vec3 SW_e = vec3(100, 255, 255);
	vec3 SW_w = vec3(100, 0, 0);

	// Corner walls
	vec3 CW_ne = vec3(200, 0, 255);
	vec3 CW_nw = vec3(200, 255, 0);
	vec3 CW_sw = vec3(200, 0, 0);
	vec3 CW_se = vec3(200, 255, 255);

	// End walls
	vec3 EW_n = vec3(255, 0, 255);
	vec3 EW_s = vec3(255, 0, 0);
	vec3 EW_e = vec3(255, 255, 0);
	vec3 EW_w = vec3(255, 255, 255);

	// Double Walls
	vec3 DW_v = vec3(50, 0, 0);
	vec3 DW_h = vec3(50, 255, 255);

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

	// Draw floor in local space. (Move i and j so that origo is in the middle of the maze)
	i -= (width / 2);
	j -= (height / 2);

	// Draw the maze depending on the color of each pixel
	if(pixelValue == floor)
	{
		DrawFloor(i, j);
	}
	else if(pixelValue == SW_n)
	{
		DrawSW_n(i, j);
	}
	else if(pixelValue == SW_s)
	{
		DrawSW_s(i, j);
	}
	else if(pixelValue == SW_e)
	{
		DrawSW_e(i, j);
	}
	else if(pixelValue == SW_w)
	{
		DrawSW_w(i, j);
	}
	else if(pixelValue == CW_ne)
	{
		DrawCW_ne(i, j);
	}
	else if(pixelValue == CW_nw)
	{
		DrawCW_nw(i, j);
	}
	else if(pixelValue == CW_sw)
	{
		DrawCW_sw(i, j);
	}
	else if(pixelValue == CW_se)
	{
		DrawCW_se(i, j);
	}
	else if(pixelValue == EW_n)
	{
		DrawEW_n(i, j);
	}
	else if(pixelValue == EW_s)
	{
		DrawEW_s(i, j);
	}
	else if(pixelValue == EW_e)
	{
		DrawEW_e(i, j);
	}
	else if(pixelValue == EW_w)
	{
		DrawEW_w(i, j);
	}
	else if(pixelValue == DW_v)
	{
		DrawDW_v(i, j);
	}
	else if(pixelValue == DW_h)
	{
		DrawDW_h(i, j);
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
	outType = 0;
	EmitVertex();												  
			
	//Right Top Vertex											  
	outPosition = vec3(i + 0.5f, 0, j - 0.5);
	outTexCoords = vec2(scaleUVXZ, -scaleUVXZ);
	outNormal  = vec3(0.0f, 1.0f, 0.0f);
	outTangent = vec3(1.0f, 0.0f, 0.0f);
	outType = 0;
	EmitVertex();

	// Left Top Vertex											  
	outPosition = vec3(i - 0.5f, 0, j - 0.5);
	outTexCoords = vec2(0, -scaleUVXZ);
	outNormal  = vec3(0.0f, 1.0f, 0.0f);
	outTangent = vec3(1.0f, 0.0f, 0.0f);
	outType = 0;
	EmitVertex();												  		
	EndPrimitive();
		
		

	// Second Triangle
	// Left Bottom vertex
	outPosition = vec3(i - 0.5f, 0, j + 0.5);
	outTexCoords = vec2(0, 0);
	outNormal  = vec3(0.0f, 1.0f, 0.0f);
	outTangent = vec3(1.0f, 0.0f, 0.0f);
	outType = 0;
	EmitVertex();
		
	// Right Bottom vertex
	outPosition = vec3(i + 0.5f, 0, j + 0.5);
	outTexCoords = vec2(scaleUVXZ, 0);
	outNormal  = vec3(0.0f, 1.0f, 0.0f);
	outTangent = vec3(1.0f, 0.0f, 0.0f);
	outType = 0;
	EmitVertex();

	// Right Top vertex
	outPosition = vec3(i + 0.5f, 0, j - 0.5);
	outTexCoords = vec2(scaleUVXZ, -scaleUVXZ);
	outNormal  = vec3(0.0f, 1.0f, 0.0f);
	outTangent = vec3(1.0f, 0.0f, 0.0f);
	outType = 0;
	EmitVertex();		
	EndPrimitive();
}

// Single wall functions
void DrawSW_n(float i, float j)
{
	// First Triangle
	// Left Bottom vertex
	outPosition = vec3(i - 0.5f, 0, j - 0.5);
	outTexCoords = vec2(0, 0);
	outNormal  = vec3(0.0f, 0.0f, -1.0f);
	outTangent = vec3(-1.0f, 0.0f, 0.0f);
	outType = 1;
	EmitVertex();												  
																  
	// Left Top Vertex											  
	outPosition = vec3(i - 0.5f, 1, j - 0.5);
	outTexCoords = vec2(0, -scaleUVY);
	outNormal  = vec3(0.0f, 0.0f, -1.0f);
	outTangent = vec3(-1.0f, 0.0f, 0.0f);
	outType = 1;
	EmitVertex();												  
																  
	//Right Top Vertex											  
	outPosition = vec3(i + 0.5f, 1, j - 0.5);
	outTexCoords = vec2(-scaleUVXZ, -scaleUVY);
	outNormal  = vec3(0.0f, 0.0f, -1.0f);
	outTangent = vec3(-1.0f, 0.0f, 0.0f);
	outType = 1;
	EmitVertex();
				
	EndPrimitive();
		
		

	// Second Triangle
	// Left Bottom vertex
	outPosition = vec3(i - 0.5f, 0, j - 0.5);
	outTexCoords = vec2(0, 0);
	outNormal  = vec3(0.0f, 0.0f, -1.0f);
	outTangent = vec3(-1.0f, 0.0f, 0.0f);
	outType = 1;
	EmitVertex();
				
	// Right Top vertex
	outPosition = vec3(i + 0.5f, 1, j - 0.5);
	outTexCoords = vec2(-scaleUVXZ, -scaleUVY);
	outNormal  = vec3(0.0f, 0.0f, -1.0f);
	outTangent = vec3(-1.0f, 0.0f, 0.0f);
	outType = 1;
	EmitVertex();
				
	// Right Bottom vertex
	outPosition = vec3(i + 0.5f, 0, j - 0.5);
	outTexCoords = vec2(-scaleUVXZ, 0);
	outNormal  = vec3(0.0f, 0.0f, -1.0f);
	outTangent = vec3(-1.0f, 0.0f, 0.0f);
	outType = 1;
	EmitVertex();
				
	EndPrimitive();
}

void DrawSW_s(float i, float j)
{
	// First Triangle
	// Left Bottom vertex
	outPosition = vec3(i - 0.5, 0, j + 0.5);
	outTexCoords = vec2(0, 0);
	outNormal  = vec3(0.0f, 0.0f, 1.0f);
	outTangent = vec3(1.0f, 0.0f, 0.0f);
	outType = 1;
	EmitVertex();
	
	//Right Top Vertex
	outPosition = vec3(i + 0.5, 1, j + 0.5);
	outTexCoords = vec2(scaleUVXZ, -scaleUVY);
	outNormal  = vec3(0.0f, 0.0f, 1.0f);
	outTangent = vec3(1.0f, 0.0f, 0.0f);
	outType = 1;
	EmitVertex();

	// Left Top Vertex
	outPosition = vec3(i - 0.5, 1, j + 0.5);
	outTexCoords = vec2(0, -scaleUVY);
	outNormal  = vec3(0.0f, 0.0f, 1.0f);
	outTangent = vec3(1.0f, 0.0f, 0.0f);
	outType = 1;
	EmitVertex();
							
	EndPrimitive();


				
	// Second Triangle
	// Left Bottom vertex
	outPosition = vec3(i - 0.5, 0, j + 0.5);
	outTexCoords = vec2(0, 0);
	outNormal  = vec3(0.0f, 0.0f, 1.0f);
	outTangent = vec3(1.0f, 0.0f, 0.0f);
	outType = 1;
	EmitVertex();
	
	// Right Bottom vertex
	outPosition = vec3(i + 0.5, 0, j + 0.5);
	outTexCoords = vec2(scaleUVXZ, 0);
	outNormal  = vec3(0.0f, 0.0f, 1.0f);
	outTangent = vec3(1.0f, 0.0f, 0.0f);
	outType = 1;
	EmitVertex();

	// Right Top vertex
	outPosition = vec3(i + 0.5, 1, j + 0.5);
	outTexCoords = vec2(scaleUVXZ, -scaleUVY);
	outNormal  = vec3(0.0f, 0.0f, 1.0f);
	outTangent = vec3(1.0f, 0.0f, 0.0f);
	outType = 1;
	EmitVertex();
				
	EndPrimitive();
}

void DrawSW_e(float i, float j)
{
	// First Triangle
	// Left Bottom vertex
	outPosition = vec3(i + 0.5, 0, j - 0.5);
	outTexCoords = vec2(0, 0);
	outNormal  = vec3(1.0f, 0.0f, 0.0f);
	outTangent = vec3(0.0f, 0.0f, -1.0f);
	outType = 1;
	EmitVertex();
	
	// Left Top Vertex
	outPosition = vec3(i + 0.5, 1, j - 0.5);
	outTexCoords = vec2(0, -scaleUVY);
	outNormal  = vec3(1.0f, 0.0f, 0.0f);
	outTangent = vec3(0.0f, 0.0f, -1.0f);
	outType = 1;
	EmitVertex();

	//Right Top Vertex
	outPosition = vec3(i + 0.5, 1, j + 0.5);
	outTexCoords = vec2(-scaleUVXZ, -scaleUVY);
	outNormal  = vec3(1.0f, 0.0f, 0.0f);
	outTangent = vec3(0.0f, 0.0f, -1.0f);
	outType = 1;
	EmitVertex();
						
	EndPrimitive();


				
	// Second Triangle
	// Left Bottom vertex
	outPosition = vec3(i + 0.5, 0, j - 0.5);
	outTexCoords = vec2(0, 0);
	outNormal  = vec3(1.0f, 0.0f, 0.0f);
	outTangent = vec3(0.0f, 0.0f, -1.0f);
	outType = 1;
	EmitVertex();
	
	// Right Top vertex
	outPosition = vec3(i + 0.5, 1, j + 0.5);
	outTexCoords = vec2(-scaleUVXZ, -scaleUVY);
	outNormal  = vec3(1.0f, 0.0f, 0.0f);
	outTangent = vec3(0.0f, 0.0f, -1.0f);
	outType = 1;
	EmitVertex();

	// Right Bottom vertex
	outPosition = vec3(i + 0.5, 0, j + 0.5);
	outTexCoords = vec2(-scaleUVXZ, 0);
	outNormal  = vec3(1.0f, 0.0f, 0.0f);
	outTangent = vec3(0.0f, 0.0f, -1.0f);
	outType = 1;
	EmitVertex();
		
	EndPrimitive();
}

void DrawSW_w(float i, float j)
{
	// First Triangle
	// Left Bottom vertex
	outPosition = vec3(i - 0.5, 0, j + 0.5);
	outTexCoords = vec2(0, 0);
	outNormal  = vec3(-1.0f, 0.0f, 0.0f);
	outTangent = vec3(0.0f, 0.0f, 1.0f);
	outType = 1;
	EmitVertex();
	
	// Left Top Vertex
	outPosition = vec3(i - 0.5, 1, j + 0.5);
	outTexCoords = vec2(0, -scaleUVY);
	outNormal  = vec3(-1.0f, 0.0f, 0.0f);
	outTangent = vec3(0.0f, 0.0f, 1.0f);
	outType = 1;
	EmitVertex();

	//Right Top Vertex
	outPosition = vec3(i - 0.5, 1, j - 0.5);
	outTexCoords = vec2(-scaleUVXZ, -scaleUVY);
	outNormal  = vec3(-1.0f, 0.0f, 0.0f);
	outTangent = vec3(0.0f, 0.0f, 1.0f);
	outType = 1;
	EmitVertex();
						
	EndPrimitive();


				
	// Second Triangle
	// Left Bottom vertex
	outPosition = vec3(i - 0.5, 0, j + 0.5);
	outTexCoords = vec2(0, 0);
	outNormal  = vec3(-1.0f, 0.0f, 0.0f);
	outTangent = vec3(0.0f, 0.0f, 1.0f);
	outType = 1;
	EmitVertex();
	
	// Right Top vertex
	outPosition = vec3(i - 0.5, 1, j - 0.5);
	outTexCoords = vec2(-scaleUVXZ, -scaleUVY);
	outNormal  = vec3(-1.0f, 0.0f, 0.0f);
	outTangent = vec3(0.0f, 0.0f, 1.0f);
	outType = 1;
	EmitVertex();

	// Right Bottom vertex
	outPosition = vec3(i - 0.5, 0, j - 0.5);
	outTexCoords = vec2(-scaleUVXZ, 0);
	outNormal  = vec3(-1.0f, 0.0f, 0.0f);
	outTangent = vec3(0.0f, 0.0f, 1.0f);
	outType = 1;
	EmitVertex();
				
	EndPrimitive();
}


// Corner wall functions
void DrawCW_ne(float i,float j)
{
	DrawSW_n(i, j);
	DrawSW_e(i, j);
}
void DrawCW_nw(float i,float j)
{
	DrawSW_n(i, j);
	DrawSW_w(i, j);
}
void DrawCW_sw(float i,float j)
{
	DrawSW_s(i, j);
	DrawSW_w(i, j);
}
void DrawCW_se(float i,float j)
{
	DrawSW_s(i, j);
	DrawSW_e(i, j);
}


// End wall functions
void DrawEW_n(float i,float j)
{
	DrawSW_e(i, j);
	DrawSW_n(i, j);
	DrawSW_w(i, j);
}
void DrawEW_s(float i,float j)
{
	DrawSW_e(i, j);
	DrawSW_s(i, j);
	DrawSW_w(i, j);
}
void DrawEW_e(float i,float j)
{
	DrawSW_e(i, j);
	DrawSW_n(i, j);
	DrawSW_s(i, j);
}
void DrawEW_w(float i,float j)
{
	DrawSW_w(i, j);
	DrawSW_n(i, j);
	DrawSW_s(i, j);
}


// Double wall functions
void DrawDW_v(float i,float j)
{
	DrawSW_e(i, j);
	DrawSW_w(i, j);
}
void DrawDW_h(float i,float j)
{
	DrawSW_n(i, j);
	DrawSW_s(i, j);
}