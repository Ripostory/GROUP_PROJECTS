#ifndef SOUND_H
#define SOUND_H

#define SOUND_TYPE_MUSIC 0
#define SOUND_TYPE_FX 1


#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <string>
using namespace std;

class Sound {

public:
	static bool Initialize ();

	Sound ();
	Sound (string, int);
	~Sound ();

	bool LoadSoundFX (string);
	bool LoadMusic (string);

	bool PlaySoundFX ();
	bool PlayMusic ();
	bool PauseMusic ();
	bool StopMusic ();

private:
	Mix_Chunk* gSound;
	Mix_Music* gMusic;
};


#endif
