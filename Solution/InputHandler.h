#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include "glew/glew.h"
#include <GLFW/glfw3.h>
#include "Display.h"
#include "Camera.h"

class InputHandler
{
public:
	InputHandler();
	virtual ~InputHandler();

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void mouseControls(Display *display, Camera *camera);
	void keyboardControls(Display *display, Camera *camera);
private:
	static bool mouseLock;
};

#endif