#include "Minotaur.h"
#include "stb_image_write.h"

Minotaur::Minotaur(irrklang::ISoundEngine * engine, std::vector<std::vector<int>> mazeGrid, Maze* maze)
	:model("Models/Deer/deer.obj"),
	stepSound("Sounds/minotaurstep.ogg", false, engine),
	growlSound("Sounds/minotaurgrowl.wav", false, engine)
{
	this->transform.GetScale() = glm::vec3(0.01f, 0.01f, 0.01f);
	this->transform.GetPos() = glm::vec3(0.0f, 1.0f, 0.0f);
	this->movementSpeed = 1 * this->transform.GetScale().y;
	this->mazeGrid = mazeGrid;
	this->maze = maze;

	// Adjust spawn position to a floor-tile
	glm::vec3 currentPos = this->maze->TransformToMazeCoords(this->transform.GetPos());
	glm::vec2 newPos = this->toNearbyFloor(glm::vec2(currentPos.x, currentPos.z));
	currentPos = glm::vec3(newPos.x, this->transform.GetPos().y, newPos.y);
	this->transform.GetPos() = currentPos;

	// Initiate destination to the current position (It will then set the real destination in Update())
	this->destination = glm::vec2(
		this->maze->TransformToMazeCoords(this->transform.GetPos()).x,
		this->maze->TransformToMazeCoords(this->transform.GetPos()).z);
}

Minotaur::~Minotaur()
{
}

Transform Minotaur::GetTransform()
{
	return this->transform;
}

void Minotaur::increaseAgressionLevel()
{
	this->searchArea--;
}

void Minotaur::reactToSound(glm::vec3 soundMapPos)
{
	this->alerted = 3;

	this->lastSoundHeardPos = soundMapPos;

	while (!this->generatedPath.empty())
		this->generatedPath.pop_back();

	glm::vec3 currentPos = this->maze->TransformToMazeCoords(this->transform.GetPos());

	GeneratePath(currentPos.z, currentPos.x, soundMapPos.z, soundMapPos.x);
}

void Minotaur::Update(glm::vec3 playerPos)
{
	glm::vec3 currentPos = this->transform.GetPos();
	glm::vec3 currentPlayerPos = this->maze->TransformToMazeCoords(playerPos);


	// If a path is not available
	if (generatedPath.empty())
	{
		// Calculate the starting position of the path in the maze
		glm::vec3 startPos = this->maze->TransformToMazeCoords(currentPos);
		glm::vec2 endPos;
		glm::vec3 trueEndPos;

		if (this->alerted == 0)
		{
			this->searchArea = 8;

			// Choose a location in the player-area at random
			endPos = glm::vec2(	currentPlayerPos.x + rand() % (this->searchArea * 2) - this->searchArea,
								currentPlayerPos.z + rand() % (this->searchArea * 2) - this->searchArea);
			// Adjust spawn position to a floor-tile
			endPos = this->toNearbyFloor(glm::vec2(endPos.x, endPos.y));
			trueEndPos = this->maze->TransformToMazeCoords(glm::vec3(endPos.x, 0.0f, endPos.y));
		}
		else if (this->alerted > 0)
		{
			this->searchArea = 4;
			this->alerted--;

			// Choose a location in the sound-area at random
			endPos = glm::vec2(	this->lastSoundHeardPos.x + rand() % (this->searchArea * 2) - this->searchArea,
								this->lastSoundHeardPos.z + rand() % (this->searchArea * 2) - this->searchArea);
			// Adjust spawn position to a floor-tile
			endPos = this->toNearbyFloor(glm::vec2(endPos.x, endPos.y));
			trueEndPos = this->maze->TransformToMazeCoords(glm::vec3(endPos.x, 0.0f, endPos.y));
		}


		// Generate path between current location and goal location
		GeneratePath(startPos.z, startPos.x, trueEndPos.z, trueEndPos.x);

		// Play growl sound
		growlSound.Play();
	}

	glm::vec3 worldDestination = this->maze->TransformToWorldCoords(glm::vec3(this->destination.x, 0, this->destination.y));

	// Set the destination to the next tile on the path
	if (worldDestination.x == currentPos.x && worldDestination.z == currentPos.z)
	{
		this->destination = generatedPath.back();
		generatedPath.pop_back();
		std::cout << "(" << this->destination.x << "," << this->destination.y << ")" << endl;
	}
		

	// Move towards the current destination

	// Identify the current direction
	glm::vec2 direction = glm::vec2(
		worldDestination.x - this->transform.GetPos().x,
		worldDestination.z - this->transform.GetPos().z);

	// If we are not walking past the destination
	if (glm::length(direction) > glm::length(this->movementSpeed*glm::normalize(direction)))
	{
		this->transform.GetPos().x += this->movementSpeed*glm::normalize(direction).x;
		this->transform.GetPos().z += this->movementSpeed*glm::normalize(direction).y;
	}
	else	// Else we are about to walk past the destination, which means that we have arrived
	{
		this->transform.GetPos().x = worldDestination.x;
		this->transform.GetPos().z = worldDestination.z;
	}

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
		this->mazeGrid[y][x] = solution;

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

	this->drawPath();

	// clear grid of solution path
	for (int y = 0; y < this->mazeGrid.size(); y++)
	{
		for (int x = 0; x < this->mazeGrid[y].size(); x++)
		{
			if ((wall != this->mazeGrid[y][x])/* && (solution != this->mazeGrid[y][x])*/)
			{
				this->mazeGrid[y][x] = path;
			}
		}
	}

}

void Minotaur::Move()
{
	
}

glm::vec2 Minotaur::toNearbyFloor(glm::vec2 pos)
{
	float x = pos.x;
	float y = pos.y;

	glm::vec3 positionWorld(x, 0, y);
	positionWorld = this->maze->TransformToWorldCoords(positionWorld);
	
	x = positionWorld.x;
	y = positionWorld.z;

	bool pingpong = false;
	while (this->maze->IsWallAtWorld(x, y) == true)
	{
		//If wall, move start position
		if (pingpong = false)
		{
			x += this->maze->GetTransform()->GetScale().x;
			pingpong = true;
		}
		else
		{
			y += this->maze->GetTransform()->GetScale().z;
			pingpong = false;
		}
	}

	return glm::vec2(x, y);
}

void Minotaur::Draw(Shader * shader)
{
	this->model.Draw(shader);
}

void Minotaur::setupColorData()
{
	// set up image vectors to hold color data
// added +1 to make the maze 64*64 pixels for use in shaders
	int newHeight = 64;
	int newWidth = 64;

	image.resize(newHeight);
	for (int y = 0; y < newHeight; y++)
	{
		image[y].resize(newWidth);
		for (int x = 0; x < newWidth; x++)
		{
			image[y][x].resize(3);
		}
	}

	for (int i = 0; i < newWidth; i++)
	{
		image[63][i][0] = 0;
		image[63][i][1] = 0;
		image[63][i][2] = 0;
	}
	for (int i = 0; i < newHeight; i++)
	{
		image[i][63][0] = 0;
		image[i][63][1] = 0;
		image[i][63][2] = 0;
	}

	// sets colors, white for walls, black for path
	for (int y = 0; y < 63; y++)
	{
		for (int x = 0; x < 63; x++)
		{
			if (mazeGrid[y][x] == 1)
			{
				for (int i = 0; i < 3; i++)
				{
					image[y][x][i] = 255;
				}
			}
			else if (mazeGrid[y][x] == 0)
			{
				for (int i = 0; i < 3; i++)
				{
					image[y][x][i] = 0;
				}
			}
			else if (mazeGrid[y][x] == 2)
			{
				image[y][x][0] = 255;
				image[y][x][1] = 0;
				image[y][x][2] = 0;
			}
		}
	}
}

void Minotaur::drawPath()
{
	setupColorData();
	
	std::vector<unsigned char> c;
	for (int y = 0; y < 63 + 1; y++)
	{
		for (int x = 0; x < 63 + 1; x++)
		{
			c.push_back(image[y][x][0]);
			c.push_back(image[y][x][1]);
			c.push_back(image[y][x][2]);
		}
	}
	
	if (!stbi_write_png("MazePNG/MINOTAURPATH.png", 64, 64, 3, c.data(), 3 * (64)))
	{
		std::cout << "ERROR MINOTAURPATH NOT WRITTEN" << std::endl;
	}
	else
	{
		std::cout << "MINOTAURPATH WRITTEN" << std::endl;
	}
}
