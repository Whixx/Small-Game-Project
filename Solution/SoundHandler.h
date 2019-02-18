#ifndef SOUNDHANDLER_H
#define SOUNDHANDLER_H
#include "SoundEngine.h"

class SoundHandler
{
public:
	SoundHandler(const char* filePath, bool loopSound, bool startPaused, irrklang::ISoundEngine* engine);
	~SoundHandler();

	void SetPosition(glm::vec3 position);
	void Play();
	void Stop();
	void SetLooped(bool value);
	void SetStartPaused(bool value);
	void SetMinDistance(float value);
	void SetVolume(float value);

private:
	irrklang::ISoundEngine* engine;
	irrklang::ISoundEffectControl* fx;
	const char* filePath;
	irrklang::vec3df position;
	bool loopSound;
	bool startPaused;

	irrklang::ISound* sound;
	irrklang::ISoundSource* source;

};

#endif // !SOUNDHANDLER_H
