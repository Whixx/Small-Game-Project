#ifndef MAZE_H
#define MAZE_H

#include <string>
#include "stb_image.h"
#include <iostream>
#include <glew\glew.h>

class Maze
{

public:
	Maze();
	~Maze();

	int GetHeight();
	int GetWidth();


	void initiateBuffers();
	void LoadMaze(const std::string& fileName);
	unsigned char readPixel(unsigned int x, unsigned int y);
	void DrawToBuffer();
	void DrawMaze();
	void BindTexture(unsigned int textureUnit);


private:
	unsigned char* imageData;
	int width;
	int height;
	int numComponents;
	GLuint m_texture;

	GLuint tbo;
	GLuint vbo;
	GLuint vao;

	GLuint nrOfPrimitives;
	
};
#endif

