#include "Minotaur.h"
#include "stb_image_write.h"

Minotaur::Minotaur(irrklang::ISoundEngine * engine, std::vector<std::vector<int>> mazeGrid, Maze* maze)
	:model("Models/Minotaur/mino.fbx"),
	stepSound("Sounds/minotaurstep3.ogg", false, engine),
	growlSound("Sounds/minotaurgrowlcut.ogg", false, engine),
	screamSound("Sounds/angryscream2.ogg", false, engine)
{
	this->transform.GetScale() = glm::vec3(0.032f);
	this->movementSpeed = 90 * this->transform.GetScale().y;
	this->mazeGrid = mazeGrid;
	this->maze = maze;
	this->agression = 0;
	this->timePassed = 0;
	this->alerted = false;
	this->interval = 0;
	this->searchArea = 13;

	// Adjust spawn position to a floor-tile
	glm::vec3 currentPos = this->maze->GetExit()->GetExitPos();
	glm::vec2 newPos = this->toNearbyFloor(glm::vec2(currentPos.x, currentPos.z));
	this->transform.GetPos() = this->maze->TransformToWorldCoords(glm::vec3(newPos.x, currentPos.y, newPos.y));

	// Initiate destination to the current position (It will then set the real destination in Update())
	this->destination = glm::vec2(
		int(this->maze->TransformToMazeCoords(this->transform.GetPos()).x),
		int(this->maze->TransformToMazeCoords(this->transform.GetPos()).z));

	this->stepSound.SetMinDistance(3);
	this->growlSound.SetMinDistance(3);
	this->screamSound.SetMinDistance(3);
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

void Minotaur::reactToSound(glm::vec3 soundMazePos)
{
	this->alerted = 3;

	this->lastSoundHeardPos = soundMazePos;

	while (!this->generatedPath.empty())
		this->generatedPath.pop_back();

	glm::vec3 currentPos = this->maze->TransformToMazeCoords(this->transform.GetPos());

	glm::vec2 tempPos = this->toNearbyFloor(glm::vec2(soundMazePos.x, soundMazePos.z));
	soundMazePos = glm::vec3(tempPos.x, 0, tempPos.y);

	GeneratePath(currentPos.z, currentPos.x, soundMazePos.z, soundMazePos.x);
}

void Minotaur::PlayScreamSound()
{
	this->screamSound.Play();
}

void Minotaur::Initialize()
{
	this->model.init();
}

void Minotaur::Update(double dt, glm::vec3 playerPos)
{
	this->model.Update(dt);

	glm::vec3 currentPos = this->transform.GetPos();
	glm::vec3 currentPlayerPos = this->maze->TransformToMazeCoords(playerPos);

	// Update sound positions
	this->growlSound.SetPosition(currentPos);
	this->stepSound.SetPosition(currentPos);
	this->screamSound.SetPosition(currentPos);

	// If a path is not available
	if (generatedPath.empty())
	{
		// Calculate the starting position of the path in the maze
		glm::vec3 startPos = this->maze->TransformToMazeCoords(currentPos);
		glm::vec2 endPos;

		if (this->alerted == 0)
		{
			// Choose a location in the player-area at random
			endPos = glm::vec2(	currentPlayerPos.x + rand() % (this->searchArea * 2) - this->searchArea,
								currentPlayerPos.z + rand() % (this->searchArea * 2) - this->searchArea);
		}
		else if (this->alerted > 0)
		{
			int tempSearchArea = 4;
			this->alerted--;

			// Choose a location in the sound-area at random
			endPos = glm::vec2(	this->lastSoundHeardPos.x + rand() % (tempSearchArea * 2) - tempSearchArea,
								this->lastSoundHeardPos.z + rand() % (tempSearchArea * 2) - tempSearchArea);
		}

		// clamp to edges and adjust spawn position to a floor-tile
		endPos = this->toNearbyFloor(endPos);

		//cout << "endPos X: " << endPos.x << endl;
		//cout << "endPos Y: " << endPos.y << endl;

		// Generate path between current location and goal location
		GeneratePath(startPos.z, startPos.x, endPos.y, endPos.x);
	}

	glm::vec3 worldDestination = this->maze->TransformToWorldCoords(glm::vec3(this->destination.x, 0, this->destination.y));

	// Set the destination to the next tile on the path
	if (worldDestination.x == currentPos.x && worldDestination.z == currentPos.z)
	{
		this->destination = generatedPath.back();
		generatedPath.pop_back();
		//std::cout << "(" << this->destination.x << "," << this->destination.y << ")" << endl;
	}
		

	// Identify the current direction
	glm::vec2 direction = glm::vec2(
		worldDestination.x - this->transform.GetPos().x,
		worldDestination.z - this->transform.GetPos().z);

	// Rotate the minotaur to face the right way
	glm::vec3 rotation = this->transform.GetRot();

	switch ((int)direction.x)
	{
	case -1:
		rotation = glm::vec3(0, glm::radians(-90.0f), 0);
		break;
	case 1:
		rotation = glm::vec3(0, glm::radians(90.0f), 0);
		break;
	}

	switch ((int)direction.y)
	{
	case -1:
		rotation = glm::vec3(0, glm::radians(180.0f), 0);
		break;
	case 1:
		rotation = glm::vec3(0, 0, 0);
		break;
	}

	this->transform.GetRot() = rotation;


	// Move towards the current destination
	// If we are not walking past the destination
	if (glm::length(direction) > glm::length((this->movementSpeed*(float)dt)*glm::normalize(direction)))
	{
		this->transform.GetPos().x += this->movementSpeed*dt*glm::normalize(direction).x;
		this->transform.GetPos().z += this->movementSpeed*dt*glm::normalize(direction).y;
	}
	else	// Else we are about to walk past the destination, which means that we have arrived
	{
		this->transform.GetPos().x = worldDestination.x;
		this->transform.GetPos().z = worldDestination.z;
	}

	// Gradually increase difficulty
	this->timePassed += dt;
	if ((int)this->timePassed > this->interval && this->agression < 13 && this->agression >= 0)
	{
		// Reset time and randomize next interval
		this->interval = 120 + ((rand() % 30) - 15);
		this->timePassed = 0;

		// Increase difficulty
		this->agression++;
		cout << "Increased Agression!";
		switch (this->agression)
		{
		case 2:
			this->searchArea = 12;
			break;
		case 4:
			this->searchArea = 10;
			break;
		case 6:
			this->searchArea = 8;
			break;
		case 8:
			this->searchArea = 6;
			break;
		case 10:
			this->searchArea = 4;
			break;
		case 13:
			this->searchArea = 1;
			break;
		}
		cout << " 'Search Area' is now: " << this->searchArea << endl;

		// Play growlsound
		this->growlSound.Play();
	}

	// Play stepsound
	this->stepSound.Play();
}

void Minotaur::DrawMeshes(Shader * shader)
{
	this->model.DrawMeshes(shader);
}

void Minotaur::GeneratePath(int startY, int startX, int destinationY, int destinationX)
{
	//// TEST print
	//if (mazeGrid[destinationY][destinationX] == 1)
	//	cout << "DESTINATION IS A WALL AND THE GAME WILL CRASH" << endl;
	//else if (mazeGrid[destinationY][destinationX] == 0)
	//	cout << "DESTINATION IS A PATH" << endl;

	// make a copy of grid to write the path in a new image
	std::vector<std::vector<int>> newGrid = this->mazeGrid;

	std::vector<distance> distances;
	// distance starts with 3, because the grid is already filled with 0/1 for paths/walls and 2 is reserved to mark solution path
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
			if ((wall != this->mazeGrid[y][x]))
			{
				this->mazeGrid[y][x] = path;
			}
		}
	}
}

void Minotaur::Move()
{
	
}

glm::vec2 Minotaur::toNearbyFloor(glm::vec2 mazePos)
{
	// Make sure that the position is not outside of the maze
	mazePos = this->ClampToEdges(mazePos);

	int x = mazePos.x;
	int y = mazePos.y;

	if (x > 32 || y > 32)
	{
		std::cout << "ERROR!!" << std::endl;
		std::cout << "x > 32 or y > 32, in ToNearbyFloor in minotaur.cpp" << std::endl;
	}
	if (this->mazeGrid[y][x] == 1)
	{
		x = x - 1;
		y = y - 1;

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				if (this->mazeGrid[y + i][x + j] == 0)
					return glm::vec2(x + j, y + i);
			}
		}
	}
	else
	{
		return glm::vec2(x, y);
	}
}

void Minotaur::Draw(Shader * shader)
{
	this->model.Draw(shader);
}

glm::vec2 Minotaur::ClampToEdges(glm::vec2 mazeCoords)
{
	// make sure destination is within the maze boundary

	int mazeHeight = this->mazeGrid.size();
	int mazeWidth = this->mazeGrid[0].size();
	mazeHeight -= 1;
	mazeWidth -= 1;

	if (mazeCoords.x > mazeWidth)
	{
		mazeCoords.x = mazeWidth;
	}
	if (mazeCoords.x < 1)
	{
		mazeCoords.x = 1;
	}
	if (mazeCoords.y > mazeHeight)
	{
		mazeCoords.y = mazeHeight;
	}
	if (mazeCoords.y < 1)
	{
		mazeCoords.y = 1;
	}

	//cout << "CHANGED TRUEENDPOS X: " << mazeCoords.x << endl;
	//cout << "CHANGED TRUEENDPOS Y: " << mazeCoords.y << endl;

	return mazeCoords;
}

void Minotaur::ResetMinotaur(std::vector<std::vector<int>> newMazeGrid, Maze* maze)
{
	this->mazeGrid = newMazeGrid;
	this->maze = maze;

	this->agression = 0;
	this->timePassed = 0;
	this->alerted = false;
	this->interval = 0;
	this->searchArea = 13;

	glm::vec3 currentPos = this->maze->GetExit()->GetExitPos();
	glm::vec2 newPos = this->toNearbyFloor(glm::vec2(currentPos.x, currentPos.z));
	this->transform.GetPos() = this->maze->TransformToWorldCoords(glm::vec3(newPos.x, currentPos.y, newPos.y));

	// Initiate destination to the current position (It will then set the real destination in Update())
	this->destination = glm::vec2(
		int(this->maze->TransformToMazeCoords(this->transform.GetPos()).x),
		int(this->maze->TransformToMazeCoords(this->transform.GetPos()).z));

	while (!this->generatedPath.empty())
	{
		generatedPath.pop_back();
	}

}

const SkeletonBuffer & Minotaur::GetSkeletonBuffer()
{
	return this->model.GetSkeletonBuffer();
}

const glm::mat4 & Minotaur::GetBoneTransform(unsigned int index)
{
	return this->model.GetBoneTransform(index);
}


void Minotaur::setupColorData()
{
	// set up image vectors to hold color data
	int height = this->mazeGrid.size();
	int width = this->mazeGrid[0].size();

	image.resize(height);
	for (int y = 0; y < height; y++)
	{
		image[y].resize(width);
		for (int x = 0; x < width; x++)
		{
			image[y][x].resize(3);
		}
	}

	// sets colors, white for walls, black for path
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
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
	for (int y = 0; y < this->mazeGrid.size(); y++)
	{
		for (int x = 0; x < this->mazeGrid[0].size(); x++)
		{
			c.push_back(image[y][x][0]);
			c.push_back(image[y][x][1]);
			c.push_back(image[y][x][2]);
		}
	}
	
#ifdef DEBUG
	if (!stbi_write_png("MazePNG/MINOTAURPATH.png", this->mazeGrid[0].size(), this->mazeGrid.size(), 3, c.data(), 3 * (this->mazeGrid.size())))
	{
		std::cout << "ERROR MINOTAURPATH NOT WRITTEN" << std::endl;
	}
	else
	{
		std::cout << "MINOTAURPATH WRITTEN" << std::endl;
	}
#endif
}
