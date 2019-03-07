#ifndef SOUNDHANDLER_H
#define SOUNDHANDLER_H
#include "SoundEngine.h"

class SoundHandler
{
public:
	SoundHandler(const char* filePath, bool loopSound, irrklang::ISoundEngine* engine);
	SoundHandler();
	~SoundHandler();

	void SetPosition(glm::vec3 position);
	void Play();
	void Stop();
	void SetLooped(bool value);				// sets the sound to either play once per call (false), or loop continuously until explicitly stopped (true)
	void SetMinDistance(float value);		// sets the distance at which the sound gets played at maximum volume. Default is 1, increase for "loud" sounds, decrease for "quiet" sounds.
	void SetVolume(float value);			// sets the volume of the sound, between 0 (mute) and 1 (full volume)

private:
	irrklang::ISoundEngine* engine;
	const char* filePath;
	irrklang::vec3df position;
	bool loopSound;

	irrklang::ISound* sound;
	irrklang::ISoundSource* source;

};

#endif // !SOUNDHANDLER_H
