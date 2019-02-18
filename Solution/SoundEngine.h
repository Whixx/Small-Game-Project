#ifndef SOUNDENGINE_H
#define SOUNDENGINE
#include <iostream>
#include <glm/glm.hpp>
#include "irrKlang/irrKlang.h"
#pragma comment(lib, "irrKlang.lib")

class SoundEngine
{
public:
	SoundEngine();
	~SoundEngine();

	void Update(glm::vec3 position, glm::vec3 viewDirection);	// updates the listeners position and view direction
	irrklang::ISoundEngine* GetEngine();						// pass engine to objects that will play sounds

private:
	irrklang::ISoundEngine* engine;

	irrklang::vec3df listenerPosition;
	irrklang::vec3df viewDirection;
	irrklang::vec3df velPerSecond;	// velPerSecond is used for doppler effects
	irrklang::vec3df upVector;

};

#endif // !SOUNDENGINE_H
