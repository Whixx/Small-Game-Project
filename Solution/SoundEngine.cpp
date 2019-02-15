#include "SoundEngine.h"

SoundEngine::SoundEngine()
{
	this->engine = irrklang::createIrrKlangDevice();

	if (!this->engine)
	{
		std::cout << "Sound engine ERROR" << std::endl;
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

//void SoundEngine::SetListenerPosition(irrklang::vec3df position, irrklang::vec3df viewDirection)
//{
//	this->engine->setListenerPosition(position, viewDirection, this->velPerSecond, this->upVector);
//}

void SoundEngine::Update(irrklang::vec3df position, irrklang::vec3df viewDirection)
{
	this->engine->setListenerPosition(position, viewDirection, this->velPerSecond, this->upVector);
}

irrklang::ISoundEngine* SoundEngine::GetEngine()
{
	return this->engine;
}
