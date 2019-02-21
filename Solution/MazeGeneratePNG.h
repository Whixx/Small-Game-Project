#ifndef MAZEGENERATEPNG_H
#define MAZEGENERATEPNG_H

#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <stdexcept>


class MazeGeneratePNG
{
public:
	MazeGeneratePNG(int height, int width);
	~MazeGeneratePNG();

	int wall = 1;
	int path = 0;
	void Set_cell(int y, int x, int value); // can be used to set exit points on the maze (or other stuff)
	int Get_cell(int y, int x);
	void Generate(void);
	void SetupColorData();
	void SetupColorDataForColor();
	void Draw_png();

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
	void replace(int set_to_replace, int sample_set);

	// color data for image
	std::vector<std::vector<std::vector<unsigned char>>> image;
};

#endif /* MAZEGENERATEPNG_H */

