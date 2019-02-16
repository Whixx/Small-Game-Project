#ifndef SOUNDENGINE_H
#define SOUNDENGINE
#include <iostream>
#include "irrKlang/irrKlang.h"
#pragma comment(lib, "irrKlang.lib")

class SoundEngine
{
public:
	SoundEngine();
	~SoundEngine();

	void Update(irrklang::vec3df position, irrklang::vec3df viewDirection);	// updates the listeners position and view direction
	irrklang::ISoundEngine* GetEngine();	// pass engine to objects that will play sounds

private:
	irrklang::ISoundEngine* engine;

	irrklang::vec3df listenerPosition;
	irrklang::vec3df viewDirection;
	irrklang::vec3df velPerSecond;	// velPerSecond is used for doppler effects
	irrklang::vec3df upVector;

};

#endif // !SOUNDENGINE_H
