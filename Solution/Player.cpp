#include "Player.h"
#include <iostream> // TODO: Remove after testing

Player::Player(float height, float fov, float near, float far, Maze * maze, irrklang::ISoundEngine * engine, PointLightHandler * PLH, float torchSize)
	: playerCamera(glm::vec3(0, height, 0), fov, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, near, far, glm::vec3(0.0f, 0.0f, 1.0f)),
	playerTorch(this->transform, glm::vec3(0.5f, 0.15f, 0.15f), engine, PLH, torchSize),
	footStep("Sounds/playerfootstep.ogg", false, engine),
	coinModel()
{
	this->playerHeight = height;
	this->playerSpeed = 0;
	this->walkingVector = glm::vec3(0.0f, 0.0f, 1.0f);
	this->maze = maze;

	this->footStep.SetVolume(0.2);

	CenterPlayer(); //Space to return to origin

	this->nrOfInventoryCoins = 0;
	this->nrOfWorldCoins = 0;

	// Add startingCoins for the player
	for (int i = 0; i < MaxNrOfCoins; i++)
		this->AddCoin();	// Incrementing nrOfInventoryCoins
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

Coin * Player::GetInventoryCoin(unsigned int index)
{
	return &this->inventoryCoins[index];
}

Coin * Player::GetWorldCoin(unsigned int index)
{
	return nullptr;
}

unsigned int Player::GetNrOfInventoryCoins()
{
	return this->nrOfInventoryCoins;
}

unsigned int Player::GetNrOfWorldCoins()
{
	return this->nrOfWorldCoins;
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
	glm::vec3 diagonal = (this->walkingVector + this->GetCamera()->GetRightVector()) * 0.8f;

	// Getting the positions
	this->playerCamera.SetOldCameraPosition(this->playerCamera.GetCameraPosition());
	glm::vec3 oldPos = this->playerCamera.GetOldCameraPosition();
	glm::vec3 newPos = playerCamera.GetCameraPosition() + this->playerSpeed * diagonal * elapsedTime;

	// Looking for collision
	newPos = this->DetectCollision(newPos, oldPos);

	// Update the new position
	this->transform.SetPos(newPos);

	footStep.Play();
}

void Player::MoveDiagonalLeftUp(float elapsedTime)
{
	// Getting diagonal vector
	glm::vec3 diagonal = (this->walkingVector - this->GetCamera()->GetRightVector()) * 0.8f;

	// Getting the positions
	this->playerCamera.SetOldCameraPosition(this->playerCamera.GetCameraPosition());
	glm::vec3 oldPos = this->playerCamera.GetOldCameraPosition();
	glm::vec3 newPos = playerCamera.GetCameraPosition() + this->playerSpeed * diagonal * elapsedTime;

	// Looking for collision
	newPos = this->DetectCollision(newPos, oldPos);

	// Update the new position
	this->transform.SetPos(newPos);

	footStep.Play();
}

void Player::MoveDiagonalRightDown(float elapsedTime)
{
	// Getting diagonal vector
	glm::vec3 diagonal = (this->walkingVector - this->GetCamera()->GetRightVector()) * 0.8f;

	// Getting the positions
	this->playerCamera.SetOldCameraPosition(this->playerCamera.GetCameraPosition());
	glm::vec3 oldPos = this->playerCamera.GetOldCameraPosition();
	glm::vec3 newPos = playerCamera.GetCameraPosition() - this->playerSpeed * diagonal * elapsedTime;

	// Looking for collision
	newPos = this->DetectCollision(newPos, oldPos);

	// Update the new position
	this->transform.SetPos(newPos);

	footStep.Play();
}

void Player::MoveDiagonalLeftDown(float elapsedTime)
{
	// Getting diagonal vector
	glm::vec3 diagonal = (this->walkingVector + this->GetCamera()->GetRightVector()) * 0.8f;

	// Getting the positions
	this->playerCamera.SetOldCameraPosition(this->playerCamera.GetCameraPosition());
	glm::vec3 oldPos = this->playerCamera.GetOldCameraPosition();
	glm::vec3 newPos = playerCamera.GetCameraPosition() - this->playerSpeed * diagonal * elapsedTime;

	// Looking for collision
	newPos = this->DetectCollision(newPos, oldPos);

	// Update the new position
	this->transform.SetPos(newPos);

	footStep.Play();
}

void Player::MoveForward(float elapsedTime)
{
	// Getting the positions
	this->playerCamera.SetOldCameraPosition(this->playerCamera.GetCameraPosition());
	glm::vec3 oldPos = this->playerCamera.GetOldCameraPosition();
	glm::vec3 newPos = playerCamera.GetCameraPosition() + this->playerSpeed * this->walkingVector * elapsedTime;
	
	// Looking for collision
	newPos = this->DetectCollision(newPos, oldPos);

	// Update the new position
	this->transform.SetPos(newPos);
	
	footStep.Play();
}

void Player::MoveBackward(float elapsedTime)
{
	// Getting the positions
	this->playerCamera.SetOldCameraPosition(this->playerCamera.GetCameraPosition());
	glm::vec3 oldPos = this->playerCamera.GetOldCameraPosition();
	glm::vec3 newPos = playerCamera.GetCameraPosition() - this->playerSpeed * this->walkingVector * elapsedTime;
	
	// Looking for collision
	newPos = this->DetectCollision(newPos, oldPos);

	// Update the new position
	this->transform.SetPos(newPos);

	footStep.Play();
}

void Player::MoveRight(float elapsedTime)
{
	// Getting the positions
	this->playerCamera.SetOldCameraPosition(this->playerCamera.GetCameraPosition());
	glm::vec3 oldPos = this->playerCamera.GetOldCameraPosition();
	glm::vec3 newPos = playerCamera.GetCameraPosition() + this->playerSpeed * playerCamera.GetRightVector() * elapsedTime;
	
	// Looking for collision
	newPos = this->DetectCollision(newPos, oldPos);

	// Update the new position
	this->transform.SetPos(newPos);

	footStep.Play();
}

void Player::MoveLeft(float elapsedTime)
{
	// Getting the positions
	this->playerCamera.SetOldCameraPosition(this->playerCamera.GetCameraPosition());
	glm::vec3 oldPos = this->playerCamera.GetOldCameraPosition();
	glm::vec3 newPos = playerCamera.GetCameraPosition() - this->playerSpeed * playerCamera.GetRightVector() * elapsedTime;

	// Looking for collision
	newPos = this->DetectCollision(newPos, oldPos);

	// Update the new position
	this->transform.SetPos(newPos);

	footStep.Play();
}

void Player::MoveUp(float elapsedTime)
{
	// Getting the positions
	this->playerCamera.SetOldCameraPosition(this->playerCamera.GetCameraPosition());
	glm::vec3 oldPos = this->playerCamera.GetOldCameraPosition();
	glm::vec3 newPos = playerCamera.GetCameraPosition() + this->playerSpeed * playerCamera.GetUpVector() * elapsedTime;

	// Looking for collision
	newPos = this->DetectCollision(newPos, oldPos);

	// Update the new position
	this->transform.SetPos(newPos);
	this->playerHeight = newPos.y;
}

void Player::MoveDown(float elapsedTime)
{
	// Getting the positions
	this->playerCamera.SetOldCameraPosition(this->playerCamera.GetCameraPosition());
	glm::vec3 oldPos = this->playerCamera.GetOldCameraPosition();
	glm::vec3 newPos = playerCamera.GetCameraPosition() - this->playerSpeed * playerCamera.GetUpVector() * elapsedTime;

	// Looking for collision
	newPos = this->DetectCollision(newPos, oldPos);

	// Update the new position
	this->transform.SetPos(newPos);
	this->playerHeight = newPos.y;
}

glm::vec3 Player::DetectCollision(glm::vec3 newPos, glm::vec3 oldPos)
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

	return newPos;
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
			x += 1.5f;
			pingpong = true;
		}
		else
		{
			y += 1.5f;
			pingpong = false;
		}
	}

	this->transform.SetPos(glm::vec3(x, this->playerHeight + this->maze->GetTransform()->GetPos().y, y));
}

void Player::UpdateMouse(const glm::vec2& newMousePosition, float elapsedTime)
{
	// Get mouse delta vector, how much the mouse has moved, update rotatearound vector
	this->playerCamera.SetMouseDelta(newMousePosition - this->playerCamera.GetOldMousePosition());
	this->playerCamera.SetRotateAround(glm::cross(this->playerCamera.GetForwardVector(), this->playerCamera.GetUpVector()));

	// Rotate forwardVector
	this->playerCamera.UpdateRotation();

	glm::vec3 forwardVector = this->playerCamera.GetForwardVector();
	float walkingX = forwardVector.x;
	float walkingY = 0.0f;
	float walkingZ = forwardVector.z;

	this->walkingVector = glm::vec3(walkingX, walkingY, walkingZ);
	this->walkingVector = glm::normalize(this->walkingVector);

	this->playerCamera.SetOldMousePosition(newMousePosition);
}

void Player::Update(double dt)
{
	// Update playerCamera
	this->playerCamera.SetCameraPosition(this->transform.GetPos());
	this->playerCamera.UpdateViewMatrix();

	// Update the torch
	this->playerTorch.Update(dt,
		this->playerCamera,
		this->walkingVector, 
		this->boundingBoxHalfSize);

#ifdef DEBUG
	if (this->playerCamera.GetCameraPosition() != this->playerCamera.GetOldCameraPosition())
	{
		//printf("Map position: X:%.2f, Z:%.2f Playerheight:%.2f\n", playerCamera.GetCameraPosition().x, playerCamera.GetCameraPosition().z, playerCamera.GetCameraPosition().y);
	
		//std::cout << "Forward Vector! X: " << this->playerCamera.GetForwardVector().x << std::endl;
		//std::cout << "Forward Vector! Y: " << this->playerCamera.GetForwardVector().y << std::endl;
		//std::cout << "Forward Vector! Z: " << this->playerCamera.GetForwardVector().z << std::endl;
	}
	
#endif

	// Update sound positions
	footStep.SetPosition(this->GetCamera()->GetCameraPosition());
}

void Player::AddCoin()
{
	// Check if bag is full
	if (this->nrOfInventoryCoins == 10)
	{
		// No more coins can be added
		return;
	}

	// Add a coin
	this->inventoryCoins[this->nrOfInventoryCoins].GetTransform()->SetPos(this->transform.GetPos());
	this->inventoryCoins[this->nrOfInventoryCoins].GetTransform()->SetScale(glm::vec3(0.2f, 0.2f, 0.2f));

	// Increment NrOfCoins
	this->nrOfInventoryCoins++;
}

void Player::RemoveCoin()
{
	if (this->nrOfInventoryCoins == 0)
	{
		return;
	}
		
	// Removes the last coin in the array
	this->nrOfInventoryCoins--;
}

void Player::DrawCoin(unsigned int index, Shader * shader)
{
	this->inventoryCoins[index].Draw(&this->coinModel, shader);
}

void Player::LayCoin()
{
	// Check if the player got coins to throw
	if (this->nrOfInventoryCoins == 0)
	{
		return;
	}

	// Check if world can have more coins // DYNAMIC FIX
	if (this->nrOfWorldCoins == 256)
	{
		return;
	}

	// Set new variables for the coin
	this->worldCoins[this->nrOfWorldCoins].GetTransform()->SetPos(this->transform.GetPos());
	this->worldCoins[this->nrOfWorldCoins].GetTransform()->SetScale(glm::vec3(0.2f, 0.2f, 0.2f));

	this->nrOfWorldCoins++;
	this->nrOfInventoryCoins--;
}
