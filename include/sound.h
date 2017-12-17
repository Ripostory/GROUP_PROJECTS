#ifndef SOUND_H
#define SOUND_H

#include <SDL2/SDL.h>
#include <string>
#include <thread>
using namespace std;


struct AudioData {

	Uint8* position;
	Uint32 length;
};

class Sound {

public:

	Sound ();
	~Sound ();
	bool LoadAudio (string, int);
	bool PlayAudio ();

private:

	AudioData audio;
	SDL_AudioSpec wav_spec;
	SDL_AudioDeviceID audioDevice;
	int time;

	Uint8* start;
	Uint32 length;

	static void AudioCallback (void*, Uint8*, int);
};



#endif
