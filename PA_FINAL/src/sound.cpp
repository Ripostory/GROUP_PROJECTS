#include "sound.h"

Sound::Sound () { 

	isPlaying = false;
	isSingle = false;
}
Sound::~Sound () {

	if (audioDevice != 0)
		SDL_CloseAudioDevice (audioDevice);

	if (start != NULL)
		SDL_FreeWAV (start);
}

bool Sound::LoadAudio (string file, float time) {

	if (isPlaying) {

		KillAudio ();
	}

	if( SDL_LoadWAV(file.c_str(), &wav_spec, &start, &length) == NULL ) {
		printf ("Unable to load audio\n");
	  return false;
	}

	this -> time = time;

	wav_spec.callback = AudioCallback;

	audio.position = 0;
	audio.length = 0; //Assign values to audio

	wav_spec.userdata = &audio;


	return true;
}

bool Sound::PlayAudio () {
	if (isPlaying)
		return true;


	isPlaying = true;

	audio.position = start;
	audio.length = length;

	audioDevice = SDL_OpenAudioDevice (NULL, 0, &wav_spec, NULL, 0);

	if (audioDevice != 0) {
		SDL_PauseAudioDevice (audioDevice, 0);

	}
	else {

		printf ("Unable to play audio\n");
		return false;
	}

	return true;
}

bool Sound::PlaySingle()
{
	if (!PlayAudio())
		return false;
	else
	{
		//add a timer to stop audio
		animator.timer(time, 100);
		isSingle = true;
		return true;
	}
}

bool Sound::KillAudio () {

	if (audioDevice == 0)
		return false; //There is nothing to kill

	isPlaying = false;

	SDL_CloseAudioDevice (audioDevice);
	return true;
}


void Sound::AudioCallback(void *userdata, Uint8 *stream, int len) {

	//Convert void pointer to audio data pointer
	AudioData* data = (AudioData*)userdata;

	//Check if audio has finished playing
	if (data -> length <= 0) {
		return;
	}

	//Play audio
	len = ( len > data -> length ? data -> length : len );
	SDL_memcpy (stream, data -> position, len);
	SDL_MixAudio (stream, data -> position, len, SDL_MIX_MAXVOLUME);
	
	data -> position += len;
	data -> length -= len;
}

void Sound::Update(unsigned int dt)
{
	//update animator
	animator.Update(dt);

	if (isSingle && isPlaying)
	{
		if (!animator.isPending(100))
		{
			KillAudio();
		}
	}
}

bool Sound::isDone()
{
	if (isPlaying)
		return false;
	else
	{
		if (isSingle)
			return true;
		return false;
	}
}

