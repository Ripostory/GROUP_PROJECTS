#include "sound.h"


Sound::Sound ( ) {

	gMusic = NULL;
	gSound = NULL;
}

Sound::Sound (string file, int type) {

	if (type == SOUND_TYPE_MUSIC) {

		gMusic = Mix_LoadMUS ( file.c_str () );
		if (gMusic == NULL) {
			
			printf ("Failed to load music! SDL_mixer Error: %s\n", Mix_GetError () );
		}
	}

	if (type == SOUND_TYPE_FX) {

		gSound = Mix_LoadWAV ( file.c_str () );
		if (gSound == NULL) {
			
			printf ("Failed to load sound effect! SDL_mixer Error: %s\n", Mix_GetError () );
		}
	}
}

Sound::~Sound () {

	if (gSound != NULL) {
		Mix_FreeChunk ( gSound );
		gSound = NULL;
	}

	if (gMusic != NULL) {
		Mix_FreeMusic ( gMusic );
		gMusic = NULL;
	}
}


bool Sound::Initialize () {

	if ( SDL_Init ( SDL_INIT_AUDIO ) < 0 ) {
		printf ( "SDL could not initialize! SDL Error %s\n", SDL_GetError () );
		return false;
	}

	if ( Mix_OpenAudio ( 44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0 ) {
		printf ( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError () );
		return false;
	}

	return true;
}

bool Sound::LoadSoundFX ( string file ) {

		gSound = Mix_LoadWAV ( file.c_str () );
	if (gSound == NULL) {
			
			printf ("Failed to load sound effect! SDL_mixer Error: %s\n", Mix_GetError () );
			return false;
	}
	return true;
}

bool Sound::LoadMusic ( string file ) {

	gMusic = Mix_LoadMUS ( file.c_str () );
	if (gMusic == NULL) {
			
			printf ("Failed to load sound effect! SDL_mixer Error: %s\n", Mix_GetError () );
			return false;
	}
	return true;
}

bool Sound::PlaySoundFX () {

	if ( gSound == NULL ) {
		printf ( "Sound effect not loaded\n" );
		return false;
	}

	Mix_PlayChannel ( -1, gSound, 0 );
	return true;
}

bool Sound::PlayMusic () {

	if ( gMusic == NULL ) {
		printf ( "Music not loaded\n" );
		return false;
	}

	if ( Mix_PlayingMusic () == 0 ) {

		Mix_PlayMusic ( gMusic, -1);
		return true;
	}

	printf ("There already exists a sound object that is playing music\n");
	return false;
	
}

bool Sound::PauseMusic () {

	if ( gMusic == NULL ) {
		printf ( "Music not loaded\n" );
		return false;
	}

	Mix_PauseMusic ();
	return true;
}
bool Sound::StopMusic () {

	if ( gMusic == NULL ) {
		printf ( "Music not loaded\n" );
		return false;
	}

	if ( Mix_PausedMusic () == 1)
		Mix_ResumeMusic ();

	return true;
}
