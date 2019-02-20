#version 440

layout (points) in;
layout (triangle_strip, max_vertices = 18) out;

out vec3 outPosition;
out vec2 outTexCoords;

uniform sampler2D texture;

uniform int width;
uniform int height;
uniform int scaleUVX;
uniform int scaleUVY;

// Single wall functions
void drawSW_n(float i, float j);
void drawSW_s(float i, float j);
void drawSW_e(float i, float j);
void drawSW_w(float i, float j);

// Corner wall functions
void drawCW_ne(float i, float j);
void drawCW_nw(float i, float j);
void drawCW_sw(float i, float j);
void drawCW_se(float i, float j);

// End wall functions
void drawEW_n(float i, float j);
void drawEW_s(float i, float j);
void drawEW_e(float i, float j);
void drawEW_w(float i, float j);

// Double wall functions
void drawDW_v(float i, float j);
void drawDW_h(float i, float j);

void main()
{
	vec3 pixelValue;
	
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
	float i = int(pointPosition.x);
	float j = int(pointPosition.y);

	// Find the pixelColor
	pixelValue = texelFetch(texture, ivec2(i,j), 0).rgb;
	pixelValue *= 255;

	pixelValue.r = floor(pixelValue.r);
	pixelValue.g = floor(pixelValue.g);
	pixelValue.b = floor(pixelValue.b);

	// Draw floor in local space. (Move i and j so that origo is in the middle of the maze)
	i -= (width / 2);
	j -= (height / 2);

	// Draw walls depending on the color of each pixel
	if(pixelValue == SW_n)
	{
		drawSW_n(i, j);
	}
	else if(pixelValue == SW_s)
	{
		drawSW_s(i, j);
	}
	else if(pixelValue == SW_e)
	{
		drawSW_e(i, j);
	}
	else if(pixelValue == SW_w)
	{
		drawSW_w(i, j);
	}
	else if(pixelValue == CW_ne)
	{
		drawCW_ne(i, j);
	}
	else if(pixelValue == CW_nw)
	{
		drawCW_nw(i, j);
	}
	else if(pixelValue == CW_sw)
	{
		drawCW_sw(i, j);
	}
	else if(pixelValue == CW_se)
	{
		drawCW_se(i, j);
	}
	else if(pixelValue == EW_n)
	{
		drawEW_n(i, j);
	}
	else if(pixelValue == EW_s)
	{
		drawEW_s(i, j);
	}
	else if(pixelValue == EW_e)
	{
		drawEW_e(i, j);
	}
	else if(pixelValue == EW_w)
	{
		drawEW_w(i, j);
	}
	else if(pixelValue == DW_v)
	{
		drawDW_v(i, j);
	}
	else if(pixelValue == DW_h)
	{
		drawDW_h(i, j);
	}
}
		 
// Single wall functions
void drawSW_n(float i, float j)
{
	// First Triangle
	// Left Bottom vertex
	outPosition = vec3(i - 0.5f, 0, j - 0.5);
	outTexCoords = vec2(0, 0);
	EmitVertex();												  
																  
	// Left Top Vertex											  
	outPosition = vec3(i - 0.5f, 1, j - 0.5);
	outTexCoords = vec2(0, scaleUVY);
	EmitVertex();												  
																  
	//Right Top Vertex											  
	outPosition = vec3(i + 0.5f, 1, j - 0.5);
	outTexCoords = vec2(scaleUVX, scaleUVY);
	EmitVertex();
				
	EndPrimitive();
		
		

	// Second Triangle
	// Left Bottom vertex
	outPosition = vec3(i - 0.5f, 0, j - 0.5);
	outTexCoords = vec2(0, 0);
	EmitVertex();
				
	// Right Top vertex
	outPosition = vec3(i + 0.5f, 1, j - 0.5);
	outTexCoords = vec2(scaleUVX, scaleUVY);
	EmitVertex();
				
	// Right Bottom vertex
	outPosition = vec3(i + 0.5f, 0, j - 0.5);
	outTexCoords = vec2(scaleUVX, 0);
	EmitVertex();
				
	EndPrimitive();
}

void drawSW_s(float i, float j)
{
	// First Triangle
	// Left Bottom vertex
	outPosition = vec3(i - 0.5, 0, j + 0.5);
	outTexCoords = vec2(0, 0);
	EmitVertex();
	
	//Right Top Vertex
	outPosition = vec3(i + 0.5, 1, j + 0.5);
	outTexCoords = vec2(scaleUVX, scaleUVY);
	EmitVertex();

	// Left Top Vertex
	outPosition = vec3(i - 0.5, 1, j + 0.5);
	outTexCoords = vec2(0, scaleUVY);
	EmitVertex();
							
	EndPrimitive();


				
	// Second Triangle
	// Left Bottom vertex
	outPosition = vec3(i - 0.5, 0, j + 0.5);
	outTexCoords = vec2(0, 0);
	EmitVertex();
	
	// Right Bottom vertex
	outPosition = vec3(i + 0.5, 0, j + 0.5);
	outTexCoords = vec2(scaleUVX, 0);
	EmitVertex();

	// Right Top vertex
	outPosition = vec3(i + 0.5, 1, j + 0.5);
	outTexCoords = vec2(scaleUVX, scaleUVY);
	EmitVertex();
				
	EndPrimitive();
}

void drawSW_e(float i, float j)
{
	// First Triangle
	// Left Bottom vertex
	outPosition = vec3(i + 0.5, 0, j - 0.5);
	outTexCoords = vec2(0, 0);
	EmitVertex();
	
	// Left Top Vertex
	outPosition = vec3(i + 0.5, 1, j - 0.5);
	outTexCoords = vec2(0, scaleUVY);
	EmitVertex();

	//Right Top Vertex
	outPosition = vec3(i + 0.5, 1, j + 0.5);
	outTexCoords = vec2(scaleUVX, scaleUVY);
	EmitVertex();
						
	EndPrimitive();


				
	// Second Triangle
	// Left Bottom vertex
	outPosition = vec3(i + 0.5, 0, j - 0.5);
	outTexCoords = vec2(0, 0);
	EmitVertex();
	
	// Right Top vertex
	outPosition = vec3(i + 0.5, 1, j + 0.5);
	outTexCoords = vec2(scaleUVX, scaleUVY);
	EmitVertex();

	// Right Bottom vertex
	outPosition = vec3(i + 0.5, 0, j + 0.5);
	outTexCoords = vec2(scaleUVX, 0);
	EmitVertex();
		
	EndPrimitive();
}

void drawSW_w(float i, float j)
{
	// First Triangle
	// Left Bottom vertex
	outPosition = vec3(i - 0.5, 0, j + 0.5);
	outTexCoords = vec2(0, 0);
	EmitVertex();
	
	// Left Top Vertex
	outPosition = vec3(i - 0.5, 1, j + 0.5);
	outTexCoords = vec2(0, scaleUVY);
	EmitVertex();

	//Right Top Vertex
	outPosition = vec3(i - 0.5, 1, j - 0.5);
	outTexCoords = vec2(scaleUVX, scaleUVY);
	EmitVertex();
						
	EndPrimitive();


				
	// Second Triangle
	// Left Bottom vertex
	outPosition = vec3(i - 0.5, 0, j + 0.5);
	outTexCoords = vec2(0, 0);
	EmitVertex();
	
	// Right Top vertex
	outPosition = vec3(i - 0.5, 1, j - 0.5);
	outTexCoords = vec2(scaleUVX, scaleUVY);
	EmitVertex();

	// Right Bottom vertex
	outPosition = vec3(i - 0.5, 0, j - 0.5);
	outTexCoords = vec2(scaleUVX, 0);
	EmitVertex();
				
	EndPrimitive();
}


// Corner wall functions
void drawCW_ne(float i,float j)
{
	drawSW_n(i, j);
	drawSW_e(i, j);
}
void drawCW_nw(float i,float j)
{
	drawSW_n(i, j);
	drawSW_w(i, j);
}
void drawCW_sw(float i,float j)
{
	drawSW_s(i, j);
	drawSW_w(i, j);
}
void drawCW_se(float i,float j)
{
	drawSW_s(i, j);
	drawSW_e(i, j);
}


// End wall functions
void drawEW_n(float i,float j)
{
	drawSW_e(i, j);
	drawSW_n(i, j);
	drawSW_w(i, j);
}
void drawEW_s(float i,float j)
{
	drawSW_e(i, j);
	drawSW_s(i, j);
	drawSW_w(i, j);
}
void drawEW_e(float i,float j)
{
	drawSW_e(i, j);
	drawSW_n(i, j);
	drawSW_s(i, j);
}
void drawEW_w(float i,float j)
{
	drawSW_w(i, j);
	drawSW_n(i, j);
	drawSW_s(i, j);
}


// Double wall functions
void drawDW_v(float i,float j)
{
	drawSW_e(i, j);
	drawSW_w(i, j);
}
void drawDW_h(float i,float j)
{
	drawSW_n(i, j);
	drawSW_s(i, j);
}