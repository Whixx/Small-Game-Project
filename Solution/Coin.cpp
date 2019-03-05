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

void Coin::SetMaze(Maze *maze)
{
	this->maze = maze;
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
		this->transform.SetPos(glm::vec3(this->oldCoinPosition.x, 0.1f, this->oldCoinPosition.z));
		this->isOnGround = true;
		return this->isOnGround;
	}

	// Update velocity (V = v0 + a*dt)
	this->velocity += this->gravity * float(dt);

	// Update position  (S = s0 + V*dt + (at^2)/2)
	glm::vec3 newPos = this->transform.GetPos() + this->velocity * float(dt) + (this->gravity * pow(float(dt), 2.f) / 2.f);
	glm::vec3 oldPos = this->oldCoinPosition;

	// Detect collision
	if (this->maze->IsWallAtWorld(newPos.x, newPos.z) == true)
	{
		// Get wall 
		Wall type = this->maze->GetWallType(newPos.x, newPos.z);

		// Double walls
		if (type == WALL_VERTICAL)
		{
			printf("You hit double vertical walls!\n");
			this->velocity.z *= -1;
		}
		else if (type == WALL_HORIZONTAL)
		{
			printf("You hit double horizontal walls!\n");
			this->velocity.x *= -1;
		}

		// Single walls
		else if (type == WALL_UP || type == WALL_DOWN)
		{
			printf("You hit a horizontal wall!\n");
			this->velocity.z *= -1;
		}
		else if (type == WALL_RIGHT || type == WALL_LEFT)
		{
			printf("You hit a vertical wall!\n");
			this->velocity.x *= -1;
		}

		// Corner walls
		else if (type == WALL_CORNER_LEFT_DOWN) // |__
		{
			float absX = abs(newPos.x);
			float absY = abs(newPos.z);

			float pixelX = floor(absX);
			printf("NewPosX: %f\n", absX);
			printf("Pixel x %f\n", pixelX);

			float pixelY = ceil(absY);
			printf("NewPosY: %f\n", absY);
			printf("Pixel y %f\n", pixelY);

			float diffX = absX - pixelX;
			float diffY = pixelY - absY;
			printf("Difference X: %f,   Y: %f\n", diffX, diffY);

			if (diffX < diffY) // |
			{
				printf("You hit a horizontal corner!\n");
				this->velocity.x *= -1;
			}
			else // __
			{
				printf("You hit a vertical corner!\n");
				this->velocity.z *= -1;
			}
		}
		else if (type == WALL_CORNER_RIGHT_DOWN) // __|
		{
			float absX = abs(newPos.x);
			float absY = abs(newPos.z);

			float pixelX = ceil(absX);
			printf("NewPosX: %f\n", absX);
			printf("Pixel x %f\n", pixelX);

			float pixelY = ceil(absY);
			printf("NewPosY: %f\n", absY);
			printf("Pixel y %f\n", pixelY);

			float diffX = pixelX - absX;
			float diffY = pixelY - absY;
			printf("Difference X: %f,   Y: %f\n", diffX, diffY);

			if (diffX < diffY) // |
			{
				printf("You hit a horizontal corner!\n");
				this->velocity.x *= -1;
			}
			else // __
			{
				printf("You hit a vertical corner!\n");
				this->velocity.z *= -1;
			}
		}
		else if (type == WALL_CORNER_RIGHT_UP) // ""|
		{
			float absX = abs(newPos.x);
			float absY = abs(newPos.z);

			float pixelX = ceil(absX);
			printf("NewPosX: %f\n", absX);
			printf("Pixel x %f\n", pixelX);

			float pixelY = floor(absY);
			printf("NewPosY: %f\n", absY);
			printf("Pixel y %f\n", pixelY);

			float diffX = pixelX - absX;
			float diffY = absY - pixelY;
			printf("Difference X: %f,   Y: %f\n", diffX, diffY);

			if (diffX < diffY) // |
			{
				printf("You hit a horizontal corner!\n");
				this->velocity.x *= -1;
			}
			else // __
			{
				printf("You hit a vertical corner!\n");
				this->velocity.z *= -1;
			}
		}
		else if (type == WALL_CORNER_LEFT_UP) // |""
		{
			float absX = abs(newPos.x);
			float absY = abs(newPos.z);

			float pixelX = floor(absX);
			printf("NewPosX: %f\n", absX);
			printf("Pixel x %f\n", pixelX);

			float pixelY = floor(absY);
			printf("NewPosY: %f\n", absY);
			printf("Pixel y %f\n", pixelY);

			float diffX = absX - pixelX;
			float diffY = absY - pixelY;
			printf("Difference X: %f,   Y: %f\n", diffX, diffY);

			if (diffX < diffY) // |
			{
				printf("You hit a horizontal corner!\n");
				this->velocity.x *= -1;
			}
			else // __
			{
				printf("You hit a vertical corner!\n");
				this->velocity.z *= -1;
			}
		}

		// End walls
		/*else if (type == WALL_END_DOWN || type == WALL_END_UP)
		{
			if (this->maze->IsWallAtWorld(oldPos.x, oldPos.z - 1.0f) == true || this->maze->IsWallAtWorld(newPos.x + 1.0f, newPos.z) == true) // | |
			{
				this->velocity.z *= -1;
			}
			else // __
			{
				this->velocity.x *= -1;
			}
		}
		else if (type == WALL_END_LEFT || type == WALL_END_RIGHT)
		{
			if (this->maze->IsWallAtWorld(newPos.x - 1.0f, newPos.z) == true || this->maze->IsWallAtWorld(newPos.x + 1.0f, newPos.z) == true) // | |
			{
				this->velocity.z *= -1;
			}
			else // __
			{
				this->velocity.x *= -1;
			}
		}*/
	}

	this->transform.SetPos(newPos);

	this->oldCoinPosition = this->transform.GetPos();
	return this->isOnGround;
}

void Coin::Draw(Model * coinModel, Shader * shader)
{
	coinModel->Draw(shader);
}
