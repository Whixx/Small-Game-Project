#ifndef SCREENSPACEQUAD_H
#define SCREENSPACEQUAD_H

#include "glm/glm.hpp"
#include <glew\glew.h>
#include <iostream>
#include "Texture.h"

// To handle mouse
#include <GLFW/glfw3.h>
#include "Display.h"

enum MENU_TYPE
{
	MENU_START,
	MENU_INGAME,
	INVALID	
};


class ClipSpaceQuad
{
public:
	ClipSpaceQuad(	glm::vec2 point = glm::vec2(0.0f, 0.0f),
					float width = 1,
					float height = 1,
					bool canBePressed = false,
					string path = "",
					MENU_TYPE = INVALID);

	~ClipSpaceQuad();

	void BindTexture();
	void Draw();
	bool IsMouseInQuad(GLFWwindow* window);

	MENU_TYPE GetType();
private:
	GLuint vao;
	bool canBePressed;
	MENU_TYPE type;

	glm::vec2 point;
	float width;
	float height;

	Texture texture;

	void CreateScreenQuad(glm::vec2 point, float width, float height);
};

#endif