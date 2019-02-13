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
	void SetPlayerPosition(irrklang::vec3df position, irrklang::vec3df direction);

	// set position of the sound sources
	void SetMusicPosition(irrklang::vec3df position);
	void SetPlayerFootStepPosition(irrklang::vec3df position);
	void PlayPlayerFootStepSound(float time);
	void SetTorchPosition(irrklang::vec3df position);
	void SetMinotaurFootPosition(irrklang::vec3df position);
	void SetMinotaurGrowlPosition(irrklang::vec3df position);
	void SetCoinPosition(irrklang::vec3df position);


private:
	irrklang::ISoundEngine* engine;
	// sounds
	irrklang::ISound* music;
	irrklang::ISound* playerFootStep;
	irrklang::ISound* torch;
	irrklang::ISound* minotaurFootStep;
	irrklang::ISound* minotaurGrowl;
	irrklang::ISound* coin;

	float playerStepTime;

};


#endif // !SOUND_H
