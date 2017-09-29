
#include "solarSystem.h"

SolarSystem::SolarSystem()
{
	angle = 0.0f;
	size = 1.0f;
	rotationSpeed = 0.0f;
	multiplier = 10.0f;
}

SolarSystem::SolarSystem(float rotation, float siz)
{
	angle = 0.0f;
	size = siz;
	rotationSpeed = rotation;
	multiplier = 1.0f;
}

SolarSystem::~SolarSystem()
{

}

void SolarSystem::LoadSolSystem(string filename)
{
	//TODO load from a file using filename

	/*===============
	 * LOAD SOLAR SYSTEM
	 *
	 * for now, is loaded via a whole bunch of function calls
	 * This function will read from a file and setup all the
	 * visuals, textures, and data for all objects
	 *
	 * note: all values have been scaled to be in relation to earth
	 * the default size is 10.0f
	 * =============*/

	Object *loader;
	Object *moons;
	//planet: rotation, orbit, distance, size
	//moon: rotation, orbit, distance, size, parent

	//visuals: model, albedo, normal

	//load visuals for the sun
	size = 1090.0f * 0.2f;
	rotationSpeed = 0.1f;
	setVisual("models/planet.obj", "planets/a_venus.jpg", "planets/n_venus.jpg");

	//Mercury
	loader = new Planet(10.0f, 10.0f, 30.0f, 3.8f);
	loader->setVisual("models/planet.obj", "planets/a_mercury.jpg", "planets/n_mercury.jpg");
	addChild(loader);

	//Venus
	loader = new Planet(10.0f, 10.0f, 33.0f, 9.5f);
	loader->setVisual("models/planet.obj", "planets/a_venus.jpg", "planets/n_venus.jpg");
	addChild(loader);

	//Earth [REFERENCE]
	loader = new Planet(10.0f, 10.0f, 35.0f, 10.0f);
	loader->setVisual("models/planet.obj", "planets/a_earth.jpg", "planets/n_earth.jpg");
	addChild(loader);

	//Mars
	loader = new Planet(10.0f, 10.0f, 37.0f, 5.3f);
	loader->setVisual("models/planet.obj", "planets/a_mars.jpg", "planets/n_mars.jpg");
	addChild(loader);

	//Jupiter
	loader = new Planet(10.0f, 10.0f, 40.0f, 112.0f);
	loader->setVisual("models/planet.obj", "planets/a_jupiter.jpg", "planets/n_mars.jpg");
	addChild(loader);

	//Saturn
	loader = new Planet(10.0f, 10.0f, 45.0f, 94.5f);
	loader->setVisual("models/planet.obj", "planets/a_saturn.jpg", "planets/n_mars.jpg");
	addChild(loader);

	//Uranus
	loader = new Planet(10.0f, 10.0f, 48.0f, 40.0f);
	loader->setVisual("models/planet.obj", "planets/a_uranus.jpg", "planets/n_mars.jpg");
	addChild(loader);

	//Neptune
	loader = new Planet(10.0f, 10.0f, 50.0f, 38.8f);
	loader->setVisual("models/planet.obj", "planets/a_neptune.jpg", "planets/n_mars.jpg");
	addChild(loader);

	//Pluto
	loader = new Planet(10.0f, 10.0f, 60.0f, 0.16f);
	loader->setVisual("models/planet.obj", "planets/a_pluto.jpg", "planets/n_pluto.jpg");
	addChild(loader);

}

void SolarSystem::Update(unsigned int dt)
{
	  //listen for event
	  for (int i = 0 ; i < listener.getSize(); i++) {
		  keyboard(listener.getEvent(i));
	  }

	  //original rotate code modified to take initial translated matrix
	  angle += rotationSpeed * multiplier *  dt * M_PI/1000 ;
	  model = glm::rotate(glm::mat4(1.0f), (angle), glm::vec3(0.0, 1.0, 0.0));

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
