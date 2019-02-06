#include "MazeGeneratePNG.h"

MazeGeneratePNG::MazeGeneratePNG(int height, int width)
{
	// height and width must be odd numbers for the maze to be "correct"
	if ((!(height % 2)) || (!(width % 2)))
	{
		throw std::invalid_argument("Height and width must be odd numbers!");
	}

	this->height = height;
	this->width = width;

	// create a grid with only walls
	grid.resize(height);
	for (int y = 0; y < height; y++)
	{
		grid[y].resize(width);
		for (int x = 0; x < width; x++)
		{
			grid[y][x] = wall;
		}
	}
}

void MazeGeneratePNG::set_cell(int y, int x, int value)
{
	if (((int)grid.size() <= y) || ((int)grid[y].size() <= x))
	{
		throw std::invalid_argument("Out of boundary!");
	}
	else if ((wall != value) && (path != value))
	{
		throw std::invalid_argument("The input can only contain 0 or 1.");
	}
	else
	{
		grid[y][x] = value;
	}
}

int MazeGeneratePNG::get_cell(int y, int x)
{
	return grid[y][x];
}

void MazeGeneratePNG::generate(void)
{
	std::mt19937 random_generator(random_device());

	// set up sets
	sets.resize(grid.size());
	for (int y = 0; y < (int)grid.size(); y++)
	{
		sets[y].resize(grid[y].size());
	}

	// save every y, x coordinate with a possible movement (vertical or horizontal) and fill the sets with different values
	std::vector<element> elements;
	int i = 1;
	for (int y = 1; y < ((int)sets.size() - 1); y += 2)
	{
		for (int x = 1; x < ((int)sets[0].size() - 1); x += 2)
		{
			if ((y + 2) < ((int)sets.size() - 1))
			{
				elements.push_back({ y, x, vertical });
			}
			if ((x + 2) < ((int)sets[0].size() - 1))
			{
				elements.push_back({ y, x, horizontal });
			}
			sets[y][x] = i;
			i++;
		}
	}

	// shuffle the elements vector
	std::shuffle(elements.begin(), elements.end(), random_generator);

	// do it until there are no elements left
	while (elements.size())
	{
		int y = elements[elements.size() - 1].y;
		int x = elements[elements.size() - 1].x;
		int orientation = elements[elements.size() - 1].orientation;
		elements.pop_back();

		if (horizontal == orientation)
		{
			// if the two sets are different, join them and carve a passage
			if (sets[y][x + 2] != sets[y][x])
			{
				replace(sets[y][x + 2], sets[y][x]);
				for (int j = 0; j < 3; j++)
				{
					grid[y][x + j] = path;
				}
			}
		}
		else if (vertical == orientation)
		{
			// if the two sets are different, join them and carve a passage
			if (sets[y + 2][x] != sets[y][x])
			{
				replace(sets[y + 2][x], sets[y][x]);
				for (int j = 0; j < 3; j++)
				{
					grid[y + j][x] = path;
				}
			}
		}
		else
		{
			// do nothing
		}
	}
}

void MazeGeneratePNG::replace(int set_to_replace, int sample_set)
{
	for (int search_y = 1; search_y < ((int)sets.size() - 1); search_y += 2)
	{
		for (int search_x = 1; search_x < ((int)sets[0].size() - 1); search_x += 2)
		{
			if (sets[search_y][search_x] == set_to_replace)
			{
				sets[search_y][search_x] = sample_set;
			}
		}
	}
}

void MazeGeneratePNG::draw_png()
{
	setupColorData();

	std::vector<unsigned char> a;
	for (int y = 0; y < this->height; y++)
	{
		for (int x = 0; x < this->width; x++)
		{
			a.push_back(image[y][x][0]);
			a.push_back(image[y][x][1]);
			a.push_back(image[y][x][2]);
		}
	}


	if (!stbi_write_png("Bitmap/maze.png", this->width, this->height, 3, a.data(), 3 * width))
	{
		std::cout << "THIS SHIT FAILED maze.png not written" << std::endl;
	}
	else
	{
		std::cout << "maze.png written to Bitmap-folder" << std::endl;
	}
}

void MazeGeneratePNG::setupColorData()
{
	// set up image vectors to hold color data
	image.resize(this->height);
	for (int y = 0; y < this->height; y++)
	{
		image[y].resize(this->width);
		for (int x = 0; x < this->width; x++)
		{
			image[y][x].resize(3);
		}
	}

	// sets colors, white for walls, black for path
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			for (int z = 0; z < 3; z++)
			{
				if (get_cell(y, x) == wall)
				{
					image[y][x][z] = 255;
				}
				else if (get_cell(y, x) == path)
				{
					image[y][x][z] = 0;
				}
			}
		}
	}
}
