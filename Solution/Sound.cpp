#include "Sound.h"

Sound::Sound(const char* filePath, bool loopSound, irrklang::ISoundEngine* engine)
{
	this->engine = engine;
	this->source = this->engine->addSoundSourceFromFile(filePath);
	this->filePath = filePath;
	this->loopSound = loopSound;
}

Sound::~Sound()
{
}

void Sound::SetPosition(glm::vec3 position)
{
	if (this->sound)
	{
		this->position.X = position.x;
		this->position.Y = position.y;
		this->position.Z = -position.z;
		this->sound->setPosition(this->position);
	}
}

void Sound::Play()
{
	// if statement makes sure only one instance of the sound is played, this might have to change for certain sounds in the future?
	if (!this->engine->isCurrentlyPlaying(this->source))
	{
		this->sound = this->engine->play3D(this->filePath, this->position, this->loopSound, false, true, irrklang::ESM_AUTO_DETECT, false);
	}
}

void Sound::Stop()
{
	this->engine->stopAllSoundsOfSoundSource(source);
}

void Sound::SetLooped(bool value)
{
	this->loopSound = value;
}

void Sound::SetMinDistance(float value)
{
	this->source->setDefaultMinDistance(value);
}

void Sound::SetVolume(float value)
{
	this->source->setDefaultVolume(value);
}
