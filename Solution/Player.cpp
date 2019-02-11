#include "Player.h"
#include <iostream> // TODO: Remove after testing

Player::Player(float height, float fov, float near, float far)
	:playerCamera(glm::vec3(0, height, 0), fov, (float)SCREENWIDTH / (float)SCREENHEIGHT, near, far, glm::vec3(0.0f, 0.0f, 1.0f))
{
	this->playerSpeed = 0;
	this->walkingVector = glm::vec3(0.0f, 0.0f, 1.0f);
	//this->maze = maze;
}

Player::~Player()
{
}

float Player::GetPlayerSpeed()
{
	return this->playerSpeed;
}

float Player::GetPlayerHeight()
{
	return this->playerHeight;
}

glm::vec3 Player::GetWalkingVector()
{
	return this->walkingVector;
}

Camera * Player::GetCamera()
{
	return &playerCamera;
}

void Player::SetPlayerHeight(float height)
{
	this->playerHeight = height;
}

void Player::SetPlayerSpeed(float speed)
{
	this->playerSpeed = speed;
}

void Player::moveForward(float elapsedTime)
{
	glm::vec3 newPos = playerCamera.GetCameraPosition() + this->playerSpeed * this->walkingVector * elapsedTime;
	playerCamera.SetCameraPosition(newPos);
}

void Player::moveBackward(float elapsedTime)
{
	glm::vec3 newPos = playerCamera.GetCameraPosition() - this->playerSpeed * this->walkingVector * elapsedTime;
	playerCamera.SetCameraPosition(newPos);
}

void Player::moveRight(float elapsedTime)
{
	glm::vec3 newPos = playerCamera.GetCameraPosition() + this->playerSpeed * playerCamera.GetRotateAround() * elapsedTime;
	playerCamera.SetCameraPosition(newPos);
}

void Player::moveLeft(float elapsedTime)
{
	glm::vec3 newPos = playerCamera.GetCameraPosition() - this->playerSpeed * playerCamera.GetRotateAround() * elapsedTime;
	playerCamera.SetCameraPosition(newPos);
}

void Player::moveUp(float elapsedTime)
{
	glm::vec3 newPos = playerCamera.GetCameraPosition() + this->playerSpeed * playerCamera.GetUpVector() * elapsedTime;
	playerCamera.SetCameraPosition(newPos);
}

void Player::moveDown(float elapsedTime)
{
	glm::vec3 newPos = playerCamera.GetCameraPosition() - this->playerSpeed * playerCamera.GetUpVector() * elapsedTime;
	playerCamera.SetCameraPosition(newPos);
}

void Player::updateMouse(const glm::vec2& newMousePosition, float elapsedTime)
{
	// Get mouse delta vector, how much the mouse has moved, update rotatearound vector
	playerCamera.SetMouseDelta(newMousePosition - playerCamera.GetOldMousePosition());
	playerCamera.SetRotateAround(glm::cross(playerCamera.GetForwardVector(), playerCamera.GetUpVector()));

	// Rotate forwardVector
	playerCamera.UpdateRotation();

	glm::vec3 forwardVector = playerCamera.GetForwardVector();
	float walkingX = forwardVector.x;
	float walkingY = 0.0f;
	float walkingZ = forwardVector.z;

	this->walkingVector = glm::vec3(walkingX, walkingY, walkingZ);
	this->walkingVector = glm::normalize(this->walkingVector);

	playerCamera.SetOldMousePosition(newMousePosition);
}

void Player::Update(double dt)
{
	// Set player position to the cameras position
	transform.SetPos(playerCamera.GetCameraPosition());

	// Test variables
	double x = 0.0;
	double y = 0.0;

	// Move player
	x = 3.0;
	y = 0.0;

	/*
	// Check collision
	if (maze->IsWallAtWorld(x, y))
	{
		// Don't move
		cout << "V�GGGGG" << endl; // TODO: Remove after testing
	}
	*/

}