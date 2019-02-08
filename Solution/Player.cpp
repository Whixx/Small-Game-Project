#include "Player.h"
#include <iostream> // TODO: Remove after testing

Player::Player(float height, float fov, float near, float far, glm::vec3 walkingVector)
	:playerCamera(glm::vec3(0, height, 0), fov, (float)SCREENWIDTH / (float)SCREENHEIGHT, near, far, walkingVector)
{
	this->playerSpeed = 0;
	this->walkingVector = walkingVector;
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
	// Get mouse delta vector, how much the mouse has moved
	playerCamera.SetMouseDelta(newMousePosition - playerCamera.GetOldMousePosition());

	// if the mouseDelta is to far away, the camera will jump to unpredicted areas.
	if (glm::length(playerCamera.GetMouseDelta()) < 50.0f)
	{
		//Update the horizontal view
		playerCamera.SetForwardVector(glm::mat3(glm::rotate(-playerCamera.GetMouseDelta().x * playerCamera.GetRotationalSpeed() * elapsedTime, playerCamera.GetUpVector())) * playerCamera.GetForwardVector());
		this->walkingVector = glm::mat3(glm::rotate(-playerCamera.GetMouseDelta().x * playerCamera.GetRotationalSpeed() * elapsedTime, playerCamera.GetUpVector())) * this->walkingVector;

		//Update the vertical view limited to 45 degrees
		playerCamera.SetRotateAround(glm::cross(playerCamera.GetForwardVector(), playerCamera.GetUpVector()));
		//if (glm::dot(glm::normalize(playerCamera.GetForwardVector()), glm::normalize(this->walkingVector)) > 0.45f)
		//{
			playerCamera.SetForwardVector(glm::mat3(glm::rotate(-playerCamera.GetMouseDelta().y * playerCamera.GetRotationalSpeed() * elapsedTime, playerCamera.GetRotateAround())) * playerCamera.GetForwardVector());
		//}
		//else
		//{
		//	playerCamera.SetForwardVector(glm::mat3(glm::rotate(playerCamera.GetMouseDelta().y * playerCamera.GetRotationalSpeed() * elapsedTime, playerCamera.GetRotateAround())) * playerCamera.GetForwardVector());
		//}
	}

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