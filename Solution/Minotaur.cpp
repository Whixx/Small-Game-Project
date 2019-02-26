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

void Minotaur::Update()
{
	// Move along path

		// If a path is not available

			// Choose a location in the player-area at random

			// Generate path between current location and goal location

			// Play growl sound
		

		// Calculate move direction

		// Move in specified direction until pos is at next tile

		// Play step sound
}
