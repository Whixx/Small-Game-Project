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
	int keyboardButton;
	// Check for keyboard inputs, used to move the camera around.
	// WASD for movearound, RF (Rise,Fall) and space to set the initial camera position & viewDir.
	keyboardButton = glfwGetKey(display->GetWindow(), GLFW_KEY_W);
	if (keyboardButton == GLFW_PRESS)
	{
		player->MoveForward(elapsedTime);
		player->minotaurSound.PlayGrowl();
	}
	keyboardButton = glfwGetKey(display->GetWindow(), GLFW_KEY_S);
	if (keyboardButton == GLFW_PRESS)
	{
		player->MoveBackward(elapsedTime);
		player->minotaurSound.StopAllSounds();
		player->sound.StopAllSounds();
	}
	keyboardButton = glfwGetKey(display->GetWindow(), GLFW_KEY_D);
	if (keyboardButton == GLFW_PRESS)
	{
		player->MoveRight(elapsedTime);
	}
	keyboardButton = glfwGetKey(display->GetWindow(), GLFW_KEY_A);
	if (keyboardButton == GLFW_PRESS)
	{
		player->MoveLeft(elapsedTime);
	}
	keyboardButton = glfwGetKey(display->GetWindow(), GLFW_KEY_R);
	if (keyboardButton == GLFW_PRESS)
	{
		player->MoveUp(elapsedTime);
	}
	keyboardButton = glfwGetKey(display->GetWindow(), GLFW_KEY_F);
	if (keyboardButton == GLFW_PRESS)
	{
		player->MoveDown(elapsedTime);
	}
	keyboardButton = glfwGetKey(display->GetWindow(), GLFW_KEY_SPACE);
	if (keyboardButton == GLFW_PRESS)
	{
		player->GetCamera()->SetCameraPosition(player->GetCamera()->GetStartCameraPosition());
		player->GetCamera()->SetForwardVector(player->GetCamera()->GetStartForwardVector());
	}
	keyboardButton = glfwGetKey(display->GetWindow(), GLFW_KEY_C);
	if (keyboardButton == GLFW_PRESS)
	{
		player->sound.PlayCoinSound(
			irrklang::vec3df(player->GetCamera()->GetCameraPosition().x, player->GetCamera()->GetCameraPosition().y, -player->GetCamera()->GetCameraPosition().z) + 
			irrklang::vec3df(player->GetCamera()->GetForwardVector().x, player->GetCamera()->GetForwardVector().y, -player->GetCamera()->GetForwardVector().z)*30);
	}
}

bool InputHandler::mouseLock = true;
