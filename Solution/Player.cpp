#include "Player.h"
#include <iostream> // TODO: Remove after testing

Player::Player(float height, float fov, float near, float far, Mesh * mesh, Texture * texture, Maze * maze, irrklang::ISoundEngine* engine)
	:playerCamera(glm::vec3(0, height, 0), fov, (float)SCREENWIDTH / (float)SCREENHEIGHT, near, far, glm::vec3(0.0f, 0.0f, 1.0f)),
	playerTorch(this->transform, mesh, texture, engine),
	footStep("Sounds/playerfootstep.ogg", false, engine)
{
	this->playerSpeed = 0;
	this->boundingBoxHalfSize = 0.8f;
	this->walkingVector = glm::vec3(0.0f, 0.0f, 1.0f);
	this->maze = maze;

	this->footStep.SetVolume(0.8);
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

	if (newPos.x > 0 && newPos.z > 0 && newPos.z < maze->GetMazeHeight() && newPos.x < maze->GetMazeWidth())
	{
		// Check collision
		if (this->maze->IsWallAtWorld(newPos.x, newPos.z) == false) {
			playerCamera.SetCameraPosition(newPos);
		}
	}
	else
	{
		playerCamera.SetCameraPosition(newPos);
	}
	
	footStep.Play();
}

void Player::MoveBackward(float elapsedTime)
{
	this->playerCamera.SetOldCameraPosition(this->playerCamera.GetCameraPosition());
	glm::vec3 newPos = playerCamera.GetCameraPosition() - this->playerSpeed * this->walkingVector * elapsedTime;

	if (newPos.x > 0 && newPos.z > 0 && newPos.z < maze->GetMazeHeight() && newPos.x < maze->GetMazeWidth())
	{
		// Check collision
		if (this->maze->IsWallAtWorld(newPos.x, newPos.z) == false) {
			playerCamera.SetCameraPosition(newPos);
		}
	}
	else
	{
		playerCamera.SetCameraPosition(newPos);
	}
	footStep.Play();
}

void Player::MoveRight(float elapsedTime)
{
	this->playerCamera.SetOldCameraPosition(this->playerCamera.GetCameraPosition());
	glm::vec3 newPos = playerCamera.GetCameraPosition() + this->playerSpeed * playerCamera.GetRotateAround() * elapsedTime;

	if (newPos.x > 0 && newPos.z > 0 && newPos.z < maze->GetMazeHeight() && newPos.x < maze->GetMazeWidth())
	{
		// Check collision
		if (this->maze->IsWallAtWorld(newPos.x, newPos.z) == false) {
			playerCamera.SetCameraPosition(newPos);
		}
	}
	else
	{
		playerCamera.SetCameraPosition(newPos);
	}
	footStep.Play();
}

void Player::MoveLeft(float elapsedTime)
{
	this->playerCamera.SetOldCameraPosition(this->playerCamera.GetCameraPosition());
	glm::vec3 newPos = playerCamera.GetCameraPosition() - this->playerSpeed * playerCamera.GetRotateAround() * elapsedTime;

	if (newPos.x > 0 && newPos.z > 0 && newPos.z < maze->GetMazeHeight() && newPos.x < maze->GetMazeWidth())
	{
		// Check collision
		if (this->maze->IsWallAtWorld(newPos.x, newPos.z) == false) {
			playerCamera.SetCameraPosition(newPos);
		}
	}
	else
	{
		playerCamera.SetCameraPosition(newPos);
	}
	footStep.Play();
}

void Player::MoveUp(float elapsedTime)
{
	this->playerCamera.SetOldCameraPosition(this->playerCamera.GetCameraPosition());
	glm::vec3 newPos = playerCamera.GetCameraPosition() + this->playerSpeed * playerCamera.GetUpVector() * elapsedTime;

	if (newPos.x > 0 && newPos.z > 0 && newPos.z < maze->GetMazeHeight() && newPos.x < maze->GetMazeWidth())
	{
		// Check collision
		if (this->maze->IsWallAtWorld(newPos.x, newPos.z) == false) {
			playerCamera.SetCameraPosition(newPos);
		}
	}
	else
	{
		playerCamera.SetCameraPosition(newPos);
	}
}

void Player::MoveDown(float elapsedTime)
{
	this->playerCamera.SetOldCameraPosition(this->playerCamera.GetCameraPosition());
	glm::vec3 newPos = playerCamera.GetCameraPosition() - this->playerSpeed * playerCamera.GetUpVector() * elapsedTime;

	if (newPos.x > 0 && newPos.z > 0 && newPos.z < maze->GetMazeHeight() && newPos.x < maze->GetMazeWidth())
	{
		// Check collision
		if (this->maze->IsWallAtWorld(newPos.x, newPos.z) == false) {
			playerCamera.SetCameraPosition(newPos);
		}
	}
	else
	{
		playerCamera.SetCameraPosition(newPos);
	}
}

void Player::CenterPlayer()
{
	int halfMazeWidth = round(this->maze->GetMazeWidth() / 2);
	int halfMazeHeight = round(this->maze->GetMazeHeight() / 2);

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
		printf("Player position: X:%.2f, Y:%.2f Playerheight:%.2f\n", playerCamera.GetCameraPosition().x, playerCamera.GetCameraPosition().z, playerCamera.GetCameraPosition().y);
		printf("Pixel positions: X:%.2f, Y:%.2f Playerheight:%.2f\n", floor(playerCamera.GetCameraPosition().x), floor(playerCamera.GetCameraPosition().z), playerCamera.GetCameraPosition().y);
	}
#endif

	// update sound positions
	footStep.SetPosition(this->GetCamera()->GetCameraPosition());
}
