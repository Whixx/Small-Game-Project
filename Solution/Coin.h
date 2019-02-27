#ifndef COIN_H
#define COIN_H

#include <glew\glew.h>
#include "Transform.h"
#include "Model.h"

class Coin
{
public:
	Coin();
	~Coin();

	void SetIsInBag(bool isInBag);

	Transform *GetTransform();
	bool IsInBag();

	void Draw(Model * coinModel, Shader * shader);
private:
	Transform transform;
	bool isInBag;
};

#endif