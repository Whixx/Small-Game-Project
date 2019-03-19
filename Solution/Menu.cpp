#include "Menu.h"



Menu::Menu()
{
	// Definera texturer
}


Menu::~Menu()
{
}

unsigned int Menu::AddButton(	glm::vec2 point,
								float width,
								float height,
								string path,
								MENU_TYPE button)
{
	this->quads.push_back(ClipSpaceQuad(point, width, height, true, path, button));
	return this->quads.size() - 1;
}

unsigned int Menu::GetNrOfQuads()
{
	return this->quads.size();
}

bool Menu::IsQuadPressed(GLFWwindow* window, unsigned int ID)
{
	return this->quads.at(ID).IsMouseInQuad(window);
}

void Menu::DrawQuads(MENU_TYPE menuType)
{
	for (int i = 0; i < this->quads.size(); i++)
	{
		if (this->quads.at(i).GetType() == menuType)
		{
			// Bind Texture
			this->quads.at(i).BindTexture();

			// Draw Quad
			this->quads.at(i).Draw();
		}
	}
}
