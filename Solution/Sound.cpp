#include "Sound.h"

Sound::Sound()
{
	this->engine = irrklang::createIrrKlangDevice();
	if (!engine)
	{
		std::cout << "sound engine error" << std::endl;
	}
	else
	{
		std::cout << "sound engine created" << std::endl;
	}
}

Sound::~Sound()
{
}

void Sound::Update(irrklang::vec3df position, irrklang::vec3df viewDirection)
{
}

void Sound::Update(irrklang::vec3df position)
{
}


irrklang::ISoundEngine * Sound::GetEngine()
{
	return this->engine;
}
