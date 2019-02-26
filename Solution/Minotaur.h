#ifndef MINOTAUR_H
#define MINOTAUR_H

#include "Transform.h"
#include "Model.h"
#include "SoundHandler.h"
#include <vector>

class Minotaur
{
public:
	Minotaur(Transform transform, irrklang::ISoundEngine* engine);
	virtual ~Minotaur();

	void Update(glm::vec3 playerPos);

private:
	Model model;
	SoundHandler stepSound;
	SoundHandler growlSound;
	Transform transform;
	float movementSpeed;
	int searchArea = 8;
	glm::vec2 destination;
	std::vector<glm::vec2> path;

	void move();
};

#endif