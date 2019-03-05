#include "Coin.h"

Coin::Coin()
{
	this->coinSpeed = 10.0f;
	this->isOnGround = false;
	this->gravity = glm::vec3(0.0, -9.82, 0.0f);
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

void Coin::SetVelocity(glm::vec3 initThrowDir)
{
	if (this->coinState == COIN_DROP)
	{
		this->velocity = glm::vec3(0.0f, -1.0f, 0.0f) * float(this->coinSpeed);
	}
	else if (this->coinState == COIN_TOSS)
	{
		this->velocity = initThrowDir * float(this->coinSpeed);
	}
}

bool Coin::UpdateDropCoin(double dt)
{
	// Check if the coin allready is on the ground
	if (this->transform.GetPos().y < 0.01)
	{
		this->transform.SetPos(glm::vec3(this->oldCoinPosition.x, 0, this->oldCoinPosition.z));
		this->isOnGround = true;
		return this->isOnGround;
	}

	this->velocity += this->gravity * float(dt);

	this->transform.SetPos(this->transform.GetPos() + this->velocity * float(dt) + (this->gravity * pow(float(dt), 2.f) / 2.f));

	this->oldCoinPosition = this->transform.GetPos();

	return this->isOnGround;
}

bool Coin::UpdateTossCoin(double dt)
{
	// Check if the coin allready is on the ground
	if (this->transform.GetPos().y < 0.01)
	{
		this->transform.SetPos(glm::vec3(this->oldCoinPosition.x, 0, this->oldCoinPosition.z));
		this->isOnGround = true;
		return this->isOnGround;
	}

	// Update velocity	(V = v0 + a*dt)
	this->velocity += this->gravity * float(dt);

	// Update position  (S = s0 + V*dt + (at^2)/2)
	this->transform.SetPos(this->transform.GetPos() + this->velocity * float(dt) + (this->gravity * pow(float(dt), 2.f) / 2.f));

	this->oldCoinPosition = this->transform.GetPos();
	return this->isOnGround;
}

void Coin::Draw(Model * coinModel, Shader * shader)
{
	coinModel->Draw(shader);
}
