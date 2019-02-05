#include "InputHandler.h"



InputHandler::InputHandler()
{
}


InputHandler::~InputHandler()
{
}

void InputHandler::key_callback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_E && action == GLFW_PRESS)
	{
		mouseLock = !mouseLock;
		if (mouseLock)
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		else
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

void InputHandler::mouseControls(Display * display, Camera * camera, float elapsedTime)
{
	double mouseXpos;
	double mouseYpos;

	// Find mouseposition (This function updates the X,Y values of the mouse position.
	glfwGetCursorPos(display->GetWindow(), &mouseXpos, &mouseYpos);
	if (mouseLock)
	{
		camera->updateCamera(glm::vec2(mouseXpos, mouseYpos), elapsedTime);
	}
}

void InputHandler::keyboardControls(Display * display, Camera * camera, float elapsedTime)
{
	int keyboardButton;
	// Check for keyboard inputs, used to move the camera around.
	// WASD for movearound, RF (Rise,Fall) and space to set the initial camera position & viewDir.
	keyboardButton = glfwGetKey(display->GetWindow(), GLFW_KEY_W);
	if (keyboardButton == GLFW_PRESS)
	{
		camera->moveForward(elapsedTime);
	}
	keyboardButton = glfwGetKey(display->GetWindow(), GLFW_KEY_S);
	if (keyboardButton == GLFW_PRESS)
	{
		camera->moveBackward(elapsedTime);
	}
	keyboardButton = glfwGetKey(display->GetWindow(), GLFW_KEY_D);
	if (keyboardButton == GLFW_PRESS)
	{
		camera->moveRight(elapsedTime);
	}
	keyboardButton = glfwGetKey(display->GetWindow(), GLFW_KEY_A);
	if (keyboardButton == GLFW_PRESS)
	{
		camera->moveLeft(elapsedTime);
	}
	keyboardButton = glfwGetKey(display->GetWindow(), GLFW_KEY_R);
	if (keyboardButton == GLFW_PRESS)
	{
		camera->moveUp(elapsedTime);
	}
	keyboardButton = glfwGetKey(display->GetWindow(), GLFW_KEY_F);
	if (keyboardButton == GLFW_PRESS)
	{
		camera->moveDown(elapsedTime);
	}
	keyboardButton = glfwGetKey(display->GetWindow(), GLFW_KEY_SPACE);
	if (keyboardButton == GLFW_PRESS)
	{
		camera->setCameraPosition(camera->getStartCameraPosition());
		camera->setForwardVector(camera->getStartForwardVector());
	}
}

bool InputHandler::mouseLock = true;