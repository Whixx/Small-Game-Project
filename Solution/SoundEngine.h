#ifndef SOUNDENGINE_H
#define SOUNDENGINE
#include <iostream>
#include "irrKlang/irrKlang.h"
#pragma comment(lib, "irrKlang.lib")
//#include "irrKlang/ik_ISoundSource.h"
//#include "irrKlang/ik_ISound.h"

class SoundEngine
{
public:
	SoundEngine();
	~SoundEngine();

	//void SetListenerPosition(irrklang::vec3df position, irrklang::vec3df viewDirection);
	void Update(irrklang::vec3df position, irrklang::vec3df viewDirection);
	irrklang::ISoundEngine* GetEngine();

private:
	irrklang::ISoundEngine* engine;

	irrklang::vec3df listenerPosition;
	irrklang::vec3df viewDirection;
	irrklang::vec3df velPerSecond;	// velPerSecond is used for doppler effects
	irrklang::vec3df upVector;

};

#endif // !SOUNDENGINE_H
