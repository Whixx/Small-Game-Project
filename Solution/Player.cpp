#include "Player.h"

Player::Player(float height, float fov, float near, float far, glm::vec3 walkingVector)
	:playerCamera(glm::vec3(0, height, 0), fov, (float)SCREENWIDTH / (float)SCREENHEIGHT, near, far, walkingVector)
{
	this->playerSpeed = 0;
	this->walkingVector = walkingVector;
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
	glm::vec3 newPos = playerCamera.getCameraPosition() + this->playerSpeed * this->walkingVector * elapsedTime;
	playerCamera.setCameraPosition(newPos);
}

void Player::moveBackward(float elapsedTime)
{
	glm::vec3 newPos = playerCamera.getCameraPosition() - this->playerSpeed * this->walkingVector * elapsedTime;
	playerCamera.setCameraPosition(newPos);
}

void Player::moveRight(float elapsedTime)
{
	glm::vec3 newPos = playerCamera.getCameraPosition() + this->playerSpeed * playerCamera.getRotateAround() * elapsedTime;
	playerCamera.setCameraPosition(newPos);
}

void Player::moveLeft(float elapsedTime)
{
	glm::vec3 newPos = playerCamera.getCameraPosition() - this->playerSpeed * playerCamera.getRotateAround() * elapsedTime;
	playerCamera.setCameraPosition(newPos);
}

void Player::moveUp(float elapsedTime)
{
	glm::vec3 newPos = playerCamera.getCameraPosition() + this->playerSpeed * playerCamera.getUpVector() * elapsedTime;
	playerCamera.setCameraPosition(newPos);
}

void Player::moveDown(float elapsedTime)
{
	glm::vec3 newPos = playerCamera.getCameraPosition() - this->playerSpeed * playerCamera.getUpVector() * elapsedTime;
	playerCamera.setCameraPosition(newPos);
}

void Player::updateMouse(const glm::vec2& newMousePosition, float elapsedTime)
{
	// Get mouse delta vector, how much the mouse has moved
	playerCamera.setMouseDelta(newMousePosition - playerCamera.getOldMousePosition());

	// if the mouseDelta is to far away, the camera will jump to unpredicted areas.
	if (glm::length(playerCamera.getMouseDelta()) < 50.0f)
	{
		//Update the horizontal view
		playerCamera.setForwardVector(glm::mat3(glm::rotate(-playerCamera.getMouseDelta().x * playerCamera.getRotationalSpeed() * elapsedTime, playerCamera.getUpVector())) * playerCamera.getForwardVector());
		this->walkingVector = glm::mat3(glm::rotate(-playerCamera.getMouseDelta().x * playerCamera.getRotationalSpeed() * elapsedTime, playerCamera.getUpVector())) * this->walkingVector;

		//Update the vertical view limited to 45 degrees
		playerCamera.setRotateAround(glm::cross(playerCamera.getForwardVector(), playerCamera.getUpVector()));
		if (glm::dot(glm::normalize(playerCamera.getForwardVector()), glm::normalize(this->walkingVector)) > 0.45f)
		{
			playerCamera.setForwardVector(glm::mat3(glm::rotate(-playerCamera.getMouseDelta().y * playerCamera.getRotationalSpeed() * elapsedTime, playerCamera.getRotateAround())) * playerCamera.getForwardVector());
		}
		else
		{
			playerCamera.setForwardVector(glm::mat3(glm::rotate(playerCamera.getMouseDelta().y * playerCamera.getRotationalSpeed() * elapsedTime, playerCamera.getRotateAround())) * playerCamera.getForwardVector());
		}
	}

	playerCamera.setOldMousePosition(newMousePosition);
}

void Player::Update(double dt)
{
	// Set player position to the cameras position
	transform.SetPos(playerCamera.getCameraPosition());

	// Collision

}