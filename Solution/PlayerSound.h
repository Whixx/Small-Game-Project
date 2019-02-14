#ifndef PLAYERSOUND_H
#define PLAYERSOUND_H
#include "Sound.h"

class PlayerSound : public Sound
{
public:
	PlayerSound();
	~PlayerSound();

	// sets the player (listener) position and viewdirection
	void SetPosition(irrklang::vec3df position);
	void SetDirection(irrklang::vec3df direction);

	void SetFootStepPosition();
	void PlayFootStep();

	void SetTorchPosition(irrklang::vec3df position);
	void PlayTorchSound();

	// coin sound will be dynamically determined when thrown?
	void PlayCoinSound(irrklang::vec3df position);

	// use this to update the players position (+ footsteps and torch)
	void Update(irrklang::vec3df position, irrklang::vec3df viewDirection);
	
	void StopAllSounds();

private:
	irrklang::ISoundEffectControl* fx;

	// player sounds
	irrklang::ISound* footStep;
	irrklang::ISoundSource* footStepSource;

	// torch sounds
	irrklang::ISound* torch;
	irrklang::ISoundSource* torchSource;

	// coin sounds
	irrklang::ISound* coin;
	irrklang::ISoundSource* coinSource;

	// variables for setting player (listener) position
	irrklang::vec3df playerPosition;
	irrklang::vec3df playerDirection;
	irrklang::vec3df velPerSecond;	// velPerSecond is used for doppler effects
	irrklang::vec3df upVector;

	irrklang::vec3df torchPosition;
};

#endif // !PLAYERSOUND_H
