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

const unsigned int DRAWDISTANCE = 6;

class Maze
{
public:
	Maze();
	~Maze();

	int GetMazeHeight();
	int GetMazeWidth();
	Transform *GetTransform();
	glm::vec2* GetDrawOrder();
	unsigned int GetTileCount();

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
	glm::vec2 drawOrder[(1 + 2 * DRAWDISTANCE)*(1 + 2 * DRAWDISTANCE)];

	Transform transform;

	GLuint wallTbo;
	GLuint wallVbo;
	GLuint wallVao;

	GLuint floorTbo;
	GLuint floorVbo;
	GLuint floorVao;

	const int scaleXZ = 1;
	const int scaleY  = 1;

	// Private functions
	void initiateWallBuffers();
	void initiateFloorBuffers();
	// When generating the maze outwards from the player
	void GenerateDrawOrder();
	glm::vec3 readPixel(unsigned int x, unsigned int y);
};
#endif