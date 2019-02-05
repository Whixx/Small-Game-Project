#include "Player.h"

Player::Player()
{
	this->playerSpeed = 0;
	this->walkingVector = playerSpeed * glm::vec3(0.3f, 0, 1);
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

void Player::SetPlayerHeight(float height)
{
	this->playerHeight = height;
}

void Player::SetPlayerSpeed(float speed)
{
	this->playerSpeed = speed;
	this->walkingVector = playerSpeed * glm::vec3(0.3f, 0, 1);
}

void Player::Update(double dt, Camera camera)
{
	// Set player position to the cameras position
	transform.SetPos(camera.getCameraPosition());

	// Collision

}