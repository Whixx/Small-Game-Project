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

#include "Exit.h"
#include "Model.h"
#include "Keystone.h"
#include "Sound.h"

#include <time.h>
using namespace std;

const unsigned int DRAWDISTANCE = 7;
const unsigned int NR_OF_START_KEYSTONES = 3;

enum Wall
{
	NO_WALL,
	WALL_VERTICAL,
	WALL_HORIZONTAL,
	WALL_CORNER_RIGHT_UP,
	WALL_CORNER_RIGHT_DOWN,
	WALL_CORNER_LEFT_UP,
	WALL_CORNER_LEFT_DOWN,
	WALL_END_UP,
	WALL_END_DOWN,
	WALL_END_RIGHT,
	WALL_END_LEFT,
	WALL_UP,
	WALL_DOWN,
	WALL_LEFT,
	WALL_RIGHT
};

class Maze
{
public:
	Maze(irrklang::ISoundEngine * engine);
	~Maze();

	int GetMazeHeight();
	int GetMazeWidth();

	Exit CreateExit();
	glm::vec3 GetExitWorldPos();
	Exit* GetExit();
	void SetExit(Exit exit);

	Transform *GetTransform();
	glm::vec2* GetDrawOrder();
	glm::vec3 TransformToMazeCoords(glm::vec3 pos);
	glm::vec3 TransformToWorldCoords(glm::vec3 pos);
	unsigned int GetTileCount();
	Transform * GetKeystoneTransform(unsigned int index);
	int GetNrOfKeystones();
	bool IsExitOpen();

	// Draw to transform feedback buffer
	void DrawMazeToBuffer();

	// Draw from transform feedback buffer
	void DrawMaze();
	void DrawMazeShadows();

	// Draw Keystones
	void DrawKeystone(unsigned int index, Shader * shader);

	bool ActivateKeystone(glm::vec3 playerPos, Sound * minotaurGrowlSound);
	void UpdateKeystones(float deltaTime);
	void ResetKeystones();

	void BindMaterial(Shader* shader);

	void LoadMaze(const std::string& fileName);
	
	bool IsWallAtWorld(float x, float y);

	Wall GetWallType(float x, float y);

	void BindTexture(unsigned int textureUnit);

	void SetExitScale();

private:

	unsigned char* imageData;
	string path;
	int width;
	int height;
	int numComponents;
	GLuint mazeTexture;
	glm::vec2 drawOrder[(1 + 2 * DRAWDISTANCE)*(1 + 2 * DRAWDISTANCE)];
	glm::vec3 exitWorldPos;
	glm::vec2 exitPos;
	bool isExitOpen;

	Exit exit;
	Model exitModelOpen;
	Model exitModelClosed;

	Keystone * keystones;
	int keystonesCapacity;
	int nrOfKeystones;
	Model keyStoneModel;
	Sound keystoneSound;
	glm::vec3 lastActivatedKeystone;

	Transform transform;

	Material* wall1Mat;
	Material* floor0Mat;

	GLuint mazeTbo;
	GLuint mazeVbo;
	GLuint mazeVao;

	const float scaleXZ = 2;
	const float scaleY  = 7;

	// Private functions
	void LoadTextures();

	void InitiateMazeBuffers();

	// When generating the maze outwards from the player
	void GenerateDrawOrder();

	// Returns the color of the pixel
	glm::vec3 readPixel(unsigned int x, unsigned int y);
	ExitPosDir FindExit();

	// Keystone functions
	KeystonePosDir CreateCubePosition();
	glm::vec3 FindNearbyFloor(glm::vec2 wallPos);
	void AddKeystone();
};
#endif