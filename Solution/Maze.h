#ifndef MAZE_H
#define MAZE_H

#include "glm/glm.hpp"
#include <string>
#include "stb_image.h"
#include <iostream>
#include <glew\glew.h>
#include <vector>
#include "Transform.h"

using namespace std;

class Maze
{
public:
	Maze();
	~Maze();

	// Not tested or used
	//Maze(const Maze &other);
	//Maze& operator=(const Maze& other);

	int GetMazeHeight();
	int GetMazeWidth();
	Transform GetTransform();

	void InitiateBuffers();
	void DrawToBuffer();
	void DrawMaze(bool texCoords);
	void LoadMaze(const std::string& fileName);

	bool IsWallAtWorld(float x, float y);

	void BindTexture(unsigned int textureUnit);

private:
	unsigned char* imageData;
	string path;
	int width;
	int height;
	int numComponents;
	GLuint texture;

	GLuint tbo;
	GLuint vbo;
	GLuint vao;

	Transform transform;

	glm::vec3 readPixel(unsigned int x, unsigned int y);
};
#endif