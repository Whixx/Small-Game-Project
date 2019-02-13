#version 440

layout (points) in;
layout (triangle_strip, max_vertices=18) out;


out vec3 testOut;

uniform sampler2D texture;

//uniform vec3 cameraPos;

//uniform mat4 viewProjection; 

// Single wall functions
void drawSW_n(float i, float j, vec3 colors);
void drawSW_s(float i,float j, vec3 colors);
void drawSW_e(float i,float j, vec3 colors);
void drawSW_w(float i,float j, vec3 colors);

// Corner wall functions
void drawCW_ne(float i,float j, vec3 colors);
void drawCW_nw(float i,float j, vec3 colors);
void drawCW_sw(float i,float j, vec3 colors);
void drawCW_se(float i,float j, vec3 colors);

// End wall functions
void drawEW_n(float i,float j, vec3 colors);
void drawEW_s(float i,float j, vec3 colors);
void drawEW_e(float i,float j, vec3 colors);
void drawEW_w(float i,float j, vec3 colors);

// Double wall functions
void drawDW_v(float i,float j, vec3 colors);
void drawDW_h(float i,float j, vec3 colors);

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

	// These colors have one wall each
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

	vec4 pointPosition = gl_in[0].gl_Position;
	int i = int(pointPosition.x);
	int j = int(pointPosition.y);

	// Find the pixelColor
	pixelValue = texelFetch(texture, ivec2(i,j), 0).rgb;
	pixelValue *= 255;

	pixelValue.r = floor(pixelValue.r);
	pixelValue.g = floor(pixelValue.g);
	pixelValue.b = floor(pixelValue.b);

	// Draw walls depending on the color of each pixel
	if(pixelValue == SW_n)
	{
		drawSW_n(i, j, pixelValue);
	}
	else if(pixelValue == SW_s)
	{
		drawSW_s(i, j, pixelValue);
	}
	else if(pixelValue == SW_e)
	{
		drawSW_e(i, j, pixelValue);
	}
	else if(pixelValue == SW_w)
	{
		drawSW_w(i, j, pixelValue);
	}
	else if(pixelValue == CW_ne)
	{
		drawCW_ne(i, j, pixelValue);
	}
	else if(pixelValue == CW_nw)
	{
		drawCW_nw(i, j, pixelValue);
	}
	else if(pixelValue == CW_sw)
	{
		drawCW_sw(i, j, pixelValue);
	}
	else if(pixelValue == CW_se)
	{
		drawCW_se(i, j, pixelValue);
	}
	else if(pixelValue == EW_n)
	{
		drawEW_n(i, j, pixelValue);
	}
	else if(pixelValue == EW_s)
	{
		drawEW_s(i, j, pixelValue);
	}
	else if(pixelValue == EW_e)
	{
		drawEW_e(i, j, pixelValue);
	}
	else if(pixelValue == EW_w)
	{
		drawEW_w(i, j, pixelValue);
	}
	else if(pixelValue == DW_v)
	{
		drawDW_v(i, j, pixelValue);
	}
	else if(pixelValue == DW_h)
	{
		drawDW_h(i, j, pixelValue);
	}

}
		 
// Single wall functions
void drawSW_n(float i, float j, vec3 colors)
{
	vec4 tempVec;

	// First Triangle
	// Left Bottom vertex
	tempVec = vec4(i - 0.5f, 0, j - 0.5, 1.0f);
	testOut = vec3(tempVec.xyz);
	EmitVertex();												  
																  
	// Left Top Vertex											  
	tempVec = vec4(i - 0.5f, 1, j - 0.5, 1.0f);
	testOut = vec3(tempVec.xyz);
	EmitVertex();												  
																  
	//Right Top Vertex											  
	tempVec = vec4(i + 0.5f, 1, j - 0.5, 1.0f);
	testOut = vec3(tempVec.xyz);
	EmitVertex();
				
	EndPrimitive();
		
		

	// Second Triangle
	// Left Bottom vertex
	tempVec = vec4(i - 0.5f, 0, j - 0.5, 1.0f);
	testOut = vec3(tempVec.xyz);
	EmitVertex();
				
	// Right Top vertex
	tempVec = vec4(i + 0.5f, 1, j - 0.5, 1.0f);
	testOut = vec3(tempVec.xyz);
	EmitVertex();
				
	// Right Bottom vertex
	tempVec = vec4(i + 0.5f, 0, j - 0.5, 1.0f);
	testOut = vec3(tempVec.xyz);
	EmitVertex();
				
	EndPrimitive();
}

void drawSW_s(float i, float j, vec3 colors)
{
	vec4 tempVec;

	// First Triangle
	// Left Bottom vertex
	tempVec = vec4(i - 0.5, 0, j + 0.5, 1.0f);
	testOut = vec3(tempVec.xyz);
	EmitVertex();
	
	//Right Top Vertex
	tempVec = vec4(i + 0.5, 1, j + 0.5, 1.0f);
	testOut = vec3(tempVec.xyz);
	EmitVertex();

	// Left Top Vertex
	tempVec = vec4(i - 0.5, 1, j + 0.5, 1.0f);
	testOut = vec3(tempVec.xyz);
	EmitVertex();
							
	EndPrimitive();


				
	// Second Triangle
	// Left Bottom vertex
	tempVec = vec4(i - 0.5, 0, j + 0.5, 1.0f);
	testOut = vec3(tempVec.xyz);
	EmitVertex();
	
	// Right Bottom vertex
	tempVec = vec4(i + 0.5, 0, j + 0.5, 1.0f);
	testOut = vec3(tempVec.xyz);
	EmitVertex();

	// Right Top vertex
	tempVec = vec4(i + 0.5, 1, j + 0.5, 1.0f);
	testOut = vec3(tempVec.xyz);
	EmitVertex();
				
	EndPrimitive();
}

void drawSW_e(float i, float j, vec3 colors)
{
	vec4 tempVec;

	// First Triangle
	// Left Bottom vertex
	tempVec = vec4(i + 0.5, 0, j - 0.5, 1.0f);
	testOut = vec3(tempVec.xyz);
	EmitVertex();
	
	// Left Top Vertex
	tempVec = vec4(i + 0.5, 1, j - 0.5, 1.0f);
	testOut = vec3(tempVec.xyz);
	EmitVertex();

	//Right Top Vertex
	tempVec = vec4(i + 0.5, 1, j + 0.5, 1.0f);
	testOut = vec3(tempVec.xyz);
	EmitVertex();
						
	EndPrimitive();


				
	// Second Triangle
	// Left Bottom vertex
	tempVec = vec4(i + 0.5, 0, j - 0.5, 1.0f);
	testOut = vec3(tempVec.xyz);
	EmitVertex();
	
	// Right Top vertex
	tempVec = vec4(i + 0.5, 1, j + 0.5, 1.0f);
	testOut = vec3(tempVec.xyz);
	EmitVertex();

	// Right Bottom vertex
	tempVec = vec4(i + 0.5, 0, j + 0.5, 1.0f);
	testOut = vec3(tempVec.xyz);
	EmitVertex();
		
	EndPrimitive();
}

void drawSW_w(float i, float j, vec3 colors)
{
	vec4 tempVec;

	// First Triangle
	// Left Bottom vertex
	tempVec = vec4(i - 0.5, 0, j + 0.5, 1.0f);
	testOut = vec3(tempVec.xyz);
	EmitVertex();
	
	// Left Top Vertex
	tempVec = vec4(i - 0.5, 1, j + 0.5, 1.0f);
	testOut = vec3(tempVec.xyz);
	EmitVertex();

	//Right Top Vertex
	tempVec = vec4(i - 0.5, 1, j - 0.5, 1.0f);
	testOut = vec3(tempVec.xyz);
	EmitVertex();
						
	EndPrimitive();


				
	// Second Triangle
	// Left Bottom vertex
	tempVec = vec4(i - 0.5, 0, j + 0.5, 1.0f);
	testOut = vec3(tempVec.xyz);
	EmitVertex();
	
	// Right Top vertex
	tempVec = vec4(i - 0.5, 1, j - 0.5, 1.0f);
	testOut = vec3(tempVec.xyz);
	EmitVertex();

	// Right Bottom vertex
	tempVec = vec4(i - 0.5, 0, j - 0.5, 1.0f);
	testOut = vec3(tempVec.xyz);
	EmitVertex();
				
	EndPrimitive();
}


// Corner wall functions
void drawCW_ne(float i,float j, vec3 colors)
{
	drawSW_n(i, j, colors);
	drawSW_e(i, j, colors);
}
void drawCW_nw(float i,float j, vec3 colors)
{
	drawSW_n(i, j, colors);
	drawSW_w(i, j, colors);
}
void drawCW_sw(float i,float j, vec3 colors)
{
	drawSW_s(i, j, colors);
	drawSW_w(i, j, colors);
}
void drawCW_se(float i,float j, vec3 colors)
{
	drawSW_s(i, j, colors);
	drawSW_e(i, j, colors);
}


// End wall functions
void drawEW_n(float i,float j, vec3 colors)
{
	drawSW_e(i, j, colors);
	drawSW_n(i, j, colors);
	drawSW_w(i, j, colors);
}
void drawEW_s(float i,float j, vec3 colors)
{
	drawSW_e(i, j, colors);
	drawSW_s(i, j, colors);
	drawSW_w(i, j, colors);
}
void drawEW_e(float i,float j, vec3 colors)
{
	drawSW_e(i, j, colors);
	drawSW_n(i, j, colors);
	drawSW_s(i, j, colors);
}
void drawEW_w(float i,float j, vec3 colors)
{
	drawSW_w(i, j, colors);
	drawSW_n(i, j, colors);
	drawSW_s(i, j, colors);
}


// Double wall functions
void drawDW_v(float i,float j, vec3 colors)
{
	drawSW_e(i, j, colors);
	drawSW_w(i, j, colors);
}
void drawDW_h(float i,float j, vec3 colors)
{
	drawSW_n(i, j, colors);
	drawSW_s(i, j, colors);
}