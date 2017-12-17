#ifndef SOUND_H
#define SOUND_H

#include <SDL2/SDL.h>
#include <string>
#include <thread>
#include "Animator.h"
using namespace std;


struct AudioData {

	Uint8* position;
	Uint32 length;
};

class Sound {

public:

	Sound ();
	~Sound ();
	void Update(unsigned int dt);
	bool LoadAudio (string, float);
	bool PlayAudio ();
	bool KillAudio ();
	bool PlaySingle();
	bool isDone();

	Animator animator;
private:

	AudioData audio;
	SDL_AudioSpec wav_spec;
	SDL_AudioDeviceID audioDevice;
	float time;

	Uint8* start;
	Uint32 length;

	bool isPlaying;
	bool isSingle;
	static void AudioCallback (void*, Uint8*, int);


};



#endif
