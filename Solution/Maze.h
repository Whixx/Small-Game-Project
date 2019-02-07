#ifndef MAZE_H
#define MAZE_H

#include "glm/glm.hpp"
#include <string>
#include "stb_image.h"
#include <iostream>
#include <glew\glew.h>
#include <vector>

using namespace std;

class Maze
{

private:
	unsigned char* imageData;
	string path;
	int width;
	int height;
	int numComponents;
	GLuint m_texture;

	glm::vec3 readPixel(unsigned int x, unsigned int y);

public:
	Maze(string bitmapPath);
	~Maze();

	// Not tested or used
	Maze(const Maze &other);
	Maze& operator=(const Maze& other);


	bool loadBMP(const std::string& fileName);
	bool IsWallAtWorld(float x, float y);

	void bind(unsigned int textureUnit);
};
#endif

