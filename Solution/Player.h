#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "Transform.h"
#include "Camera.h"
#include "Display.h"
#include "Maze.h"
#include "Torch.h"
#include "Sound.h"

#define PI 3.1415926535

using namespace std;

class Player
{
public:
	// Default constructor
	Player(float height, float fov, float near, float far, Mesh *mesh, Texture *texture);
	virtual ~Player();
	
	float GetPlayerSpeed();
	float GetPlayerHeight();
	glm::vec3 GetWalkingVector();
	Camera* GetCamera();
	Torch GetTorch();

	void SetPlayerHeight(float height);
	void SetPlayerSpeed(float speed);
	void MoveForward(float elapsedTime);
	void MoveBackward(float elapsedTime);
	void MoveRight(float elapsedTime);
	void MoveLeft(float elapsedTime);
	void MoveUp(float elapsedTime);
	void MoveDown(float elapsedTime);
	void UpdateMouse(const glm::vec2& newMousePosition, float elapsedTime);
	void Update(double dt);

	Sound sound;
	
private:
	Camera playerCamera;
	Transform transform;
	Torch playerTorch;
	
	Maze* maze;
	float playerSpeed;
	float playerHeight;
	glm::vec3 walkingVector;

};

#endif //DISPLAY_H