#ifndef MINOTAUR_H
#define MINOTAUR_H

#include "Transform.h"
#include "Model.h"
#include "Sound.h"
#include "Maze.h"
#include <vector>

class Minotaur
{
public:
	Minotaur(irrklang::ISoundEngine * engine, std::vector<std::vector<int>> mazeGrid, Maze* maze);
	virtual ~Minotaur();
	Transform GetTransform();
	void increaseAgressionLevel();
	void reactToSound(glm::vec3 soundPos);
	void PlayScreamSound();

	void Initialize();
	void Update(double dt, glm::vec3 playerPos);
	void DrawMeshes(Shader* shader);
	void Draw(Shader* shader);
	glm::vec2 ClampToEdges(glm::vec2 mazeCoords);	// takes in maze coords and spits out mazecoords
	const SkeletonBuffer& GetSkeletonBuffer();
	const glm::mat4& GetBoneTransform(unsigned int index);

	void ResetMinotaur(std::vector<std::vector<int>> newMazeGrid, Maze* newMaze);

	// TEST
	void setupColorData();
	void drawPath();

private:
	AnimatedModel model;
	Sound stepSound;
	Sound growlSound;
	Sound screamSound;
	Transform transform;
	float movementSpeed;
	int alerted = 0;
	glm::vec3 lastSoundHeardPos;
	int searchArea;
	glm::vec2 destination;
	Maze* maze;
	std::vector<glm::vec2> generatedPath;
	std::vector<std::vector<int>> mazeGrid;
	int interval;
	float timePassed;
	int agression;

	struct distance {
		int y;
		int x;
	};

	void GeneratePath(int startY, int startX, int destinationY, int destinationX);
	void Move();
	glm::vec2 toNearbyFloor(glm::vec2 mazePos);

	// TEST
	// color data for image
	std::vector<std::vector<std::vector<unsigned char>>> image;
};

#endif