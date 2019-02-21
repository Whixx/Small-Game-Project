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
	Transform *GetTransform();

	void InitiateBuffers();

	// Draw to transform feedback buffer
	void DrawWallsToBuffer();
	void DrawFloorToBuffer();

	// Draw from transform feedback buffer
	void DrawWalls();
	void DrawFloor();

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

	Transform transform;

	GLuint wallTbo;
	GLuint wallVbo;
	GLuint wallVao;

	GLuint floorTbo;
	GLuint floorVbo;
	GLuint floorVao;

	const int scaleXZ = 1;
	const int scaleY = 1;
	void initiateWallBuffers();
	void initiateFloorBuffers();
	glm::vec3 readPixel(unsigned int x, unsigned int y);
};
#endif