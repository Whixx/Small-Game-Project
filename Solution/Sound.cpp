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

	// set up fx pointer
	this->fx = 0;

	this->velPerSecond = irrklang::vec3df(0.0, 0.0, 0.0);
	this->upVector = irrklang::vec3df(0.0, 1.0, 0.0);

	// initiate sounds

	// player sounds
	this->playerFootStepSource = this->engine->addSoundSourceFromFile("Sounds/playerfootstep.ogg");

	// torch sound, this will always be on?
	this->PlayTorchSound();

	// minotaur sounds
	this->minotaurFootStepSource = this->engine->addSoundSourceFromFile("Sounds/minotaurstep.ogg");
	PlayMinotaurFootStep();

	this->minotaurGrowlSource = this->engine->addSoundSourceFromFile("Sounds/minotaurgrowl.wav");


	// coin sound
	this->coinSource = this->engine->addSoundSourceFromFile("Sounds/coin.wav");

}

Sound::~Sound()
{
}

void Sound::SetPlayerPosition(irrklang::vec3df position, irrklang::vec3df direction)
{
	this->engine->setListenerPosition(position, direction, this->velPerSecond, this->upVector);
}

void Sound::SetMusicPosition(irrklang::vec3df position)
{
	this->music->setPosition(position);
}

void Sound::SetPlayerFootStepPosition(irrklang::vec3df position)
{
	if (this->playerFootStep)
	{
		this->playerFootStep->setPosition(position);
	}
}

void Sound::PlayPlayerFootStep()
{
	if (!this->engine->isCurrentlyPlaying(this->playerFootStepSource))
	{
		this->playerFootStep = this->engine->play3D("Sounds/playerfootstep.ogg", this->playerPosition, false, false, true, irrklang::ESM_AUTO_DETECT, true);

		// apply effects here if needed, example below
		//this->fx = playerFootStep->getSoundEffectControl();
		//fx->enableWavesReverbSoundEffect();

	}
}

void Sound::SetTorchPosition(irrklang::vec3df position)
{
	this->torch->setPosition(position);
}

void Sound::PlayTorchSound()
{
	this->torch = this->engine->play3D("Sounds/torch.wav", this->playerPosition , true, false, true, irrklang::ESM_AUTO_DETECT, true);

	//apply effects here if needed
}

void Sound::SetMinotaurFootPosition(irrklang::vec3df position)
{
	// temporär minotaurfotsteg som går runt i cirkel
	newPosition.Y = 0.0;
	newPosition.X = sinf(counter * 0.005f * 3.15) * 5.0f;
	newPosition.Z = cosf(counter * 0.005f * 3.15) * 5.0f;
	counter++;

	this->minotaurFootStep->setPosition(newPosition);
}

void Sound::PlayMinotaurFootStep()
{
	if (!this->engine->isCurrentlyPlaying(this->playerFootStepSource))
	{
		this->minotaurFootStep = this->engine->play3D("Sounds/minotaurstep.ogg", this->minotaurPosition, true, false, true, irrklang::ESM_AUTO_DETECT, true);

		// apply effects here if needed, example below
	}
}

void Sound::StopMinotaurFootstep()
{
	this->minotaurFootStep->stop();
}

void Sound::SetMinotaurGrowlPosition(irrklang::vec3df position)
{
	if (minotaurGrowl)
	{
		this->minotaurGrowl->setPosition(position);
	}
}

void Sound::PlayMinotaurGrowl()
{
	if (!this->engine->isCurrentlyPlaying(this->minotaurGrowlSource))
	{
		this->minotaurGrowl = this->engine->play3D("Sounds/minotaurgrowl.wav", this->minotaurPosition, false, false, true, irrklang::ESM_AUTO_DETECT, true);

		// apply effects here if needed
	}
}

void Sound::PlayCoinSound(irrklang::vec3df position)
{
	if (!this->engine->isCurrentlyPlaying(this->coinSource))
	{
		this->coin = this->engine->play3D("Sounds/coin.wav", position, false, false, true, irrklang::ESM_AUTO_DETECT, true);
	}
}

void Sound::Update(irrklang::vec3df position, irrklang::vec3df viewDirection)
{
	this->SetPlayerPosition(position, viewDirection);

	// set position of footsteps and torch 
	this->SetPlayerFootStepPosition(position);
	this->SetTorchPosition(position);
	this->SetMinotaurFootPosition(position);
	this->SetMinotaurGrowlPosition(position);
}

void Sound::UpdatePlayer(irrklang::vec3df position, irrklang::vec3df viewDirection)
{
	this->playerPosition = position;
	this->SetPlayerPosition(position, viewDirection);
}

void Sound::UpdateMinotaur(irrklang::vec3df position)
{
	this->minotaurPosition = position;
}

void Sound::StopAllSounds()
{
	this->engine->stopAllSounds();
}
