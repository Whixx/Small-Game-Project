#ifndef SOUND_H
#define SOUND_H
#include <iostream>
#include <irrKlang/irrKlang.h>
#include <vector>
#pragma comment(lib, "irrKlang.lib")

class Sound
{
public:
	Sound();
	~Sound();

	// set player (camera) position and view direction
	virtual void SetPosition(irrklang::vec3df position) = 0;

	// two update functions, first one is for player (listener) who also needs a view direction
	virtual void Update(irrklang::vec3df position, irrklang::vec3df viewDirection);
	virtual void Update(irrklang::vec3df position);

	virtual void StopAllSounds() = 0;

	irrklang::ISoundEngine* GetEngine();

private:
	irrklang::ISoundEngine* engine;
	//irrklang::ISoundEffectControl* fx;

};


#endif // !SOUND_H
