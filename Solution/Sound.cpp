#include "Sound.h"

Sound::Sound()
{
	this->engine = irrklang::createIrrKlangDevice();
	if (!engine)
	{
		std::cout << "some shet went wrong" << std::endl;
	}
	else
	{
		std::cout << "soundengine created fine" << std::endl;
	}

	// initiate sounds
	//this->music = this->engine->play3D("Sounds/bell.wav", irrklang::vec3df(0, 0, 0), false, false, true, irrklang::ESM_AUTO_DETECT, true);
	//this->music->setMinDistance(1.0f);
	this->playerFootStep = this->engine->play3D("Sounds/playerfootstep.wav", irrklang::vec3df(0, 0, 0), false, true, true, irrklang::ESM_AUTO_DETECT, true);
	//this->playerFootStep->setPosition(irrklang::vec3df(10.0, -0.0, 0.0));
	this->playerStepTime = 0;
}

Sound::~Sound()
{
}

void Sound::SetPlayerPosition(irrklang::vec3df position, irrklang::vec3df direction)
{
	this->engine->setListenerPosition(position, direction);
}

void Sound::SetMusicPosition(irrklang::vec3df position)
{
	this->music->setPosition(position);
}

void Sound::SetPlayerFootStepPosition(irrklang::vec3df position)
{
	this->playerFootStep->setPosition(position);
}

void Sound::PlayPlayerFootStepSound(float time)
{
	this->playerStepTime += time;
	if (playerStepTime > 0.5)
	{
		this->playerFootStep = this->engine->play3D("Sounds/playerfootstep.wav", irrklang::vec3df(0, 0, 0), false, false, true, irrklang::ESM_AUTO_DETECT, true);
		//this->engine->play3D("Sounds/playerfootstep.wav", irrklang::vec3df(10, 0, 0), false, true, true, irrklang::ESM_AUTO_DETECT, true);
		playerStepTime = 0.0;
	}
}
