#ifndef SOUNDHANDLER_H
#define SOUNDHANDLER_H
#include "SoundEngine.h"

class SoundHandler
{
public:
	SoundHandler(const char* filePath, bool loopSound, bool startPaused, irrklang::ISoundEngine* engine);
	~SoundHandler();

	void SetPosition(irrklang::vec3df position);
	void Play();
	void Stop();
	void SetLoop(bool value);
	void SetStartPaused(bool value);
	irrklang::ISoundEngine* GetEngine();


private:
	irrklang::ISoundEngine* engine;
	const char* filePath;
	irrklang::vec3df position;
	bool loopSound;
	bool startPaused;

	irrklang::ISound* sound;
	irrklang::ISoundSource* source;


};


#endif // !SOUNDHANDLER_H
