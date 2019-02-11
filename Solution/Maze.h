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
public:
	Maze(string bitmapPath);
	~Maze();

	// Not tested or used
	Maze(const Maze &other);
	Maze& operator=(const Maze& other);


	bool LoadBMP(const std::string& fileName);
	bool IsWallAtWorld(float x, float y);

	void Bind(unsigned int textureUnit);

private:
	unsigned char* imageData;
	string path;
	int width;
	int height;
	int numComponents;
	GLuint texture;

	glm::vec3 readPixel(unsigned int x, unsigned int y);
};
#endif

