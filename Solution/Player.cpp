#include "Player.h"
#include <iostream> // TODO: Remove after testing

Player::Player(float height, float fov, float near, float far, Maze * maze, irrklang::ISoundEngine * engine, PointLightHandler * PLH, float torchSize, Minotaur * minotaur)
	: playerCamera(glm::vec3(0, height, 0), fov, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, near, far, glm::vec3(0.0f, 0.0f, 1.0f)),
	playerTorch(this->transform, glm::vec3(0.5f, 0.15f, 0.15f), engine, PLH, torchSize),
	footStep("Sounds/playerfootstep.ogg", false, engine),
	dropSound("Sounds/CoinHitGround.wav", false, engine),
	collisionSound("Sounds/CoinHitWall.wav", false, engine),
	coinModel("Models/Coin/coin.obj")
{
	this->playerHeight = height;
	this->playerSpeed = 0;
	this->walkingVector = glm::vec3(0.0f, 0.0f, 1.0f);
	this->maze = maze;
	this->soundEngine = engine;
	this->minotaur = minotaur;

	this->footStep.SetVolume(0.2);
	this->dropSound.SetVolume(0.2);
	this->collisionSound.SetVolume(2.0);

	CenterPlayer(); //Space to return to origin

	this->nrOfInventoryCoins = 0;
	this->nrOfWorldCoins = 0;

	// Add startingCoins for the player
	for (int i = 0; i < MAX_NR_OF_COINS; i++)
	{
		this->AddCoinToInventory();	// Incrementing nrOfInventoryCoins
	}
		
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

glm::vec3 Player::GetPos()
{
	return this->transform.GetPos();
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
	return &this->worldCoins[index];
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
			x += 1;
			pingpong = true;
		}
		else
		{
			y += 1;
			pingpong = false;
		}
	}

	glm::vec3 newPos = this->maze->TransformToWorldCoords(glm::vec3(x, 0.0f, y));

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

	// Update sound positions
	footStep.SetPosition(this->GetCamera()->GetCameraPosition());

	// Update the torch
	this->playerTorch.Update(dt,
		this->playerCamera,
		this->walkingVector, 
		this->boundingBoxHalfSize);

	// Update coins
	this->UpdateCoins(dt);

	this->CheckIfWin();

	glm::vec3 minoPos = this->minotaur->GetTransform().GetPos();
	float minoBB = this->minotaur->GetTransform().GetScale().x * 10.0f;

	// Check if player dies
	if ((minoPos.x <= this->transform.GetPos().x + this->boundingBoxHalfSize + minoBB && minoPos.x >= this->transform.GetPos().x - this->boundingBoxHalfSize - minoBB) && 
		(minoPos.z <= this->transform.GetPos().z + this->boundingBoxHalfSize + minoBB && minoPos.z >= this->transform.GetPos().z - this->boundingBoxHalfSize - minoBB))
	{
		EventHandler& EH = EventHandler::GetInstance();
		EH.AddEvent(EVENT_PLAYER_LOSE);
	}

#ifdef DEBUG
	//if (this->playerCamera.GetCameraPosition() != this->playerCamera.GetOldCameraPosition())
	//{
	//	printf("Map position: X:%.2f, Y:%.2f Playerheight:%.2f\n", playerCamera.GetCameraPosition().x, playerCamera.GetCameraPosition().z, playerCamera.GetCameraPosition().y);
	//
	//	std::cout << "Forward Vector! X: " << this->playerCamera.GetForwardVector().x << std::endl;
	//	std::cout << "Forward Vector! Y: " << this->playerCamera.GetForwardVector().y << std::endl;
	//	std::cout << "Forward Vector! Z: " << this->playerCamera.GetForwardVector().z << std::endl;
	//}

		//std::cout << "Forward Vector! X: " << this->playerCamera.GetForwardVector().x << std::endl;
		//std::cout << "Forward Vector! Y: " << this->playerCamera.GetForwardVector().y << std::endl;
		//std::cout << "Forward Vector! Z: " << this->playerCamera.GetForwardVector().z << std::endl;
	//}

#endif
}

void Player::AddCoinToInventory()
{
	// Check if bag is full
	if (this->nrOfInventoryCoins == MAX_NR_OF_COINS)
	{
		// No more coins can be added
		return;
	}

	// Add a coin
	this->inventoryCoins[this->nrOfInventoryCoins].GetTransform()->SetPos(this->transform.GetPos());
	this->inventoryCoins[this->nrOfInventoryCoins].GetTransform()->SetScale(glm::vec3(0.025f));

	// Increment NrOfCoins
	this->nrOfInventoryCoins++;
}

void Player::RemoveCoinFromInventory()
{
	if (this->nrOfInventoryCoins == 0)
	{
		return;
	}
		
	// Removes the last coin in the array
	this->nrOfInventoryCoins--;
}

void Player::DropCoin()
{
	this->AddCoinToWorld(COIN_DROP);
}

void Player::TossCoin()
{
	this->AddCoinToWorld(COIN_TOSS);
}

void Player::PlayWallCollisionSound()
{
	this->collisionSound.Play();
}

void Player::PlayGroundCollisionSound()
{
	this->dropSound.Play();
}

void Player::DrawCoin(unsigned int index, Shader * shader)
{
	this->worldCoins.at(index).Draw(&this->coinModel, shader);
}

void Player::AddCoinToWorld(unsigned int state)
{
	// Check if the player got coins to drop/toss
	if (this->nrOfInventoryCoins == 0)
	{
		return;
	}

	// Check if world can have more coins // DYNAMIC FIX
	if (this->nrOfWorldCoins == 256)
	{
		return;
	}

	// Update the torch so that it is located in front of the player
	glm::vec3 throwPos = this->playerCamera.GetCameraPosition()
		- this->playerCamera.GetRightVector() * 0.075f
		+ this->playerCamera.GetUpVector() * -0.11f;


	Transform transformTmp = this->transform;
	// Set the starting position of the coin to be on the player and set the scale
	transformTmp.SetPos(throwPos);
	transformTmp.SetScale(this->inventoryCoins[this->nrOfInventoryCoins - 1].GetTransform()->GetScale());

	// Set the state (if coin is tossed or dropped)
	Coin coinTmp = Coin(transformTmp, state, this->maze);
	this->worldCoins.push_back(coinTmp);

	// Give coins the initThrowDir
	this->worldCoins.at(this->nrOfWorldCoins).SetVelocity(this->GetCamera()->GetForwardVector());

	this->nrOfWorldCoins++;
	this->nrOfInventoryCoins--;
}

void Player::UpdateCoins(double dt)
{
	// Loop through all worldCoins
	for (int i = 0; i < this->nrOfWorldCoins; i++)
	{
		// Check if the coin should be updated 
		if (this->worldCoins.at(i).IsOnGround() == false)
		{
			// Check the type of update for the coin
			switch (this->worldCoins.at(i).GetCoinState())
			{
			case COIN_DROP:
				this->worldCoins.at(i).UpdateDropCoin(dt);
				break;

			case COIN_TOSS:
				this->worldCoins.at(i).UpdateTossCoin(dt);
				break;

			default:
				#ifdef DEBUG
				std::cout << "Invalid State for coin" << std::endl;
				#endif
				break;
			}

			if (this->worldCoins.at(i).GetWallHit() == true)
			{
				this->PlayWallCollisionSound();
			}

			if (this->worldCoins.at(i).IsOnGround() == true)
			{
				this->PlayGroundCollisionSound();
			}
		}
	}
}

void Player::CheckIfWin()
{
	// Get a vector with only x and z values
	glm::vec2 p2d = glm::vec2(this->transform.GetPos().x, this->transform.GetPos().z);
	glm::vec2 m2d = glm::vec2(this->maze->GetExitWorldPos().x, this->maze->GetExitWorldPos().z);

	glm::vec2 dist2d = glm::abs(p2d - m2d);
	float wallRadius = maze->GetTransform()->GetScale().x / 2.0;

	// Check if player is at exit
	if (dist2d.x <= wallRadius && dist2d.y <= wallRadius && maze->IsExitOpen())
	{
		EventHandler& EH = EventHandler::GetInstance();
		EH.AddEvent(EVENT_PLAYER_WIN);
	}
}
