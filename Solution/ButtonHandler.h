#ifndef CLIPSPACEQUADHANDLER_H
#define CLIPSPACEQUADHANDLER_H

#include "ClipSpaceQuad.h"
#include <vector>

enum Buttons
{
	BUTTON_START,
	BUTTON_QUIT,
	NR_OF_BUTTONS
};


class ButtonHandler
{
public:
	ButtonHandler();
	~ButtonHandler();

	void AddButton(	glm::vec2 point,
					float width,
					float height,
					string path);

	unsigned int GetNrOfQuads();

	bool IsQuadPressed(GLFWwindow* window, unsigned int ID);

	void DrawQuads();

private:
	std::vector<ClipSpaceQuad> quads;

};

#endif