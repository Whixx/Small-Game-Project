#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "Transform.h"
#include "Camera.h"
#include "Display.h"
#include "Maze.h"

#define PI 3.1415926535

using namespace std;

class Player
{
public:
	// Default constructor
	Player(float height, float fov, float near, float far);
	virtual ~Player();
	
	float GetPlayerSpeed();
	float GetPlayerHeight();
	glm::vec3 GetWalkingVector();
	Camera* GetCamera();

	void SetPlayerHeight(float height);
	void SetPlayerSpeed(float speed);
	void moveForward(float elapsedTime);
	void moveBackward(float elapsedTime);
	void moveRight(float elapsedTime);
	void moveLeft(float elapsedTime);
	void moveUp(float elapsedTime);
	void moveDown(float elapsedTime);
	void updateMouse(const glm::vec2& newMousePosition, float elapsedTime);
	void Update(double dt);
	
private:
	Camera playerCamera;
	Transform transform;

	Maze* maze;
	float playerSpeed;
	float playerHeight;
	glm::vec3 walkingVector;
};

#endif //DISPLAY_H