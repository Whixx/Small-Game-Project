#ifndef MAZEGENERATEPNG_H
#define MAZEGENERATEPNG_H

#include <iostream>
#include <vector>
#include <random>
#include <glm/vec2.hpp>


class MazeGeneratePNG
{
public:
	MazeGeneratePNG(int height, int width);
	~MazeGeneratePNG();

	int path = 0;
	int wall = 1;
	void SetCell(int y, int x, int value); // can be used to set exit points on the maze (or other stuff)
	int GetCell(int y, int x);
	void Generate(void);
	void SetupColorData();
	void SetupColorDataForColor();
	void DrawPNG();
	std::vector<std::vector<int>> GetGrid();


	//// pathfinding
	//int solution = 2;
	//void GeneratePath(int startY, int startX, int destinationY, int destinationX);
	//void SetupPathData();
	//void DrawRedPath();

private:
	std::random_device randomDevice;
	int vertical = 0;
	int horizontal = 1;
	int north = 0;
	int south = 1;
	int west = 2;
	int east = 3;
	std::vector<std::vector<int>> grid;
	int width;
	int height;

	// for kruskal
	struct Element
	{
		int y;
		int x;
		int orientation;
	};
	std::vector<std::vector<int>> sets;
	void Replace(int setToReplace, int sampleSet);

	// color data for image
	std::vector<std::vector<std::vector<unsigned char>>> image;

	//// pathfinding
	//// for dijkstras alg
	//struct distance {
	//	int y;
	//	int x;
	//};
	//std::vector<std::vector<int>> redPath;

	//vector of coordinates for the minotaur to use
	std::vector<glm::vec2> generatedPath;
};

#endif /* MAZEGENERATEPNG_H */

