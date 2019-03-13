#ifndef SCREENSPACEQUAD_H
#define SCREENSPACEQUAD_H

#include "glm/glm.hpp"
#include <glew\glew.h>
#include <iostream>
#include "MaterialHandler.h"

// To handle mouse
#include <GLFW/glfw3.h>
#include "Display.h"

class ClipSpaceQuad
{
public:
	ClipSpaceQuad(	glm::vec2 point = glm::vec2(0.0f, 0.0f),
					float width = 1,
					float height = 1,
					bool canBePressed = false,
					string path = "");

	~ClipSpaceQuad();

	void BindTexture();
	void Draw();
	bool IsMouseInQuad(GLFWwindow* window);
private:
	GLuint vao;
	bool canBePressed;

	glm::vec2 point;
	float width;
	float height;

	Texture* texture;

	void CreateScreenQuad(glm::vec2 point, float width, float height);
};

#endif