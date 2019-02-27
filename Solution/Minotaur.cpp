#include "Minotaur.h"

Minotaur::Minotaur(irrklang::ISoundEngine * engine)
	:model("Models/Deer/deer.obj"),
	stepSound("Sounds/minotaurstep.ogg", false, engine),
	growlSound("Sounds/minotaurgrowl.wav", false, engine)
{
	this->transform.GetScale() = glm::vec3(0.01f, 0.01f, 0.01f);
	this->transform.GetPos() = glm::vec3(0.0f, 2.0f, 0.0f);
	this->movementSpeed = 1 * this->transform.GetScale().y;
}

Minotaur::~Minotaur()
{
}

Transform Minotaur::GetTransform()
{
	return this->transform;
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
		
		this->path.push_back(glm::vec2(5, 5));
		this->path.push_back(glm::vec2(5, 4));
		this->path.push_back(glm::vec2(5, 3));
		this->path.push_back(glm::vec2(5, 2));
		this->path.push_back(glm::vec2(5, 1));
		this->path.push_back(glm::vec2(5, 0));
		this->path.push_back(glm::vec2(4, 0));
		this->path.push_back(glm::vec2(3, 0));
		this->path.push_back(glm::vec2(2, 0));
		this->path.push_back(glm::vec2(1, 0));
		
		// Update the destination
		this->destination = path.at(path.size() - 1);
		path.pop_back();

		// Play growl sound
		growlSound.Play();
	}
	
	// Set the destination to the next tile on the path
	if (this->destination.x == this->transform.GetPos().x && this->destination.y == this->transform.GetPos().z)
	{
		this->destination = path.at(path.size() - 1);
		path.pop_back();
	}
		
	// Move towards the current destination
	Move();
	// Play step sound
}

void Minotaur::Move()
{
	// Identify the current position direction
	glm::vec2 direction = glm::vec2(
		this->destination.x - this->transform.GetPos().x,
		this->destination.y - this->transform.GetPos().z);

	// If we are not walking past the destination
	if (glm::length(direction) > glm::length(this->movementSpeed*glm::normalize(direction)))
	{
		this->transform.GetPos().x += this->movementSpeed*glm::normalize(direction).x;
		this->transform.GetPos().z += this->movementSpeed*glm::normalize(direction).y;
	}
	else	// Else we are about to walk past the destination, which means that we have arrived
	{
		this->transform.GetPos().x = this->destination.x;
		this->transform.GetPos().z = this->destination.y;
	}
}

void Minotaur::Draw(Shader * shader)
{
	this->model.Draw(shader);
}
