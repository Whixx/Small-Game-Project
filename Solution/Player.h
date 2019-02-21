#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "Transform.h"
#include "Camera.h"
#include "Display.h"
#include "Torch.h"
#include "SoundHandler.h"
#include "Maze.h"

#define PI 3.1415926535

using namespace std;

class Player
{
public:
	// Default constructor
	Player(float height, float fov, float near, float far, Maze * maze, irrklang::ISoundEngine* engine, PointLightHandler* PLH);
	virtual ~Player();
	
	float GetPlayerSpeed();
	float GetPlayerHeight();
	glm::vec3 GetWalkingVector();
	Camera* GetCamera();
	Torch* GetTorch();

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
	void DetectCollision(glm::vec3 newPos, glm::vec3 oldPos);
	void CenterPlayer();
	void UpdateMouse(const glm::vec2& newMousePosition, float elapsedTime);
	void Update(double dt);
	
private:
	Camera playerCamera;
	Transform transform;
	Torch playerTorch;
	
	Maze* maze;
	float playerSpeed;
	float playerHeight;
	const float boundingBoxHalfSize = 0.15f;
	glm::vec3 walkingVector;

	SoundHandler footStep;

};

#endif //DISPLAY_H
