#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>

#include "sound.h"
#include <string>
#include <thread>
using namespace std;




class Window
{
  public:
    Window();
    ~Window();
    bool Initialize(const string &name, int* width, int* height);
    void Swap();
    SDL_Window* getWindow();
		

		static bool PlayAudio ();

		// variable declarations
		static Uint8 *audio_pos; // global pointer to the audio buffer to be played
		static Uint32 audio_len; // remaining length of the sample we have to play


		// prototype for our audio callback
		// see the implementation for more information
		static void my_audio_callback(void *userdata, Uint8 *stream, int len);


  private:
    SDL_Window* gWindow;
    SDL_GLContext gContext;
};

#endif /* WINDOW_H */
