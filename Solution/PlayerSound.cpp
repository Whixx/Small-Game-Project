#include "PlayerSound.h"

PlayerSound::PlayerSound()
{
	this->fx = 0;

	this->velPerSecond = irrklang::vec3df(0.0, 0.0, 0.0);
	this->upVector = irrklang::vec3df(0.0, 1.0, 0.0);
	this->torchPosition = irrklang::vec3df(0.0, 0.0, 0.0);

	this->footStepSource = this->GetEngine()->addSoundSourceFromFile("Sounds/playerfootstep.ogg");
	this->footStepSource->setDefaultVolume(0.5);	// this may have to change when better sound is used

	this->torchSource = this->GetEngine()->addSoundSourceFromFile("Sounds/torch.wav");
	// starts playing the torch sound immediately, this will have to be called from player/torch class when needed later
	this->PlayTorchSound();

	this->coinSource = this->GetEngine()->addSoundSourceFromFile("Sounds/coin.wav");
}

PlayerSound::~PlayerSound()
{
}

void PlayerSound::SetPosition(irrklang::vec3df position)
{
	this->playerPosition = position;
}

void PlayerSound::SetDirection(irrklang::vec3df direction)
{
	this->playerDirection = direction;
}

void PlayerSound::SetFootStepPosition()
{
	if (this->footStep)
	{
		this->footStep->setPosition(this->playerPosition);
	}
}

void PlayerSound::PlayFootStep()
{
	if (!this->GetEngine()->isCurrentlyPlaying(this->footStepSource))
	{
		this->footStep = this->GetEngine()->play3D("Sounds/playerfootstep.ogg", this->playerPosition, false, false, true, irrklang::ESM_AUTO_DETECT, true);

		// apply effects here if needed, example below
		//this->fx = this->footStep->getSoundEffectControl();
		//fx->enableWavesReverbSoundEffect();
	}
}

void PlayerSound::SetTorchPosition(irrklang::vec3df position)
{
	if (this->torch)
	{
		this->torchPosition = position;
		this->torch->setPosition(position);
	}
}

void PlayerSound::PlayTorchSound()
{
	if (!this->GetEngine()->isCurrentlyPlaying(this->torchSource))
	{
		// uses playerposition atm, there is a torchPosition/setTorchPosition variable for more dynamic positioning of torch sound
		this->torch = this->GetEngine()->play3D("Sounds/torch.wav", this->playerPosition, true, false, true, irrklang::ESM_AUTO_DETECT, true);
	}
}

void PlayerSound::PlayCoinSound(irrklang::vec3df position)
{
	if (!this->GetEngine()->isCurrentlyPlaying(this->coinSource))
	{
		this->coin = this->GetEngine()->play3D("Sounds/coin.wav", position, false, false, true, irrklang::ESM_AUTO_DETECT, true);
	}
}

void PlayerSound::Update(irrklang::vec3df position, irrklang::vec3df viewDirection)
{
	this->SetPosition(position);
	this->SetDirection(viewDirection);
	this->GetEngine()->setListenerPosition(this->playerPosition, this->playerDirection, this->velPerSecond, this->upVector);

	this->SetFootStepPosition();
	this->SetTorchPosition(position);
}

void PlayerSound::StopAllSounds()
{
	this->GetEngine()->stopAllSoundsOfSoundSource(footStepSource);
	this->GetEngine()->stopAllSoundsOfSoundSource(torchSource);
	this->GetEngine()->stopAllSoundsOfSoundSource(coinSource);
}
