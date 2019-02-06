#ifndef MAZEGENERATEPNG_H
#define MAZEGENERATEPNG_H

#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <stdexcept>
#include "stb_image_write.h"

class MazeGeneratePNG
{
public:
	MazeGeneratePNG(int height, int width);

	int wall = 0;
	int path = 1;
	void set_cell(int y, int x, int value); // can be used to set exit points on the maze (or other stuff)
	int get_cell(int y, int x);
	void generate(void);
	void setupColorData();
	void draw_png();

private:
	std::random_device random_device;
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
	struct element {
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

