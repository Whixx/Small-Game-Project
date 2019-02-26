#ifndef MINOTAUR_H
#define MINOTAUR_H

#include "Transform.h"
#include "Model.h"
#include "SoundHandler.h"

class Minotaur
{
public:
	Minotaur(Transform transform, irrklang::ISoundEngine* engine);
	virtual ~Minotaur();

private:
	Model model;
	SoundHandler stepSound;
	SoundHandler growlSound;
	Transform transform;
	float movementSpeed;

};

#endif