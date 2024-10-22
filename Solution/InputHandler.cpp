#include "InputHandler.h"

InputHandler::InputHandler()
{
}

InputHandler::~InputHandler()
{
}

void InputHandler::Key_callback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_E && action == GLFW_PRESS)
	{
		EventHandler& EH = EventHandler::GetInstance();
		EH.AddEvent(EVENT_KEY_E_PRESSED);
	}

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		EventHandler& EH = EventHandler::GetInstance();

		mouseLock = !mouseLock;
		if (mouseLock)
		{
			//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			//EH.AddEvent(EVENT_PLAYING);
		}	
		else
		{
			//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			EH.AddEvent(EVENT_PAUSED);
		}
	}
}

void InputHandler::mouse_button_callback(GLFWwindow * window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		EventHandler& EH = EventHandler::GetInstance();
		// Since leftclick has multiple uses, we instead add a event that its clicked, and check what type of click it is in the eventloop
		EH.AddEvent(EVENT_MOUSE_LEFT_PRESSED);
	}

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		EventHandler& EH = EventHandler::GetInstance();
		EH.AddEvent(EVENT_PLAYER_DROPCOIN);
	}
}

void InputHandler::MouseControls(Display * display, Player * player, float elapsedTime)
{
	double mouseXpos;
	double mouseYpos;

	// Find mouseposition (This function updates the X,Y values of the mouse position.
	glfwGetCursorPos(display->GetWindow(), &mouseXpos, &mouseYpos);
	if (this->mouseLock)
	{
		player->UpdateMouse(glm::vec2(mouseXpos, mouseYpos), elapsedTime);
	}
}

void InputHandler::KeyboardControls(Display * display, Player * player, float elapsedTime)
{
	// Front
	int W = glfwGetKey(display->GetWindow(), GLFW_KEY_W);
	int	S = glfwGetKey(display->GetWindow(), GLFW_KEY_S);
	
	// Sides
	int D = glfwGetKey(display->GetWindow(), GLFW_KEY_D);
	int A = glfwGetKey(display->GetWindow(), GLFW_KEY_A);

	// Upside
	int R = glfwGetKey(display->GetWindow(), GLFW_KEY_R);
	int F = glfwGetKey(display->GetWindow(), GLFW_KEY_F);

	// Recenter
	int Space = glfwGetKey(display->GetWindow(), GLFW_KEY_SPACE);

	bool doubleKeys = false;

	// ForwardRight
	if (W == GLFW_PRESS && D == GLFW_PRESS && doubleKeys == false)
	{
		player->MoveDiagonalRightUp(elapsedTime);
		doubleKeys = true;
	}

	// ForwardLeft
	if (W == GLFW_PRESS && A == GLFW_PRESS && doubleKeys == false)
	{
		player->MoveDiagonalLeftUp(elapsedTime);
		doubleKeys = true;
	}

	// BackwardRight
	if (S == GLFW_PRESS && D == GLFW_PRESS && doubleKeys == false)
	{
		player->MoveDiagonalRightDown(elapsedTime);
		doubleKeys = true;
	}

	// BackwardLeft
	if (S == GLFW_PRESS && A == GLFW_PRESS && doubleKeys == false)
	{
		player->MoveDiagonalLeftDown(elapsedTime);
		doubleKeys = true;
	}

	// Check for keyboard inputs, used to move the camera around.
	// WASD for movearound, RF (Rise,Fall) and space to set the initial camera position & viewDir.
	if (W == GLFW_PRESS && doubleKeys == false)
	{
		player->MoveForward(elapsedTime);
	}
	if (S == GLFW_PRESS && doubleKeys == false)
	{
		player->MoveBackward(elapsedTime);
	}
	if (D == GLFW_PRESS && doubleKeys == false)
	{
		player->MoveRight(elapsedTime);
	}
	if (A == GLFW_PRESS && doubleKeys == false)
	{
		player->MoveLeft(elapsedTime);
	}
#if defined(DEBUG)
	if (R == GLFW_PRESS && doubleKeys == false)
	{
		player->MoveUp(elapsedTime);
	}
	if (F == GLFW_PRESS && doubleKeys == false)
	{
		player->MoveDown(elapsedTime);
	}
#endif
	/*if (Space == GLFW_PRESS)
	{
		player->CenterPlayer();
	}*/
}

void InputHandler::SetMouseLockTrue()
{
	this->mouseLock = true;
}

bool InputHandler::mouseLock = false;
