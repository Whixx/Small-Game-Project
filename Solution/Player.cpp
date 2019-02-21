#include "Player.h"
#include <iostream> // TODO: Remove after testing

Player::Player(float height, float fov, float near, float far, Mesh * mesh, Texture * texture, Maze * maze, irrklang::ISoundEngine* engine)
	:playerCamera(glm::vec3(0, height, 0), fov, (float)SCREENWIDTH / (float)SCREENHEIGHT, near, far, glm::vec3(0.0f, 0.0f, 1.0f)),
	playerTorch(this->transform, mesh, texture, engine),
	footStep("Sounds/playerfootstep.ogg", false, engine)
{
	this->playerHeight = height;
	this->playerSpeed = 0;
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

void Player::MoveDiagonalRightUp(float elapsedTime)
{
	// Getting diagonal vector
	glm::vec3 diagonal = (this->walkingVector + this->GetCamera()->GetRightVector());

	// Getting the positions
	this->playerCamera.SetOldCameraPosition(this->playerCamera.GetCameraPosition());
	glm::vec3 oldPos = this->playerCamera.GetOldCameraPosition();
	glm::vec3 newPos = playerCamera.GetCameraPosition() + this->playerSpeed * diagonal * elapsedTime;

	// Looking for collision
	this->DetectCollision(newPos, oldPos);

	footStep.Play();
}

void Player::MoveDiagonalLeftUp(float elapsedTime)
{
	// Getting diagonal vector
	glm::vec3 diagonal = (this->walkingVector - this->GetCamera()->GetRightVector());

	// Getting the positions
	this->playerCamera.SetOldCameraPosition(this->playerCamera.GetCameraPosition());
	glm::vec3 oldPos = this->playerCamera.GetOldCameraPosition();
	glm::vec3 newPos = playerCamera.GetCameraPosition() + this->playerSpeed * diagonal * elapsedTime;

	// Looking for collision
	this->DetectCollision(newPos, oldPos);

	footStep.Play();
}

void Player::MoveDiagonalRightDown(float elapsedTime)
{
	// Getting diagonal vector
	glm::vec3 diagonal = (this->walkingVector - this->GetCamera()->GetRightVector());

	// Getting the positions
	this->playerCamera.SetOldCameraPosition(this->playerCamera.GetCameraPosition());
	glm::vec3 oldPos = this->playerCamera.GetOldCameraPosition();
	glm::vec3 newPos = playerCamera.GetCameraPosition() - this->playerSpeed * diagonal * elapsedTime;

	// Looking for collision
	this->DetectCollision(newPos, oldPos);

	footStep.Play();
}

void Player::MoveDiagonalLeftDown(float elapsedTime)
{
	// Getting diagonal vector
	glm::vec3 diagonal = (this->walkingVector + this->GetCamera()->GetRightVector());

	// Getting the positions
	this->playerCamera.SetOldCameraPosition(this->playerCamera.GetCameraPosition());
	glm::vec3 oldPos = this->playerCamera.GetOldCameraPosition();
	glm::vec3 newPos = playerCamera.GetCameraPosition() - this->playerSpeed * diagonal * elapsedTime;

	// Looking for collision
	this->DetectCollision(newPos, oldPos);

	footStep.Play();
}

void Player::MoveForward(float elapsedTime)
{
	// Getting the positions
	this->playerCamera.SetOldCameraPosition(this->playerCamera.GetCameraPosition());
	glm::vec3 oldPos = this->playerCamera.GetOldCameraPosition();
	glm::vec3 newPos = playerCamera.GetCameraPosition() + this->playerSpeed * this->walkingVector * elapsedTime;
	
	// Looking for collision
	this->DetectCollision(newPos, oldPos);
	
	footStep.Play();
}

void Player::MoveBackward(float elapsedTime)
{
	// Getting the positions
	this->playerCamera.SetOldCameraPosition(this->playerCamera.GetCameraPosition());
	glm::vec3 oldPos = this->playerCamera.GetOldCameraPosition();
	glm::vec3 newPos = playerCamera.GetCameraPosition() - this->playerSpeed * this->walkingVector * elapsedTime;
	
	// Looking for collision
	this->DetectCollision(newPos, oldPos);

	footStep.Play();
}

void Player::MoveRight(float elapsedTime)
{
	// Getting the positions
	this->playerCamera.SetOldCameraPosition(this->playerCamera.GetCameraPosition());
	glm::vec3 oldPos = this->playerCamera.GetOldCameraPosition();
	glm::vec3 newPos = playerCamera.GetCameraPosition() + this->playerSpeed * playerCamera.GetRightVector() * elapsedTime;
	
	// Looking for collision
	this->DetectCollision(newPos, oldPos);

	footStep.Play();
}

void Player::MoveLeft(float elapsedTime)
{
	// Getting the positions
	this->playerCamera.SetOldCameraPosition(this->playerCamera.GetCameraPosition());
	glm::vec3 oldPos = this->playerCamera.GetOldCameraPosition();
	glm::vec3 newPos = playerCamera.GetCameraPosition() - this->playerSpeed * playerCamera.GetRightVector() * elapsedTime;

	// Looking for collision
	this->DetectCollision(newPos, oldPos);

	footStep.Play();
}

void Player::MoveUp(float elapsedTime)
{
	// Getting the positions
	this->playerCamera.SetOldCameraPosition(this->playerCamera.GetCameraPosition());
	glm::vec3 oldPos = this->playerCamera.GetOldCameraPosition();
	glm::vec3 newPos = playerCamera.GetCameraPosition() + this->playerSpeed * playerCamera.GetUpVector() * elapsedTime;

	// Looking for collision
	this->DetectCollision(newPos, oldPos);
	this->playerHeight = newPos.y;
}

void Player::MoveDown(float elapsedTime)
{
	// Getting the positions
	this->playerCamera.SetOldCameraPosition(this->playerCamera.GetCameraPosition());
	glm::vec3 oldPos = this->playerCamera.GetOldCameraPosition();
	glm::vec3 newPos = playerCamera.GetCameraPosition() - this->playerSpeed * playerCamera.GetUpVector() * elapsedTime;

	// Looking for collision
	this->DetectCollision(newPos, oldPos);
	this->playerHeight = newPos.y;
}

void Player::DetectCollision(glm::vec3 newPos, glm::vec3 oldPos)
{
	// Recieving components
	float height = this->maze->GetMazeHeight();
	float width = this->maze->GetMazeWidth();

	// The player needs an offset to not be able to clip the walls
	float playerOffset = 0.1f;

	// The offset is the offset for the whole bounding box which includes the player's torch
	float offset = this->boundingBoxHalfSize + playerOffset;

	// Check collision if player is on ground level
	if (this->playerHeight < 2.0f)
	{
		// Flat walls
		// Check right
		if (this->maze->IsWallAtWorld(newPos.x + offset, newPos.z) == true && (newPos.x > oldPos.x)) 
		{
			newPos.x = oldPos.x;
		}
		// Check left
		if (this->maze->IsWallAtWorld(newPos.x - offset, newPos.z) == true && (newPos.x < oldPos.x)) 
		{
			newPos.x = oldPos.x;
		}
		// Check up
		if (this->maze->IsWallAtWorld(newPos.x, newPos.z + offset) == true && (newPos.z > oldPos.z)) 
		{
			newPos.z = oldPos.z;
		}
		// Check down
		if (this->maze->IsWallAtWorld(newPos.x, newPos.z - offset) == true && (newPos.z < oldPos.z))
		{
			newPos.z = oldPos.z;
		}

		// Corners
		// Check right-down corner
		if (this->maze->IsWallAtWorld(newPos.x + offset, newPos.z + offset) == true) 
		{
			if (newPos.x > oldPos.x) 
			{
				newPos.x = oldPos.x;
			}
			if (newPos.z > oldPos.z) 
			{
				newPos.z = oldPos.z;
			}
		}
		// Check left-up corner
		if (this->maze->IsWallAtWorld(newPos.x - offset, newPos.z - offset) == true) 
		{
			if (newPos.x < oldPos.x) 
			{
				newPos.x = oldPos.x;
			}
			if (newPos.z < oldPos.z) 
			{
				newPos.z = oldPos.z;
			}
		}
		// Check right-up corner
		if (this->maze->IsWallAtWorld(newPos.x + offset, newPos.z - offset) == true) 
		{
			if (newPos.x > oldPos.x) 
			{
				newPos.x = oldPos.x;
			}
			if (newPos.z < oldPos.z) 
			{
				newPos.z = oldPos.z;
			}
		}
		// Check left-down corner
		if (this->maze->IsWallAtWorld(newPos.x - offset, newPos.z + offset) == true) 
		{
			if (newPos.x < oldPos.x) 
			{
				newPos.x = oldPos.x;
			}
			if (newPos.z > oldPos.z) 
			{
				newPos.z = oldPos.z;
			}
		}
	}

	// Update the new position
	playerCamera.SetCameraPosition(newPos);
}

void Player::CenterPlayer()
{
	float x = 0.0f;
	float y = 0.0f;

	// NOT NEEDED Transform world coords to texture coords. ( 1 pixel on texture corresponds to 1.0, origo is (0, 0) for both spaces

	bool pingpong = false;
	while (this->maze->IsWallAtWorld(x, y) == true) 
	{
		//If wall, move start position
		if (pingpong = false)
		{
			x += 1.0f;
			pingpong = true;
		}
		else
		{
			y += 1.0f;
			pingpong = false;
		}
	}

	this->playerCamera.SetCameraPosition(glm::vec3(x, this->playerHeight + this->maze->GetTransform()->GetPos().y, y));
}

void Player::UpdateMouse(const glm::vec2& newMousePosition, float elapsedTime)
{
	// Get mouse delta vector, how much the mouse has moved, update rotatearound vector
	this->playerCamera.SetMouseDelta(newMousePosition - playerCamera.GetOldMousePosition());
	this->playerCamera.SetRotateAround(glm::cross(playerCamera.GetForwardVector(), playerCamera.GetUpVector()));

	// Rotate forwardVector
	this->playerCamera.UpdateRotation();

	glm::vec3 forwardVector = playerCamera.GetForwardVector();
	float walkingX = forwardVector.x;
	float walkingY = 0.0f;
	float walkingZ = forwardVector.z;

	this->walkingVector = glm::vec3(walkingX, walkingY, walkingZ);
	this->walkingVector = glm::normalize(this->walkingVector);

	this->playerCamera.SetOldMousePosition(newMousePosition);
}

void Player::Update(double dt)
{
	// Set player position to the cameras position
	this->transform.SetPos(this->playerCamera.GetCameraPosition());
	
	// Update the torch in front of the player'
	this->playerTorch.SetPos(this->playerCamera.GetCameraPosition()
		+ this->GetWalkingVector() * this->boundingBoxHalfSize
		+ this->playerCamera.GetRightVector() * 0.075f
		+ this->playerCamera.GetUpVector() * -0.11f);

	// Update the torch's rotation
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
		//printf("Map position: X:%.2f, Y:%.2f Playerheight:%.2f\n", playerCamera.GetCameraPosition().x, playerCamera.GetCameraPosition().z, playerCamera.GetCameraPosition().y);

		//std::cout << "Forward Vector! X: " << this->playerCamera.GetForwardVector().x << std::endl;
		//std::cout << "Forward Vector! Y: " << this->playerCamera.GetForwardVector().y << std::endl;
		//std::cout << "Forward Vector! Z: " << this->playerCamera.GetForwardVector().z << std::endl;
	}
	
#endif

	// Update sound positions
	footStep.SetPosition(this->GetCamera()->GetCameraPosition());
}
