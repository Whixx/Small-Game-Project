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
		mouseLock = !mouseLock;
		if (mouseLock)
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		else
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
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
		//player->minotaurSound.PlayGrowl();
	}
	if (S == GLFW_PRESS && doubleKeys == false)
	{
		player->MoveBackward(elapsedTime);
		//player->minotaurSound.StopAllSounds();
		//player->sound.StopAllSounds();
	}
	if (D == GLFW_PRESS && doubleKeys == false)
	{
		player->MoveRight(elapsedTime);
	}
	if (A == GLFW_PRESS && doubleKeys == false)
	{
		player->MoveLeft(elapsedTime);
	}
	if (R == GLFW_PRESS && doubleKeys == false)
	{
		player->MoveUp(elapsedTime);
	}
	if (F == GLFW_PRESS && doubleKeys == false)
	{
		player->MoveDown(elapsedTime);
	}
	if (Space == GLFW_PRESS)
	{
		player->CenterPlayer();
	}
	//keyboardButton = glfwGetKey(display->GetWindow(), GLFW_KEY_C);
	//if (keyboardButton == GLFW_PRESS)
	//{
	//	player->sound.PlayCoinSound(
	//		irrklang::vec3df(player->GetCamera()->GetCameraPosition().x, player->GetCamera()->GetCameraPosition().y, -player->GetCamera()->GetCameraPosition().z) + 
	//		irrklang::vec3df(player->GetCamera()->GetForwardVector().x, player->GetCamera()->GetForwardVector().y, -player->GetCamera()->GetForwardVector().z)*30);
	//}
}

bool InputHandler::mouseLock = true;
