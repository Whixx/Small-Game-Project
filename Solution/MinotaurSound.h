#ifndef MINOTAURSOUND_H
#define MINOTAURSOUND
#include "Sound.h"

class MinotaurSound : public Sound
{
public:
	MinotaurSound();
	~MinotaurSound();

	void SetPosition(irrklang::vec3df position);
	void PlayFootStep();
	void StopFootStep();	// used if the step sound is a longer file looping rather than a single stomping sound
	void PlayGrowl();

	void Update(irrklang::vec3df position);
	void StopAllSounds();

	void TestMinotaurPosition();

private:
	irrklang::ISoundEffectControl* fx;

	// minotaur sounds
	irrklang::ISound* footStep;
	irrklang::ISoundSource* footStepSource;
	irrklang::ISound* growl;
	irrklang::ISoundSource* growlSource;

	irrklang::vec3df position;

	// test variables
	irrklang::vec3df newPosition;
	float counter = 0;

};

#endif // !MINOTAURSOUND_H
