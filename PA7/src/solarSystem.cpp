
#include "solarSystem.h"

SolarSystem::SolarSystem()
{
	angle = 0.0f;
	size = 1.0f;
	rotationSpeed = 0.0f;
	multiplier = 1.0f;
	loadNewModel("assets/planet.obj");
	loadNewTexture("assets/a_pluto.jpg");
	loadNewNormal("assets/n_earth.jpg");
}

SolarSystem::SolarSystem(string filename, float rotation, float siz)
{
	angle = 0.0f;
	size = siz;
	rotationSpeed = rotation;
	multiplier = 1.0f;
	loadNewModel(filename);

	//TODO remove temp files
	loadNewTexture("assets/a_pluto.jpg");
	loadNewNormal("assets/n_earth.jpg");
	addChild(new Planet());
}

SolarSystem::~SolarSystem()
{

}

void SolarSystem::Update(unsigned int dt)
{
	  //listen for event
	  for (int i = 0 ; i < listener.getSize(); i++) {
		  keyboard(listener.getEvent(i));
	  }

	  //original rotate code modified to take initial translated matrix
	  angle += rotationSpeed * multiplier *  dt * M_PI/1000 ;
	  model = glm::rotate(model, (angle), glm::vec3(0.0, 1.0, 0.0));

	  //scale model based on size;
	  model = glm::scale(model, glm::vec3(size));

	  //update children
	  for (int i = 0; i < children.size(); i++)
	  {
		  children[i]->setMultiplier(multiplier);
		  children[i]->Update(dt);
	  }
}

void SolarSystem::setSize(float siz)
{
	size = siz;
}

void SolarSystem::keyboard(eventType event)
{
	if (event.eventVer == SDL_KEYDOWN)
	{

	  // increase rotation speed
	  if (event.key == SDLK_q)
	  {
	  	multiplier += SPEED_STEP;
	  }

	  // decrease rotation speed
	  if (event.key == SDLK_e)
	  {
	   	multiplier -= SPEED_STEP;
	  }
	}
	else if (event.eventVer == SDL_MOUSEBUTTONDOWN)
	{
		//toggle rotation
		if (event.mButton == SDL_BUTTON_RIGHT)
		{
			if (rotationSpeed != 0)
				rotationSpeed = 0.0f;
			else
				rotationSpeed = 0.5f;
		}
	}
}
