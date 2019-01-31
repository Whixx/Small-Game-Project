#ifndef MAZEBITMAPGENERATOR_H
#define MAZEBITMAPGENERATOR_H
#include <random>

enum eDirection
{
	eDirection_Invalid = 0,
	eDirection_Up = 1,
	eDirection_Right = 2,
	eDirection_Down = 4,
	eDirection_Left = 8
};

class MazeBitmapGenerator
{
public:
	MazeBitmapGenerator(int imageSize, int numCells);
	~MazeBitmapGenerator();
	int cellIndex();
	int randomInt();
	int randomInt4();
	bool isDirValid(eDirection dir);
	eDirection getDirection();


private:
	int imageSize;
	int numCells;
	int cellSize;
	char* g_Maze;
	int g_PtX;
	int g_PtY;

	std::random_device rd;
	std::mt19937 randGen;
	std::uniform_int_distribution<> dis;
	std::uniform_int_distribution<> dis4;

	int Heading_X[9];
	int Heading_Y[9];
	int Mask[9];
};

#endif // !MAZEBITMAPGENERATOR_H