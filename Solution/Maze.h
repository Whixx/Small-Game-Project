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
#include "MaterialHandler.h"

using namespace std;

const unsigned int DRAWDISTANCE = 7;

class Maze
{
public:
	Maze();
	~Maze();

	int GetMazeHeight();
	int GetMazeWidth();
	glm::vec2 GetExitPos() const;
	glm::vec3 GetExitWorldPos() const;
	Transform *GetTransform();
	glm::vec2* GetDrawOrder();
	glm::vec3 TransformToMazeCoords(glm::vec3 pos);
	glm::vec3 TransformToWorldCoords(glm::vec3 pos);
	unsigned int GetTileCount();

	// Draw to transform feedback buffer
	void DrawMazeToBuffer();

	// Draw from transform feedback buffer
	void DrawMaze();
	void DrawShadows();

	void BindMaterial(Shader* shader);

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
	glm::vec3 exitWorldPos;

	Transform transform;

	Material* wall1Mat;
	Material* floor0Mat;

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
	glm::vec2 FindExit();
};
#endif