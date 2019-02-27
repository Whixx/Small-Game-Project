#include "Coin.h"

Coin::Coin()
{
}

Coin::~Coin()
{
}

Transform * Coin::GetTransform()
{
	return &this->transform;
}

void Coin::Draw(Model * coinModel, Shader * shader)
{
	coinModel->Draw(shader);
}
