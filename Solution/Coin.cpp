#include "Coin.h"

Coin::Coin()
{
	this->movementSpeed = glm::vec3(0.0f);
	this->isOnGround = false;
}

Coin::~Coin()
{
}

Transform * Coin::GetTransform()
{
	return &this->transform;
}

bool Coin::IsOnGround()
{
	return this->isOnGround;
}

unsigned int Coin::GetCoinState()
{
	return this->coinState;
}

void Coin::SetCoinState(unsigned int state)
{
	this->coinState = state;
}

bool Coin::UpdateDropCoin(double dt)
{
	// Check if the coin allready is on the ground
	if (this->transform.GetPos().y < 0.01)
	{
		this->transform.SetPos(glm::vec3(this->transform.GetPos().x, 0.0f, this->transform.GetPos().z));
		this->isOnGround = true;
		return true;
	}

	// v = v0 + at
	this->movementSpeed.y = this->movementSpeed.y + 9.82 * dt;
	this->transform.SetPos(glm::vec3(this->transform.GetPos().x, this->transform.GetPos().y - this->movementSpeed.y * 0.01, this->transform.GetPos().z));

	return false;
}

void Coin::Draw(Model * coinModel, Shader * shader)
{
	coinModel->Draw(shader);
}
