#include "Display.h"
#include <glew/glew.h>
#include <iostream>

Display::Display()
{
	if (!glfwInit())
		exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	this->window = glfwCreateWindow(SCREENWIDTH, SCREENHEIGHT, "Open GL Project", NULL, NULL);

	if (!this->window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(this->window);
	glfwSwapInterval(1);

	// Glew init behövs för att glCreateProgram ska funka
	GLenum status = glewInit();

	if (status != GLEW_OK)
	{
		std::cerr << "Glew failed to initialize!" << endl;
	}
}

void Display::SwapBuffers(int width, int height)
{
	glfwGetFramebufferSize(window, &width, &height);

	glfwSwapBuffers(window);
	glfwPollEvents();
}

bool Display::IsWindowClosed()
{
	return glfwWindowShouldClose(this->window);
}

void Display::SetTitle(string newTitle)
{
	glfwSetWindowTitle(this->window, newTitle.c_str());
}

GLFWwindow* Display::GetWindow()
{
	return this->window;
}

Display::~Display()
{
	glfwDestroyWindow(this->window);
	glfwTerminate;
	exit(EXIT_SUCCESS);
}

