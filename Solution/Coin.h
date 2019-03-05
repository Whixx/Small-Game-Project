#ifndef COIN_H
#define COIN_H

#include <glew\glew.h>
#include "Transform.h"
#include "Model.h"
#include "Maze.h"

enum COIN_STATE
{
	COIN_DROP,
	COIN_TOSS,
	NR_OF_COIN_STATES
};

class Coin
{
public:
	Coin();
	~Coin();

	Transform *GetTransform();
	bool IsOnGround();
	unsigned int GetCoinState();

	void SetCoinState(unsigned int state);
	void SetVelocity(glm::vec3 initThrowDir);

	bool UpdateDropCoin(double dt);
	bool UpdateTossCoin(double dt);

	void Draw(Model * coinModel, Shader * shader);
private:
	Transform transform;

	bool isOnGround;
	unsigned int coinState;

	glm::vec3 velocity;
	double coinSpeed;
	glm::vec3 gravity;
	glm::vec3 oldCoinPosition;

	//Maze maze;
};

#endif