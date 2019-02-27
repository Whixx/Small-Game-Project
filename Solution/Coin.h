#ifndef COIN_H
#define COIN_H

#include <glew\glew.h>
#include "Transform.h"
#include "Model.h"

class Coin
{
public:
	Coin();
	//Coin& operator=(const Coin &other);
	~Coin();

	Transform *GetTransform();

	void Draw(Model * coinModel, Shader * shader);
private:
	Transform transform;
};

#endif