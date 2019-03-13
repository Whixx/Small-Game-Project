#include "ClipSpaceQuad.h"

ClipSpaceQuad::ClipSpaceQuad(glm::vec2 point, 
	float width,
	float height,
	bool canBePressed, 
	string path)
{
	this->canBePressed = canBePressed;

	this->point = point;
	this->width = width;
	this->height = height;

	this->canBePressed = canBePressed;

	this->CreateScreenQuad(point, width, height);

	this->texture = Texture(path, "TextureDiffuse", false);
}

ClipSpaceQuad::~ClipSpaceQuad()
{
}

void ClipSpaceQuad::BindTexture()
{
	this->texture.Bind(0);
}

void ClipSpaceQuad::Draw()
{
	glBindVertexArray(this->vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

bool ClipSpaceQuad::IsMouseInQuad(GLFWwindow* window)
{
	// Check if the quad can be pressed
	if (this->canBePressed == false)
		return false;

	double leftSide = this->point.x - this->width;
	double rightSide = this->point.x + this->width;
	double botSide = this->point.y - this->height;
	double topSide = this->point.y + this->height;

	bool isInQuad = false;

	// Get mouse position
	double mousePosX;
	double mousePosY;
	glfwGetCursorPos(window, &mousePosX, &mousePosY);
	
	// Transform mousePos to screenSpace
	mousePosX = (mousePosX / (SCREEN_WIDTH/2)) - 1;
	mousePosY = (mousePosY / (SCREEN_HEIGHT/2)) - 1;
	
	// Since we have flipped the clip space
	mousePosY *= -1;

	
	// Check leftside of the quad
	if (mousePosX > leftSide)
	{
		// Check rightside of the quad
		if (mousePosX < rightSide)
		{
			// Check botside of the quad
			if (mousePosY > botSide)
			{
				// Check topside of the quad
				if (mousePosY < topSide)
				{
					isInQuad = true;
					std::cout << "Mouse is inside quad!" << std::endl;
				}
			}
		}
	}
	return isInQuad;
}

void ClipSpaceQuad::CreateScreenQuad(glm::vec2 point, float width, float height)
{
	float fullScreenQuadData[] =
	{
		point.x - width, point.y - height, 0.0, 0.0, 0.0,		// Bottom left
		point.x + width, point.y + height, 0.0, 1.0, 1.0,		// Top right
		point.x + width, point.y - height, 0.0, 1.0, 0.0,		// Bottom Right
				 				   
		point.x - width, point.y - height, 0.0, 0.0, 0.0,		// Bottom left
		point.x - width, point.y + height, 0.0, 0.0, 1.0,		// Top Left
		point.x + width, point.y + height, 0.0, 1.0, 1.0,		// Top Right
	};

	//float fullScreenQuadData[] =
	//{
	//	bottomLeftCorner.x, bottomLeftCorner.y, 0.0, 0.0, 0.0,		// Bottom left
	//	topRightCorner.x, topRightCorner.y, 0.0, 1.0, 1.0,			// Top right
	//	bottomRightCorner.x, bottomRightCorner.y, 0.0, 1.0, 0.0,	// Bottom Right
	//
	//	bottomLeftCorner.x, bottomLeftCorner.y, 0.0, 0.0, 0.0,		// Bottom left
	//	topLeftCorner.x, topLeftCorner.y, 0.0, 0.0, 1.0,			// Top Left
	//	topRightCorner.x, topRightCorner.y, 0.0, 1.0, 1.0			// Top Right
	//};

	glGenVertexArrays(1, &this->vao);
	glBindVertexArray(this->vao);

	GLuint quadBuffer;
	glGenBuffers(1, &quadBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, quadBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 5 * 6, &fullScreenQuadData[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(float) * 5, 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(float) * 5, (void*)(sizeof(float) * 3));

	glBindVertexArray(0);
}
