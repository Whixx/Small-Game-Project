#include "Minotaur.h"

Minotaur::Minotaur(irrklang::ISoundEngine * engine, std::vector<std::vector<int>> mazeGrid, Maze* maze)
	:model("Models/Deer/deer.obj"),
	stepSound("Sounds/minotaurstep.ogg", false, engine),
	growlSound("Sounds/minotaurgrowl.wav", false, engine)
{
	this->transform.GetScale() = glm::vec3(0.01f, 0.01f, 0.01f);
	this->transform.GetPos() = glm::vec3(0.0f, 2.0f, 0.0f);
	this->movementSpeed = 1 * this->transform.GetScale().y;
	this->mazeGrid = mazeGrid;
	this->maze = maze;
	this->spawnOnFloor();

	this->destination = glm::vec2(
		this->transform.GetPos().x,
		this->transform.GetPos().z);
}

Minotaur::~Minotaur()
{
}

Transform Minotaur::GetTransform()
{
	return this->transform;
}

void Minotaur::Update(glm::vec3 playerPos)
{

	// If a path is not available
	if (generatedPath.empty())
	{
		// Choose a location in the player-area at random
		glm::vec2 randomPos =
			glm::vec2(	playerPos.x + rand() % (this->searchArea * 2) - this->searchArea,
						playerPos.y + rand() % (this->searchArea * 2) - this->searchArea);
		// Generate path between current location and goal location
		
		GeneratePath(this->transform.GetPos().z, this->transform.GetPos().x, 0, 1);

		// Play growl sound
		growlSound.Play();
	}
	
	// Set the destination to the next tile on the path
	if (this->destination.x == this->transform.GetPos().x && this->destination.y == this->transform.GetPos().z)
	{
		this->destination = generatedPath.back();
		generatedPath.pop_back();
	}
		
	// Move towards the current destination
	Move();
	// Play step sound
	this->stepSound.Play();
}

void Minotaur::GeneratePath(int startY, int startX, int destinationY, int destinationX)
{
	// make a copy of grid to write the path in a new image
	std::vector<std::vector<int>> newGrid = this->mazeGrid;

	std::vector<distance> distances;
	// distance starts with 3, because the grid is already filled with 0, 1 for paths/walls and 2 is reserved to mark solution path
	int distanceCount = 3;
	distances.push_back({ startY, startX });
	bool newDistance = true;
	int y = 0;
	int x = 0;
	int path = 0;
	int wall = 1;
	int solution = 2;

	// go to "frontier" and save distance (steps) from start
	while (newDistance)
	{
		newDistance = false;
		int distanceMax = distances.size();
		distanceCount++;

		for (int i = 0; i < distanceMax; i++)
		{
			y = distances[0].y;
			x = distances[0].x;

			newGrid[y][x] = distanceCount;

			// save if north is path
			if ((y > 0) && (path == newGrid[y - 1][x]))
			{
				distances.push_back({ y - 1, x });
				newDistance = true;
			}
			// save if south is path
			if (((y + 1) < newGrid.size()) && (path == newGrid[y + 1][x]))
			{
				distances.push_back({ y + 1, x });
				newDistance = true;
			}
			// save if west is path
			if ((x > 0) && (path == newGrid[y][x - 1]))
			{
				distances.push_back({ y, x - 1 });
				newDistance = true;
			}
			// save if east is path
			if (((x + 1) < newGrid[0].size()) && (path == newGrid[y][x + 1]))
			{
				distances.push_back({ y, x + 1 });
				newDistance = true;
			}

			// stop at destination
			if ((y == destinationY) && (x == destinationX))
			{
				newDistance = false;
				break;
			}
			distances.erase(distances.begin());
		}
	}

	// backtrack to the start
	y = destinationY;
	x = destinationX;
	distanceCount = newGrid[y][x];
	generatedPath.push_back(glm::vec2(x, y));

	while (distanceCount != 3)
	{
		// mark the path back as solution and send coordinates to path variable to be used by the minotaur
		newGrid[y][x] = solution;

		distanceCount--;
		if ((y > 0) && (distanceCount == newGrid[y - 1][x]))
		{
			y--;
			generatedPath.push_back(glm::vec2(x, y));
		}
		else if (((y + 1) < newGrid.size()) && (distanceCount == newGrid[y + 1][x]))
		{
			y++;
			generatedPath.push_back(glm::vec2(x, y));
		}
		else if ((x > 0) && (distanceCount == newGrid[y][x - 1]))
		{
			x--;
			generatedPath.push_back(glm::vec2(x, y));
		}
		else if (((x + 1) < newGrid[0].size()) && (distanceCount == newGrid[y][x + 1]))
		{
			x++;
			generatedPath.push_back(glm::vec2(x, y));
		}
		else
		{
			// nothing
		}
	}

	//// clear path of unused distance numbers
	//for (int y = 0; y < this->redPath.size(); y++)
	//{
	//	for (int x = 0; x < this->redPath[y].size(); x++)
	//	{
	//		if ((wall != this->redPath[y][x]) && (solution != this->redPath[y][x]))
	//		{
	//			this->redPath[y][x] = path;
	//		}
	//	}
	//}
}

void Minotaur::Move()
{
	// Identify the current position direction
	glm::vec2 direction = glm::vec2(
		this->destination.x - this->transform.GetPos().x,
		this->destination.y - this->transform.GetPos().z);
	//glm::vec3 = maze->TransformToWorldCoords(this->transform.GetPos());
	// If we are not walking past the destination
	if (glm::length(direction) > glm::length(this->movementSpeed*glm::normalize(direction)))
	{
		this->transform.GetPos().x += this->movementSpeed*glm::normalize(direction).x;
		this->transform.GetPos().z += this->movementSpeed*glm::normalize(direction).y;
	}
	else	// Else we are about to walk past the destination, which means that we have arrived
	{
		this->transform.GetPos().x = this->destination.x;
		this->transform.GetPos().z = this->destination.y;
	}
}

void Minotaur::spawnOnFloor()
{
	float x = 0.0f;
	float y = 0.0f;

	// NOT NEEDED Transform world coords to texture coords. ( 1 pixel on texture corresponds to 1.0, origo is (0, 0) for both spaces

	bool pingpong = false;
	while (this->maze->IsWallAtWorld(x, y) == true)
	{
		//If wall, move start position
		if (pingpong = false)
		{
			x += 1.5f;
			pingpong = true;
		}
		else
		{
			y += 1.5f;
			pingpong = false;
		}
	}

	this->transform.SetPos(glm::vec3(x, 1, y));
}

void Minotaur::Draw(Shader * shader)
{
	this->model.Draw(shader);
}
