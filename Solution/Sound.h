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
	void PlayPlayerFootStep();

	void SetTorchPosition(irrklang::vec3df position);
	void PlayTorchSound();

	void SetMinotaurFootPosition(irrklang::vec3df position);
	void PlayMinotaurFootStep();
	void StopMinotaurFootstep(); // används om fotstegen är en lång loop istället för ett steg som upprepas
	void SetMinotaurGrowlPosition(irrklang::vec3df position);
	void PlayMinotaurGrowl();

	void PlayCoinSound(irrklang::vec3df position);

	void Update(irrklang::vec3df position, irrklang::vec3df viewDirection);
	void UpdatePlayer(irrklang::vec3df position, irrklang::vec3df viewDirection);
	void UpdateMinotaur(irrklang::vec3df position);
	void StopAllSounds();

private:
	irrklang::ISoundEngine* engine;
	irrklang::ISoundEffectControl* fx;

	// sounds
	irrklang::ISound* music;

	// player sounds
	irrklang::ISound* playerFootStep;
	irrklang::ISoundSource* playerFootStepSource;

	// torch sounds
	irrklang::ISound* torch;

	// minotaur sounds
	irrklang::ISound* minotaurFootStep;
	irrklang::ISoundSource* minotaurFootStepSource;
	irrklang::ISound* minotaurGrowl;
	irrklang::ISoundSource* minotaurGrowlSource;

	// coin sounds
	irrklang::ISound* coin;
	irrklang::ISoundSource* coinSource;

	// variables for setting player (listener) position
	irrklang::vec3df velPerSecond, upVector; // velPerSecond is used for doppler effects

	irrklang::vec3df playerPosition;
	irrklang::vec3df minotaurPosition;

	irrklang::vec3df newPosition;
	float counter = 0;

};


#endif // !SOUND_H
