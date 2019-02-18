#include "Player.h"
#include <iostream> // TODO: Remove after testing

Player::Player(float height, float fov, float near, float far, Mesh * mesh, Texture * texture, Maze * maze, PointLightHandler* PLH)
	:playerCamera(glm::vec3(0, height, 0), fov, (float)SCREENWIDTH / (float)SCREENHEIGHT, near, far, glm::vec3(0.0f, 0.0f, 1.0f))
{
	this->playerTorch = Torch(this->transform, mesh, texture, PLH, glm::vec3(1.0f, 0.3f, 0.3f));
	this->playerSpeed = 0;
	this->boundingBoxHalfSize = 0.8f;
	this->walkingVector = glm::vec3(0.0f, 0.0f, 1.0f);
	this->maze = maze;
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
	return &this->playerCamera;
}

Torch* Player::GetTorch()
{
	return &this->playerTorch;
}

void Player::SetPlayerHeight(float height)
{
	this->playerHeight = height;
}

void Player::SetPlayerSpeed(float speed)
{
	this->playerSpeed = speed;
}

void Player::MoveForward(float elapsedTime)
{
	this->playerCamera.SetOldCameraPosition(this->playerCamera.GetCameraPosition());
	glm::vec3 newPos = playerCamera.GetCameraPosition() + this->playerSpeed * this->walkingVector * elapsedTime;

	// Check collision
	if (this->maze->IsWallAtWorld(newPos.x, newPos.z) == true) {
		playerCamera.SetCameraPosition(newPos);
	}
}

void Player::MoveBackward(float elapsedTime)
{
	this->playerCamera.SetOldCameraPosition(this->playerCamera.GetCameraPosition());
	glm::vec3 newPos = playerCamera.GetCameraPosition() - this->playerSpeed * this->walkingVector * elapsedTime;

	// Check collision
	if (this->maze->IsWallAtWorld(newPos.x, newPos.z) == true) {
		playerCamera.SetCameraPosition(newPos);
	}
}

void Player::MoveRight(float elapsedTime)
{
	this->playerCamera.SetOldCameraPosition(this->playerCamera.GetCameraPosition());
	glm::vec3 newPos = playerCamera.GetCameraPosition() + this->playerSpeed * playerCamera.GetRotateAround() * elapsedTime;

	// Check collision
	if (this->maze->IsWallAtWorld(newPos.x, newPos.z) == true) {
		playerCamera.SetCameraPosition(newPos);
	}
}

void Player::MoveLeft(float elapsedTime)
{
	this->playerCamera.SetOldCameraPosition(this->playerCamera.GetCameraPosition());
	glm::vec3 newPos = playerCamera.GetCameraPosition() - this->playerSpeed * playerCamera.GetRotateAround() * elapsedTime;

	// Check collision
	if (this->maze->IsWallAtWorld(newPos.x, newPos.z) == true) {
		playerCamera.SetCameraPosition(newPos);
	}
}

void Player::MoveUp(float elapsedTime)
{
	this->playerCamera.SetOldCameraPosition(this->playerCamera.GetCameraPosition());
	glm::vec3 newPos = playerCamera.GetCameraPosition() + this->playerSpeed * playerCamera.GetUpVector() * elapsedTime;

	// Check collision
	if (this->maze->IsWallAtWorld(newPos.x, newPos.z) == true) {
		playerCamera.SetCameraPosition(newPos);
	}
}

void Player::MoveDown(float elapsedTime)
{
	this->playerCamera.SetOldCameraPosition(this->playerCamera.GetCameraPosition());
	glm::vec3 newPos = playerCamera.GetCameraPosition() - this->playerSpeed * playerCamera.GetUpVector() * elapsedTime;

	// Check collision
	if (this->maze->IsWallAtWorld(newPos.x, newPos.z) == true) {
		playerCamera.SetCameraPosition(newPos);
	}
}

void Player::CenterPlayer()
{
	float halfMazeWidth = this->maze->GetMazeWidth() / 2;
	float halfMazeHeight = this->maze->GetMazeHeight() / 2;

	bool pingpong = false;
	while (this->maze->IsWallAtWorld(halfMazeWidth, halfMazeHeight) == true) 
	{
		//If wall, move start position
		if (pingpong = false)
		{
			halfMazeWidth += 1;
			pingpong = true;
		}
		else
		{
			halfMazeWidth += 1;
			pingpong = false;
		}
	}

	playerCamera.SetCameraPosition(glm::vec3(halfMazeWidth, playerHeight, halfMazeHeight));
	playerCamera.SetOldCameraPosition(glm::vec3(halfMazeWidth, playerHeight, halfMazeHeight));
}

void Player::UpdateMouse(const glm::vec2& newMousePosition, float elapsedTime)
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
	this->transform.SetPos(this->playerCamera.GetCameraPosition());
	
	// Update the torch in front of the player'
	this->playerTorch.SetPos(this->playerCamera.GetCameraPosition()
		+ this->GetWalkingVector() * this->boundingBoxHalfSize
		+ this->playerCamera.GetRightVector() * 0.4f
		+ this->playerCamera.GetUpVector() * -0.5f);

	glm::vec3 forward = glm::vec3(playerTorch.GetTransform().GetWorldMatrix()[2][0], playerTorch.GetTransform().GetWorldMatrix()[2][1], playerTorch.GetTransform().GetWorldMatrix()[2][2]);
	glm::vec3 camToTorch = glm::vec3(playerTorch.GetPos() - playerCamera.GetCameraPosition());
	glm::vec3 crossVect = normalize(glm::cross(camToTorch, forward));

	if (crossVect.y > 0.3f)
	{
		this->playerTorch.GetRot().y -= dt * 10;
	}
	else if (crossVect.y < -0.3f)
	{
		this->playerTorch.GetRot().y += dt * 10;
	}
	else 
	{
		// No rotation
	}
	
#ifdef DEBUG
	if (this->playerCamera.GetCameraPosition() != this->playerCamera.GetOldCameraPosition())
	{
		printf("Map position: X:%.2f, Y:%.2f Playerheight:%.2f\n", playerCamera.GetCameraPosition().x, playerCamera.GetCameraPosition().z, playerCamera.GetCameraPosition().y);
	}
#endif

	// Test variables
	double x = 0.0;
	double y = 0.0;

	// Move player
	x = 3.0;
	y = 0.0;

	// Check collision
	/*
	if (maze->IsWallAtWorld(x, y))
	{
		// Don't move
		cout << "V�GGGGG" << endl; // TODO: Remove after testing
	}
	*/

}