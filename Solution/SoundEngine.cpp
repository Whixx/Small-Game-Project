#include "SoundEngine.h"

SoundEngine::SoundEngine()
{
	this->engine = irrklang::createIrrKlangDevice();

	if (!this->engine)
	{
		std::cout << "Sound engine ERROR" << std::endl;
		exit(0); // TODO: dont quit, fix sounds
	}
	else
	{
		std::cout << "Sound engine created" << std::endl;
	}

	this->velPerSecond = irrklang::vec3df(0.0, 0.0, 0.0);
	this->upVector = irrklang::vec3df(0.0, 1.0, 0.0);
}

SoundEngine::~SoundEngine()
{
}

void SoundEngine::Update(glm::vec3 position, glm::vec3 viewDirection)
{
	this->engine->setListenerPosition(
		irrklang::vec3df(position.x, position.y, -position.z),
		irrklang::vec3df(viewDirection.x, viewDirection.y, -viewDirection.z),
		this->velPerSecond,
		this->upVector);
}

irrklang::ISoundEngine* SoundEngine::GetEngine()
{
	return this->engine;
}
