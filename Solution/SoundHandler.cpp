#include "SoundHandler.h"

SoundHandler::SoundHandler(const char* filePath, bool loopSound, bool startPaused, irrklang::ISoundEngine* engine)
{
	this->engine = engine;

	this->source = this->engine->addSoundSourceFromFile(filePath);
	this->filePath = filePath;
	this->loopSound = loopSound;
	this->startPaused = startPaused;

}

//SoundHandler::SoundHandler()
//{
//}

SoundHandler::~SoundHandler()
{
}

void SoundHandler::SetPosition(irrklang::vec3df position)
{
	if (this->sound)
	{
		this->position = position;
		this->sound->setPosition(position);
	}
}

void SoundHandler::Play()
{
	// if statement makes sure only one instance of the sound is played, this might have to change for certain sounds in the future?
	if (!this->engine->isCurrentlyPlaying(this->source))
	{
		this->sound = this->engine->play3D(this->filePath, this->position, this->loopSound, this->startPaused, true, irrklang::ESM_AUTO_DETECT, true);
	}
}

void SoundHandler::Stop()
{
	this->engine->stopAllSoundsOfSoundSource(source);
}

void SoundHandler::SetLooped(bool value)
{
	this->loopSound = value;
}

void SoundHandler::SetStartPaused(bool value)
{
	this->startPaused = value;
}
