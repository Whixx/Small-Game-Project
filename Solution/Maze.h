#ifndef MAZE_H
#define MAZE_H

#include "glm/glm.hpp"
#include <string>
#include "stb_image.h"
#include <iostream>
#include <glew\glew.h>
#include <vector>
#include "Transform.h"
#include "Texture.h"
#include "Shader.h"

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

	// Draw to transform feedback buffer
	void DrawMazeToBuffer();

	// Draw from transform feedback buffer
	void DrawMaze();

	void BindWallMaterial(Shader* shader);
	void BindFloorMaterial(Shader* shader);

	void LoadMaze(const std::string& fileName);

	bool IsWallAtWorld(float x, float y);

	void BindTexture(unsigned int textureUnit);
private:

	unsigned char* imageData;
	string path;
	int width;
	int height;
	int numComponents;
	GLuint mazeTexture;
	glm::vec2 drawOrder[(1 + 2 * DRAWDISTANCE)*(1 + 2 * DRAWDISTANCE)];

	Transform transform;

	std::vector<Texture*> wallTextures;
	std::vector<Texture*> floorTextures;

	GLuint mazeTbo;
	GLuint mazeVbo;
	GLuint mazeVao;

	const int scaleXZ = 2;
	const int scaleY  = 2;

	// Private functions
	void LoadTextures();

	void InitiateMazeBuffers();

	// When generating the maze outwards from the player
	void GenerateDrawOrder();
	glm::vec3 readPixel(unsigned int x, unsigned int y);
};
#endif