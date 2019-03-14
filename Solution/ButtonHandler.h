#ifndef CLIPSPACEQUADHANDLER_H
#define CLIPSPACEQUADHANDLER_H

#include "ClipSpaceQuad.h"
#include <vector>

class ButtonHandler
{
public:
	ButtonHandler();
	~ButtonHandler();

	unsigned int AddButton(	glm::vec2 point,
					float width,
					float height,
					string path,
					MENU_TYPE button);

	unsigned int GetNrOfQuads();

	bool IsQuadPressed(GLFWwindow* window, unsigned int ID);

	void DrawQuads(MENU_TYPE menuType);

private:
	std::vector<ClipSpaceQuad> quads;

};

#endif