
#ifndef GUI_H_
#define GUI_H_

#include "imgui.h"
#include "imgui_impl_sdl_gl3.h"
#include <iostream>

class GUI {
public:
	GUI(SDL_Window *window);
	GUI();
	~GUI();
	void Render();
	void Update(unsigned int dt);
	void initGUI(SDL_Window *window);
private:
	SDL_Window *gWindow;
};


#endif /* GUI_H_ */
