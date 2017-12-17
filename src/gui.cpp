#include "gui.h"

GUI::GUI(SDL_Window *window)
{
	gWindow = window;
	ImGui_ImplSdlGL3_Init(window);
}

GUI::GUI()
{
	gWindow = NULL;
}

GUI::~GUI()
{
	ImGui_ImplSdlGL3_Shutdown();
}

void GUI::Render()
{
	ImGui::Render();
}

void GUI::Update(unsigned int dt)
{
	ImGui_ImplSdlGL3_NewFrame(gWindow);

}

void GUI::initGUI(SDL_Window *window)
{
	gWindow = window;
	ImGui_ImplSdlGL3_Init(window);
}
