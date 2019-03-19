#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "Transform.h"
#include "Camera.h"
#include "Display.h"
#include "Torch.h"
#include "Sound.h"
#include "Maze.h"
#include "Coin.h"
#include "EventHandler.h"
#include "Minotaur.h"

#define PI 3.1415926535
const unsigned int MAX_NR_OF_COINS = 10;

using namespace std;

class Player
{
public:
	// Default constructor
	Player(float height, float fov, float near, float far, Maze * maze, irrklang::ISoundEngine* engine, PointLightHandler* PLH, float torchSize, Minotaur * minotaur);
	virtual ~Player();
	
	glm::vec3 GetPos();
	float GetPlayerSpeed();
	float GetPlayerHeight();
	glm::vec3 GetWalkingVector();
	glm::vec3 DetectCollision(glm::vec3 newPos, glm::vec3 oldPos);
	Camera* GetCamera();
	Torch* GetTorch();
	Coin * GetInventoryCoin(unsigned int index);
	Coin * GetWorldCoin(unsigned int index);
	unsigned int GetNrOfInventoryCoins();
	unsigned int GetNrOfWorldCoins();
	void ResetCoins();	// reset number of coins to 10

	void SetPlayerHeight(float height);
	void SetPlayerSpeed(float speed);
	void MoveDiagonalRightUp(float elapsedTime);
	void MoveDiagonalLeftUp(float elapsedTime);
	void MoveDiagonalRightDown(float elapsedTime);
	void MoveDiagonalLeftDown(float elapsedTime);
	void MoveForward(float elapsedTime);
	void MoveBackward(float elapsedTime);
	void MoveRight(float elapsedTime);
	void MoveLeft(float elapsedTime);
	void MoveUp(float elapsedTime);
	void MoveDown(float elapsedTime);
	void CenterPlayer();
	void UpdateMouse(const glm::vec2& newMousePosition, float elapsedTime);
	void Update(double dt);
	void UpdateOnlyTorch(double dt);

	// AddCoin and RemoveCoin adds/removes on the last index in the coinArray
	// To be used when throwing and laying coins
	void AddCoinToInventory();
	void RemoveCoinFromInventory();

	// Coin mechanics
	void DropCoin();
	void TossCoin();
	bool PickUpCoin();

	void SpawnCoinAtMinotaur();

	virtual void PlayWallCollisionSound(unsigned int index);
	void PlayGroundCollisionSound(unsigned int index);
	
	void DrawCoin(unsigned int index, Shader * shader);

	// test variable
	int printCounter = 0;

	void ResetPlayer(Maze* maze);

private:
	Camera playerCamera;
	Transform transform;
	Torch playerTorch;

	Model coinModel;
	irrklang::ISoundEngine* soundEngine;

	Coin inventoryCoins[MAX_NR_OF_COINS];	// Coins in the players inventory
	std::vector<Coin> worldCoins;				// Coins on the ground in the world // DYNAMIC FIX

	unsigned int nrOfInventoryCoins;
	unsigned int nrOfWorldCoins;
	
	Maze* maze;
	float playerSpeed;
	float playerHeight;
	const float boundingBoxHalfSize = 0.15f;
	glm::vec3 walkingVector;

	Minotaur * minotaur;

	Sound footStep;
	Sound dropSound;
	Sound collisionSound;

	// Private Functions
	void AddCoinToWorld(unsigned int state);
	void UpdateCoins(double dt);
	// FindNearbyCoin returns the index of the coin in the worldCoinsVector
	int FindNearbyCoin();
	void CheckIfWin();
	void CheckIfLoose();
};

#endif //DISPLAY_H
