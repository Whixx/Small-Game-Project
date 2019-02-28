#ifndef MINOTAUR_H
#define MINOTAUR_H

#include "Transform.h"
#include "Model.h"
#include "SoundHandler.h"
#include "Maze.h"
#include <vector>

class Minotaur
{
public:
	Minotaur(irrklang::ISoundEngine * engine, std::vector<std::vector<int>> mazeGrid, Maze* maze);
	virtual ~Minotaur();
	Transform GetTransform();

	void Update(glm::vec3 playerPos);
	void Draw(Shader* shader);

private:
	Model model;
	SoundHandler stepSound;
	SoundHandler growlSound;
	Transform transform;
	float movementSpeed;
	int searchArea = 8;
	glm::vec2 destination;
	Maze* maze;
	std::vector<glm::vec2> generatedPath;
	std::vector<std::vector<int>> mazeGrid;

	struct distance {
		int y;
		int x;
	};

	void GeneratePath(int startY, int startX, int destinationY, int destinationX);
	void Move();
	void spawnOnFloor();
};

#endif