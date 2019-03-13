#include "ButtonHandler.h"



ButtonHandler::ButtonHandler()
{
}


ButtonHandler::~ButtonHandler()
{
}

void ButtonHandler::AddButton(	glm::vec2 point,
								float width,
								float height,
								string path)
{
	this->quads.push_back(ClipSpaceQuad(point, width, height, true, path));
}

unsigned int ButtonHandler::GetNrOfQuads()
{
	return this->quads.size();
}

bool ButtonHandler::IsQuadPressed(GLFWwindow* window, unsigned int ID)
{
	return this->quads.at(ID).IsMouseInQuad(window);
}

void ButtonHandler::DrawQuads()
{
	for (int i = 0; i < this->quads.size(); i++)
	{
		// Bind Texture
		this->quads.at(i).BindTexture();

		// Draw Quad
		this->quads.at(i).Draw();
	}
}
