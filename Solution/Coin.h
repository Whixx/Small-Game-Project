#ifndef COIN_H
#define COIN_H

#include <glew\glew.h>
#include "Transform.h"
#include "Model.h"

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

	bool UpdateDropCoin(double dt);

	void Draw(Model * coinModel, Shader * shader);
private:
	Transform transform;

	bool isOnGround;
	unsigned int coinState;
	glm::vec3 movementSpeed;
};

#endif