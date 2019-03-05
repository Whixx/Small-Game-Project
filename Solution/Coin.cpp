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

glm::vec3 Coin::DetectWalls(glm::vec3 newPos)
{
	// Get wall-type
	Wall type = this->maze->GetWallType(newPos.x, newPos.z);
	float offset = this->maze->GetTransform()->GetScale().x * 0.05f;
	newPos.x -= offset;
	newPos.y -= offset;

	// Double walls
	if (type == WALL_VERTICAL)
	{
		this->velocity.x *= -1;
	}
	else if (type == WALL_HORIZONTAL)
	{
		this->velocity.z *= -1;
	}

	// Single walls
	else if (type == WALL_UP || type == WALL_DOWN)
	{
		this->velocity.z *= -1;
	}
	else if (type == WALL_RIGHT || type == WALL_LEFT)
	{
		this->velocity.x *= -1;
	}

	// Corner walls
	else if (type == WALL_CORNER_LEFT_DOWN) // |__
	{
		float absX = newPos.x + this->maze->GetMazeWidth();
		float absY = newPos.z + this->maze->GetMazeHeight();

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
	}
	else if (type == WALL_CORNER_RIGHT_DOWN) // __|
	{
		float absX = newPos.x + this->maze->GetMazeWidth();
		float absY = newPos.z + this->maze->GetMazeHeight();

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
	}
	else if (type == WALL_CORNER_RIGHT_UP) // ""|
	{
		float absX = newPos.x + this->maze->GetMazeWidth();
		float absY = newPos.z + this->maze->GetMazeHeight();

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
	}
	else if (type == WALL_CORNER_LEFT_UP) // |""
	{
		float absX = newPos.x + this->maze->GetMazeWidth();
		float absY = newPos.z + this->maze->GetMazeHeight();

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
	}

	// End walls
	else if (type == WALL_END_DOWN)
	{
		float absX = newPos.x + this->maze->GetMazeWidth();
		float absY = newPos.z + this->maze->GetMazeHeight();

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
	}
	else if (type == WALL_END_UP)
	{
		float absX = newPos.x + this->maze->GetMazeWidth();
		float absY = newPos.z + this->maze->GetMazeHeight();

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
	}
	else if (type == WALL_END_LEFT)
	{
		float absX = newPos.x + this->maze->GetMazeWidth();
		float absY = newPos.z + this->maze->GetMazeHeight();

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
	}
	else if (type == WALL_END_RIGHT)
	{
		float absX = newPos.x + this->maze->GetMazeWidth();
		float absY = newPos.z + this->maze->GetMazeHeight();

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
	}

	return this->velocity;
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

void Coin::SetMaze(Maze *maze)
{
	this->maze = maze;
}

bool Coin::UpdateDropCoin(double dt)
{
	glm::vec3 rotation = glm::vec3(3.0f, 1.0f, 2.0f);

	// Check if the coin allready is on the ground
	if (this->transform.GetPos().y < 0.01f)
	{
		this->transform.SetRot(glm::vec3(0.0f, 0.0f, 0.0f));
		this->transform.SetPos(glm::vec3(this->oldCoinPosition.x, 0.1f, this->oldCoinPosition.z));
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
	glm::vec3 rotation = glm::vec3(3.0f, 1.0f, 2.0f);

	// Check if the coin allready is on the ground
	if (this->transform.GetPos().y < 0.01)
	{
		this->transform.SetRot(glm::vec3(0.0f, 0.0f, 0.0f));
		this->transform.SetPos(glm::vec3(this->oldCoinPosition.x, 0.1f, this->oldCoinPosition.z));
		this->isOnGround = true;
		return this->isOnGround;
	}

	this->GetTransform()->GetRot() += rotation;

	// Update velocity (V = v0 + a*dt)
	this->velocity += this->gravity * float(dt);

	// Update position  (S = s0 + V*dt + (at^2)/2)
	glm::vec3 newPos = this->transform.GetPos() + this->velocity * float(dt) + (this->gravity * pow(float(dt), 2.f) / 2.f);

	// Detect collision
	this->velocity = this->DetectWalls(newPos);

	this->transform.SetPos(newPos);

	this->oldCoinPosition = this->transform.GetPos();
	return this->isOnGround;
}

void Coin::Draw(Model * coinModel, Shader * shader)
{
	coinModel->Draw(shader);
}
