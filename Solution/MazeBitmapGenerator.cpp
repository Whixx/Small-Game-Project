#include "MazeBitmapGenerator.h"

MazeBitmapGenerator::MazeBitmapGenerator(int imageSize, int numCells)
{
	this->imageSize = imageSize;
	this->numCells = numCells;
	this->cellSize = imageSize / numCells;
	this->g_Maze = new char[numCells * numCells];
	this->g_PtX = 0;
	this->g_PtY = 0;

	//this->rd();
	std::mt19937 randGen(this->rd());

	std::uniform_int_distribution<> dis(0, numCells - 1);
	std::uniform_int_distribution<> dis4(0, 3);

	std::uniform_int_distribution<> dis(0, numCells - 1);
	std::uniform_int_distribution<> dis4(0, 3);

	//                   0  1  2  3  4  5  6  7  8
	//                      U  R     D           L
	int Heading_X[9] = { 0, 0,+1, 0, 0, 0, 0, 0,-1 };
	int Heading_Y[9] = { 0,-1, 0, 0,+1, 0, 0, 0, 0 };
	int Mask[9] = {
								0,
								eDirection_Down | eDirection_Down << 4,
								eDirection_Left | eDirection_Left << 4,
								0,
								eDirection_Up | eDirection_Up << 4,
								0,
								0,
								0,
								eDirection_Right | eDirection_Right << 4
	};

}

MazeBitmapGenerator::~MazeBitmapGenerator()
{

}

int MazeBitmapGenerator::cellIndex()
{
	return this->g_PtX + this->numCells * this->g_PtY;
}

int MazeBitmapGenerator::randomInt()
{
	return static_cast<int>(dis(this->randGen));
}

int MazeBitmapGenerator::randomInt4()
{
	return static_cast<int>(dis4(this->randGen));
}

bool MazeBitmapGenerator::isDirValid(eDirection dir)
{
	int newX = g_PtX + Heading_X[dir];
	int newY = g_PtY + Heading_Y[dir];

	if (!dir || newX < 0 || newY < 0 || newX >= numCells || newY >= numCells)
	{
		return false;
	}

	return !g_Maze[newX + numCells * newY];
}

eDirection MazeBitmapGenerator::getDirection()
{
	eDirection dir = eDirection(1 << randomInt4());

	while (true)
	{
		for (int i = 0; i < 4; i++)
		{
			if (isDirValid(dir))
			{
				return eDirection(dir);
			}

			dir = eDirection(dir << 1);

			if (dir > eDirection_Left)
			{
				dir = eDirection_Up;
			}
		}

		dir = eDirection((g_Maze[cellIndex()] & 0xf0) >> 4);

		// nowhere to go
		if (!dir) 
		{
			return eDirection_Invalid;
		}

		g_PtX += Heading_X[dir];
		g_PtY += Heading_Y[dir];

		dir = eDirection(1 << randomInt4());
	}
}

void MazeBitmapGenerator::generateMaze()
{
	int cells = 0;

	for (eDirection dir = getDirection(); dir != eDirection_Invalid; dir = getDirection())
	{
		//if (++cells % 1000 == 0)
		//{
		//	std::cout << ".";
		//}

		g_Maze[cellIndex()] |= dir;
		g_PtX += Heading_X[dir];
		g_PtY += Heading_Y[dir];
		g_Maze[cellIndex()] = Mask[dir];
	}

	//std::cout << std::endl;
}
