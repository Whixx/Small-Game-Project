#include "Minotaur.h"

Minotaur::Minotaur(Transform transform, irrklang::ISoundEngine * engine)
	:model("Models/Deer/deer.obj"),
	stepSound("Sounds/minotaurstep.ogg", false, engine),
	growlSound("Sounds/minotaurgrowl.wav", false, engine)
{
	this->transform = transform;
	this->movementSpeed = 1 * this->transform.GetScale().y;
}

Minotaur::~Minotaur()
{
}
