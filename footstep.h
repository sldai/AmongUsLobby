#pragma once
#include <irrklang/irrKlang.h>

using namespace irrklang;

class FootStep
{
public:
	FootStep();
	~FootStep();
	void reset()
	{
		t = 0;
		last_sound = 0;
	}
	void update(float dt)
	{
		t += dt;
		if (t - last_sound >= 1.0f / freq)
		{
			SoundEngine->play2D("resources/audio/solid.wav",false);
			last_sound = t;
		}
	}
private:
	ISoundEngine* SoundEngine;
	float t;
	float last_sound;
	float freq = 2.0;
};

FootStep::FootStep()
{
	SoundEngine = createIrrKlangDevice();;
	reset();
}

FootStep::~FootStep()
{
	delete SoundEngine;
}