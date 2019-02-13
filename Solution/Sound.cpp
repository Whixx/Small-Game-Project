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

	// set up fx pointer
	this->fx = 0;

	// initiate sounds

	// player sounds
	this->playerFootStep = this->engine->play3D("Sounds/playerfootstep.wav", irrklang::vec3df(0, -1, 0), false, true, true, irrklang::ESM_AUTO_DETECT, true);
	this->playerStepTime = 0;

	// torch sound, this will always be on?
	this->torch = this->engine->play3D("Sounds/torch.wav", irrklang::vec3df(2, 0, -2), true, false, true, irrklang::ESM_AUTO_DETECT, true);
	//this->PlayTorchSound();

	// minotaur sounds
	this->minotaurFootStep = this->engine->play3D("Sounds/minotaurstep.ogg", irrklang::vec3df(0, 0, 0), true, false, true, irrklang::ESM_AUTO_DETECT, true);
	this->minotaurGrowl = this->engine->play3D("Sounds/minotaurgrowl.wav", irrklang::vec3df(0, 0, 0), false, true, true, irrklang::ESM_AUTO_DETECT, true);

	// coin sound
	//this->coin = this->engine->



}

Sound::~Sound()
{
}

void Sound::SetPlayerPosition(irrklang::vec3df position, irrklang::vec3df direction)
{
	this->engine->setListenerPosition(position, direction);
	this->SetPlayerFootStepPosition(position);
	this->SetTorchPosition(position);
	this->SetMinotaurFootPosition(position);
}

void Sound::SetMusicPosition(irrklang::vec3df position)
{
	this->music->setPosition(position);
}

void Sound::SetPlayerFootStepPosition(irrklang::vec3df position)
{
	this->playerFootStep->setPosition(position);
}

void Sound::PlayPlayerFootStep(float time)
{
	// apply effects if needed
	//this->fx = playerFootStep->getSoundEffectControl();
	//fx->enableWavesReverbSoundEffect();

	this->playerStepTime += time;
	if (playerStepTime > 0.5)
	{
		this->playerFootStep = this->engine->play3D("Sounds/playerfootstep.wav", irrklang::vec3df(0, 0, 0), false, false, true, irrklang::ESM_AUTO_DETECT, true);
		playerStepTime = 0.0;
	}
}

void Sound::SetTorchPosition(irrklang::vec3df position)
{
	this->torch->setPosition(position);
}

void Sound::PlayTorchSound()
{
	//this->torch = this->engine->play3D("Sounds/torch.wav", irrklang::vec3df(0, 0, 0), true, false, true, irrklang::ESM_AUTO_DETECT, true);
}

void Sound::SetMinotaurFootPosition(irrklang::vec3df position)
{
	newPosition.Y = 0.0;
	newPosition.X = sinf(counter * 0.005f * 3.15) * 5.0f;
	newPosition.Z = cosf(counter * 0.005f * 3.15) * 5.0f;
	counter++;

	this->minotaurFootStep->setPosition(newPosition);
}
