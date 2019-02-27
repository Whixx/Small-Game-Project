#include "Coin.h"

Coin::Coin()
{
	this->isInBag = true;
}

Coin::~Coin()
{
}

void Coin::SetIsInBag(bool isInBag)
{
	this->isInBag = isInBag;
}

Transform * Coin::GetTransform()
{
	return &this->transform;
}

bool Coin::IsInBag()
{
	return this->isInBag;
}

void Coin::Draw(Model * coinModel, Shader * shader)
{
	coinModel->Draw(shader);
}
