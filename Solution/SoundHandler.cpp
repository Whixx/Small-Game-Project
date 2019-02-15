#include "SoundHandler.h"

SoundHandler::SoundHandler(const char* filePath, bool loopSound, bool startPaused, irrklang::ISoundEngine* engine)
{
	this->engine = engine;

	this->source = this->engine->addSoundSourceFromFile(filePath);
	this->filePath = filePath;
	this->loopSound = loopSound;
	this->startPaused = startPaused;


	//this->CreateSound();
}

SoundHandler::~SoundHandler()
{
}

void SoundHandler::SetPosition(irrklang::vec3df position)
{
	this->position = position;
}

void SoundHandler::Play()
{
	if (!this->engine->isCurrentlyPlaying(this->source))
	{
		this->sound = this->engine->play3D(this->filePath, this->position, this->loopSound, this->startPaused, true, irrklang::ESM_AUTO_DETECT, true);
	}
}

irrklang::ISoundEngine * SoundHandler::GetEngine()
{
	return this->engine;
}
