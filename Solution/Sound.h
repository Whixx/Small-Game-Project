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
	void PlayPlayerFootStep(float time);

	void SetTorchPosition(irrklang::vec3df position);
	void PlayTorchSound();

	void SetMinotaurFootPosition(irrklang::vec3df position);
	void PlayMinotaurFootStep();
	void SetMinotaurGrowlPosition(irrklang::vec3df position);
	void PlayMinotaurGrowl();

	void SetCoinPosition(irrklang::vec3df position);
	void PlayCoinSound();

	void Update();

private:
	irrklang::ISoundEngine* engine;
	irrklang::ISoundEffectControl* fx;

	// sounds
	irrklang::ISound* music;

	// player sounds
	irrklang::ISound* playerFootStep;

	// torch sounds
	irrklang::ISound* torch;

	// minotaur sounds
	irrklang::ISound* minotaurFootStep;
	irrklang::ISound* minotaurGrowl;

	// coin sounds
	irrklang::ISound* coin;

	float playerStepTime;
	irrklang::vec3df newPosition;
	float counter = 0;

};


#endif // !SOUND_H
