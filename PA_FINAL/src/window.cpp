#include <window.h>

Uint8* Window::audio_pos = NULL; // global pointer to the audio buffer to be played
Uint32 Window::audio_len = 0; // remaining length of the sample we have to play


Window::Window()
{
  gWindow = NULL;
}

Window::~Window()
{
  SDL_StopTextInput();
  SDL_DestroyWindow(gWindow);
  gWindow = NULL;
  SDL_Quit();
}

bool Window::Initialize(const string &name, int* width, int* height)
{
    // Start SDL
  if(SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    printf("SDL failed to initialize: %s\n", SDL_GetError());
    return false;
  }

  // Start OpenGL for SDL
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
  SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );
  SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );

  // Create window
  SDL_DisplayMode current;
  SDL_GetCurrentDisplayMode(0, &current);
  
  //use for fullscreen
  if (*height == 0 && *width == 0)
  {
    *height = current.h;
    *width = current.w;
  }

  gWindow = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, *width, *height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
  if(gWindow == NULL)
  {
    printf("Widow failed to create: %s\n", SDL_GetError());
    return false;
  }

  // Create context
  gContext = SDL_GL_CreateContext(gWindow);
  if(gContext == NULL)
  {
    printf("OpenGL context not created: %s\n", SDL_GetError());
    return false;
  }

  // Use VSync
  if(SDL_GL_SetSwapInterval(1) < 0)
  {
    printf("Unable to use VSync: %s\n", SDL_GetError());
    return false;
  }

	SDL_SetRelativeMouseMode (SDL_TRUE);

	//Initialize SDL Audio
	if (SDL_Init (SDL_INIT_AUDIO) < 0)
		return false;

	PlayAudio ();

  return true;
}


bool Window::PlayAudio () {

	// local variables
	static Uint32 wav_length; // length of our sample
	static Uint8 *wav_buffer; // buffer containing our audio file
	static SDL_AudioSpec wav_spec; // the specs of our piece of music
	
	
	/* Load the WAV */
	// the specs, length and buffer of our wav are filled
	if( SDL_LoadWAV("assets/sounds/Sample-Audio.wav", &wav_spec, &wav_buffer, &wav_length) == NULL ){
	  return false;
	}
	// set the callback function
	wav_spec.callback = my_audio_callback;
	wav_spec.userdata = NULL;
	// set our global static variables
	audio_pos = wav_buffer; // copy sound buffer
	audio_len = wav_length; // copy file length
	
	/* Open the audio device */
	if ( SDL_OpenAudio(&wav_spec, NULL) < 0 ){
	  fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
	  //exit(-1);
	}
	
	/* Start playing */
	SDL_PauseAudio(0);

	

	// wait until we're done playing
	while (audio_len > 0) {
		SDL_Delay(10); 
	}
	
	// shut everything down
	SDL_CloseAudio();
	SDL_FreeWAV(wav_buffer);

	return true;
}

// audio callback function
// here you have to copy the data of your audio buffer into the
// requesting audio buffer (stream)
// you should only copy as much as the requested length (len)
void Window::my_audio_callback(void *userdata, Uint8 *stream, int len) {
	
	static int calls = 0;

	printf ("%d\n", calls);

	calls++;

	if (audio_len ==0)
		return;
	
	len = ( len > audio_len ? audio_len : len );
	SDL_memcpy (stream, audio_pos, len); 					// simply copy from one buffer into the other
	SDL_MixAudio(stream, audio_pos, len, SDL_MIX_MAXVOLUME);// mix from one buffer into another
	
	audio_pos += len;
	audio_len -= len;
}

void Window::Swap()
{
  SDL_GL_SwapWindow(gWindow);
}

SDL_Window* Window::getWindow()
{
	return gWindow;
}

