#include "MinotaurSound.h"

MinotaurSound::MinotaurSound()
{
	this->fx = 0;

	this->footStepSource = this->GetEngine()->addSoundSourceFromFile("Sounds/minotaurstep.ogg");
	this->growlSource = this->GetEngine()->addSoundSourceFromFile("Sounds/minotaurgrowl.wav");
}

MinotaurSound::~MinotaurSound()
{
}

void MinotaurSound::SetPosition(irrklang::vec3df position)
{
	this->position = position;
}

void MinotaurSound::PlayFootStep()
{
	if (!this->GetEngine()->isCurrentlyPlaying(this->footStepSource))
	{
		this->footStep = this->GetEngine()->play3D("Sounds/minotaurstep.ogg", this->position, true, false, true, irrklang::ESM_AUTO_DETECT, true);
		// this would set how far the sound is heard
		//this->footStep->setMinDistance()

		// apply effects here if needed
	}
}

void MinotaurSound::StopFootStep()
{
	this->footStep->stop();
}

void MinotaurSound::PlayGrowl()
{
	if (!this->GetEngine()->isCurrentlyPlaying(this->growlSource))
	{
		this->growl = this->GetEngine()->play3D("Sounds/minotaurgrowl.wav", this->position, false, false, true, irrklang::ESM_AUTO_DETECT, true);
	
		// apply effects here if needed
	}
}

void MinotaurSound::Update(irrklang::vec3df position)
{
	this->SetPosition(position);

	if(this->footStep)
	this->footStep->setPosition(position);

	if(this->growl)
	this->growl->setPosition(position);

	this->TestMinotaurPosition();
}

void MinotaurSound::StopAllSounds()
{
	this->GetEngine()->stopAllSoundsOfSoundSource(footStepSource);
	this->GetEngine()->stopAllSoundsOfSoundSource(growlSource);
}

void MinotaurSound::TestMinotaurPosition()
{
	// test simulation of minotaur that is walking around
	newPosition.Y = 0.0;
	newPosition.X = sinf(counter * 0.002f * 3.15) * 5.0f;
	newPosition.Z = cosf(counter * 0.002f * 3.15) * 5.0f;
	counter++;
	this->SetPosition(newPosition);
	if(this->footStep)
	this->footStep->setPosition(newPosition);
	if (this->growl)
	this->growl->setPosition(newPosition);
}
