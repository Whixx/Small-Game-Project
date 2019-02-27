#include "Coin.h"

Coin::Coin()
{
}

//Coin & Coin::operator=(const Coin & other)
//{
//	// TODO: insert return statement here
//}

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
