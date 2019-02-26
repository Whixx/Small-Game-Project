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

void Minotaur::Update(glm::vec3 playerPos)
{
	// If a path is not available
	if (path.empty())
	{
		// Choose a location in the player-area at random
		glm::vec2 randomPos =
			glm::vec2(	playerPos.x + rand() % (this->searchArea * 2) - this->searchArea,
						playerPos.y + rand() % (this->searchArea * 2) - this->searchArea);
		// Generate path between current location and goal location
		
			// this->path = GeneratePath(glm::vec2(this->transform.GetPos()), randomPos);
		
		// Play growl sound
		growlSound.Play();
	}
	
	// Set the destination to the next tile on the path
	if (this->destination == glm::vec2(this->transform.GetPos()))
	{
		this->destination = path[path.size()];
		path.pop_back();
	}
		
	// Move towards the current destination
	this->move();

	// Play step sound
}

void Minotaur::move()
{
	// Identify the current position direction
	glm::vec2* currentPos = &glm::vec2(this->transform.GetPos());
	glm::vec2 direction = this->destination - *currentPos;

	// If we are not walking past the destination
	if (glm::length(direction) > glm::length(this->movementSpeed*glm::normalize(direction)))
		*currentPos += this->movementSpeed*glm::normalize(direction);
	else	// Else we are about to walk past the destination, which means that we have arrived
		*currentPos = this->destination;
}
