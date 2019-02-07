#ifndef MAZE_H
#define MAZE_H

#include <string>
#include "stb_image.h"
#include <iostream>
#include <glew\glew.h>

class Maze
{

private:
	unsigned char* imageData;
	int width;
	int height;
	int numComponents;
	GLuint m_texture;

public:
	Maze();
	~Maze();


	void LoadMaze(const std::string& fileName);
	unsigned char readPixel(unsigned int x, unsigned int y);
	void Draw(unsigned int width, unsigned int height);
	void BindTexture(unsigned int textureUnit);
};
#endif

