#include "gui.h"

GUI* GUI::m_instance = NULL;

GUI::GUI(SDL_Window *window)
{
	gWindow = window;
	ImGui_ImplSdlGL3_Init(window);

	score = 0;
	lives = 3;
	newLifeCountdown = 1000;

	m_instance = this;
}

GUI::GUI()
{
	gWindow = NULL;
	m_instance = this;

	score = 0;
	lives = 3;
	newLifeCountdown = 1000;
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
	ImGui::Text(
		"Score: %d\n \
Lives: %d\n \
PRESS k TO LOAD BALL \
     ", this -> score, this -> lives);
}

void GUI::initGUI(SDL_Window *window)
{
	gWindow = window;
	ImGui_ImplSdlGL3_Init(window);
}

void GUI::ChangeScore (int delta) {

	std::cout << "Adding " << delta << " to score " << score << std::endl;

	newLifeCountdown -= delta;
	score += delta;

	if (newLifeCountdown <= 0) {

		ChangeLives (1);
		newLifeCountdown = 1000;
	}
}

void GUI::ChangeLives (int delta) {

	std::cout << "Adding " << delta << " to lives " << std::endl;

	lives += delta;
}

int GUI::GetLives () {

	return lives;
}

int GUI::GetScore () {

	return score;
}

GUI* GUI::getInstance () {

	return m_instance;
}
