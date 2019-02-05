#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "Transform.h"
#include "Camera.h"

using namespace std;

class Player
{
public:
	// Default constructor
	Player();
	virtual ~Player();
	
	float GetPlayerSpeed();
	float GetPlayerHeight();
	glm::vec3 GetWalkingVector();
	void SetPlayerHeight(float height);
	void SetPlayerSpeed(float speed);
	virtual void Update(double dt, Camera camera);

private:
	Transform transform;
	float playerSpeed;
	float playerHeight;
	glm::vec3 walkingVector;
};

#endif //DISPLAY_H