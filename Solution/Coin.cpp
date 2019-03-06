#include "Coin.h"

Coin::Coin(irrklang::ISoundEngine * engine, Maze * maze)
	:tossSound("Sounds/CoinToss.wav", false, engine),
	dropSound("CoinHitGround/.wav", false, engine),
	collisionSound("Sounds/CoinHitWall.wav", false, engine)
{
	this->tossSound.SetVolume(0.7);
	this->dropSound.SetVolume(0.3);
	this->collisionSound.SetVolume(1.0);

	this->maze = maze;

	this->coinSpeed = 8.0f;
	this->isOnGround = false;
	this->tossed = false;
	this->gravity = glm::vec3(0.0, -9.82, 0.0f);
	this->rotation = glm::vec3(0.0f, 0.0f, 0.0f);
}

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

bool Coin::IsOnGround()
{
	return this->isOnGround;
}

unsigned int Coin::GetCoinState()
{
	return this->coinState;
}

bool Coin::DetectWalls(glm::vec3 newPos, glm::vec3 oldPos, glm::vec3 &velocity)
{
	bool hit = true;

	// Offset because of tessellation
	float tessOffset = this->maze->GetTransform()->GetScale().x * 0.1f;
	if (oldCoinPosition.x < newPos.x)
	{
		newPos.x += tessOffset;
	}
	else if (oldCoinPosition.x > newPos.x)
	{
		newPos.x -= tessOffset;
	}
	tessOffset = this->maze->GetTransform()->GetScale().z * 0.1f;
	if (oldCoinPosition.z < newPos.z)
	{
		newPos.z += tessOffset;
	}
	else if (oldCoinPosition.z > newPos.z)
	{
		newPos.z -= tessOffset;
	}

	// Get wall-type
	Wall type = this->maze->GetWallType(newPos.x, newPos.z);
	glm::vec3 mazeCoords = this->maze->TransformToMazeCoords(glm::vec3(newPos.x, 0.0f, newPos.z));
	float absX = mazeCoords.x;
	float absY = mazeCoords.z;
	
	// Double walls
	if (type == WALL_VERTICAL)
	{
		this->velocity.x *= -1;
		this->rotation = glm::vec3(3.0f, 1.0f, 2.0f);
	}
	else if (type == WALL_HORIZONTAL)
	{
		this->velocity.z *= -1;
		this->rotation = glm::vec3(3.0f, 1.0f, 2.0f);
	}

	// Single walls
	else if (type == WALL_UP || type == WALL_DOWN)
	{
		this->velocity.z *= -1;
		this->rotation = glm::vec3(3.0f, 1.0f, 2.0f);
	}
	else if (type == WALL_RIGHT || type == WALL_LEFT)
	{
		this->velocity.x *= -1;
		this->rotation = glm::vec3(3.0f, 1.0f, 2.0f);
	}

	// Corner walls
	else if (type == WALL_CORNER_LEFT_DOWN) // |__
	{
		float pixelX = floor(absX);
		float pixelY = ceil(absY);

		float diffX = absX - pixelX;
		float diffY = pixelY - absY;

		if (diffX < diffY) // |
		{
			this->velocity.x *= -1;
		}
		else // __
		{
			this->velocity.z *= -1;
		}
		this->rotation = glm::vec3(3.0f, 1.0f, 2.0f);
	}
	else if (type == WALL_CORNER_RIGHT_DOWN) // __|
	{
		float pixelX = ceil(absX);
		float pixelY = ceil(absY);

		float diffX = pixelX - absX;
		float diffY = pixelY - absY;

		if (diffX < diffY) // |
		{
			this->velocity.x *= -1;
		}
		else // __
		{
			this->velocity.z *= -1;
		}
		this->rotation = glm::vec3(3.0f, 1.0f, 2.0f);
	}
	else if (type == WALL_CORNER_RIGHT_UP) // ""|
	{
		float pixelX = ceil(absX);
		float pixelY = floor(absY);

		float diffX = pixelX - absX;
		float diffY = absY - pixelY;

		if (diffX < diffY) // |
		{
			this->velocity.x *= -1;
		}
		else // __
		{
			this->velocity.z *= -1;
		}
		this->rotation = glm::vec3(3.0f, 1.0f, 2.0f);
	}
	else if (type == WALL_CORNER_LEFT_UP) // |""
	{
		float pixelX = floor(absX);
		float pixelY = floor(absY);

		float diffX = absX - pixelX;
		float diffY = absY - pixelY;

		if (diffX < diffY) // |
		{
			this->velocity.x *= -1;
		}
		else // __
		{
			this->velocity.z *= -1;
		}
		this->rotation = glm::vec3(3.0f, 1.0f, 2.0f);
	}

	// End walls
	else if (type == WALL_END_DOWN)
	{
		float pixelX = floor(absX);
		float nextPixelX = ceil(absX);
		float pixelY = ceil(absY);

		float diffX = absX - pixelX;
		float diffNextX = nextPixelX - absX;
		float diffY = pixelY - absY;

		if (diffY < diffX && diffY < diffNextX) // |
		{
			this->velocity.z *= -1;
		}
		else // __
		{
			this->velocity.x *= -1;
		}
		this->rotation = glm::vec3(3.0f, 1.0f, 2.0f);
	}
	else if (type == WALL_END_UP)
	{
		float pixelX = floor(absX);
		float nextPixelX = ceil(absX);
		float pixelY = floor(absY);

		float diffX = absX - pixelX;
		float diffNextX = nextPixelX - absX;
		float diffY = absY - pixelY;

		if (diffY < diffX && diffY < diffNextX) // |
		{
			this->velocity.z *= -1;
		}
		else // __
		{
			this->velocity.x *= -1;
		}
		this->rotation = glm::vec3(3.0f, 1.0f, 2.0f);
	}
	else if (type == WALL_END_LEFT)
	{
		float pixelX = floor(absX);
		float pixelY = floor(absY);
		float nextPixelY = ceil(absY);

		float diffX = absX - pixelX;
		float diffY = absY - pixelY;
		float diffNextY = nextPixelY - absY;

		if (diffY > diffX && diffNextY > diffX) // |
		{
			this->velocity.x *= -1;
		}
		else // __
		{
			this->velocity.z *= -1;
		}
		this->rotation = glm::vec3(3.0f, 1.0f, 2.0f);
	}
	else if (type == WALL_END_RIGHT)
	{
		float pixelX = ceil(absX);
		float pixelY = floor(absY);
		float nextPixelY = ceil(absY);

		float diffX = pixelX - absX;
		float diffY = absY - pixelY;
		float diffNextY = nextPixelY - absY;

		if (diffY > diffX && diffNextY > diffX) // |
		{
			this->velocity.x *= -1;
		}
		else // __
		{
			this->velocity.z *= -1;
		}
		this->rotation = glm::vec3(3.0f, 1.0f, 2.0f);
	}

	// If the coin hits the exact corner
	float diffX = ceil(absX) - absX;
	float diffY = ceil(absY) - absY;
	float diffX2 = absX - floor(absX);
	float diffY2 = absY - floor(absY);
	if (diffX == diffY || diffX2 == diffY2)
	{
		this->velocity.x *= -1;
		this->velocity.z *= -1;
		this->rotation = glm::vec3(3.0f, 1.0f, 2.0f);
	}

	// If no wall detection was found
	if (type != NO_WALL) 
	{
		hit = false;
	}

	return hit;
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

void Coin::SetEngine(irrklang::ISoundEngine * engine)
{
	this->engine = engine;
}

bool Coin::UpdateDropCoin(double dt)
{
	glm::vec3 rotation = glm::vec3(3.0f, 1.0f, 2.0f);

	// Check if the coin allready is on the ground
	if (this->transform.GetPos().y < 0.01f)
	{
		this->transform.SetRot(glm::vec3(0.0f, 0.0f, 0.0f));
		this->transform.SetPos(glm::vec3(this->oldCoinPosition.x, 0.1f, this->oldCoinPosition.z));
		this->dropSound.Play();
		this->isOnGround = true;
		return this->isOnGround;
	}

	this->GetTransform()->GetRot() += rotation;

	this->velocity += this->gravity * float(dt);
	this->transform.SetPos(this->transform.GetPos() + this->velocity * float(dt) + (this->gravity * pow(float(dt), 2.f) / 2.f));

	this->oldCoinPosition = this->transform.GetPos();

	return this->isOnGround;
}

bool Coin::UpdateTossCoin(double dt)
{
	if (tossed == false)
	{
		tossSound.Play();
		tossed = true;
	}

	// Check if the coin allready is on the ground
	if (this->transform.GetPos().y < 0.01f)
	{
		// Removes "sliding"
		glm::vec3 offset = this->velocity * float(dt);

		this->transform.SetRot(glm::vec3(0.0f, 0.0f, 0.0f));
		this->transform.SetPos(glm::vec3(this->oldCoinPosition.x - offset.x, 0.1f, this->oldCoinPosition.z - offset.z));
		this->dropSound.Play();
		this->isOnGround = true;
		return this->isOnGround;
	}

	this->GetTransform()->GetRot() += this->rotation;

	// Update velocity (V = v0 + a*dt)
	this->velocity += this->gravity * float(dt);

	// Update position  (S = s0 + V*dt + (at^2)/2)
	glm::vec3 newPos = this->transform.GetPos() + this->velocity * float(dt) + (this->gravity * pow(float(dt), 2.0f) / 2.0f);

	// Detect collision
	if (this->DetectWalls(newPos, this->oldCoinPosition, this->velocity) == true)
	{
		collisionSound.Play();
	}

	this->transform.SetPos(newPos);

	this->oldCoinPosition = newPos;

	return this->isOnGround;
}

void Coin::Draw(Model * coinModel, Shader * shader)
{
	coinModel->Draw(shader);
}
