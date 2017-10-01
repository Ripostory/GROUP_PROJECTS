
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
	multiplier = 0.01f;
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
	Object *moon;
	//planet: rotation, orbit, distance, size
	//moon: rotation, orbit, distance, size, parent

	//visuals: model, albedo, normal

	//load visuals for the sun
	size = 1090.0f * 0.2f;
	rotationSpeed = 0.1f;
	setVisual("models/planet.obj", "planets/a_venus.jpg", "planets/n_venus.jpg");

	//Mercury
	loader = new Planet(-590.0f, 2.4f, 30.0f, 3.8f);
	loader->setVisual("models/planet.obj", "planets/a_mercury.jpg", "planets/n_mercury.jpg");
	addChild(loader);

	//Venus
	loader = new Planet(-2430.0f, 6.1f, 33.0f, 9.5f);
	loader->setVisual("models/planet.obj", "planets/a_venus.jpg", "planets/n_venus.jpg");
	addChild(loader);

	//Earth [REFERENCE]
	loader = new Planet(10.0f, 10.0f, 35.0f, 10.0f);
	loader->setVisual("models/planet.obj", "planets/a_earth.jpg", "planets/n_earth.jpg");
	loader->loadNewTexture("planets/m_earth.jpg", 2);
	loader->loadNewTexture("planets/s_earth.jpg", 3);
	loader->setEarth(true);
	addChild(loader);

		//moon
		moon = new Moon(-10.0f, 0.83f, 0.5f, 2.7f, loader);
		moon->setVisual("models/planet.obj", "moons/a_moon.jpg", "moons/n_moon.jpg");
		loader->addChild(moon);

	//Mars
	loader = new Planet(10.1f, 18.8f, 37.0f, 5.3f);
	loader->setVisual("models/planet.obj", "planets/a_mars.jpg", "planets/n_mars.jpg");
	addChild(loader);

		//pheobes

		//deimos


	//Jupiter
	loader = new Planet(3.7f, 118.6f, 40.0f, 112.0f);
	loader->setGasGiant(true);
	loader->setVisual("models/planet.obj", "planets/a_jupiter.jpg", "planets/n_mars.jpg");
	addChild(loader);

		//io
		moon = new Moon(7.0f, 0.43f, 3.5f, 2.7f, loader);
		moon->setVisual("models/planet.obj", "moons/a_moon.jpg", "moons/n_moon.jpg");
		loader->addChild(moon);

		//castillo
		moon = new Moon(5.0f, 0.73f, 4.5f, 8.7f, loader);
		moon->setVisual("models/planet.obj", "moons/a_moon.jpg", "moons/n_moon.jpg");
		loader->addChild(moon);

		//ganymede
		moon = new Moon(10.0f, 0.83f, 6.2f, 4.4f, loader);
		moon->setVisual("models/planet.obj", "moons/a_moon.jpg", "moons/n_moon.jpg");
		loader->addChild(moon);

		//europa
		moon = new Moon(4.0f, 1.0f, 7.5f, 9.7f, loader);
		moon->setVisual("models/planet.obj", "moons/a_moon.jpg", "moons/n_moon.jpg");
		loader->addChild(moon);


	//Saturn
	loader = new Planet(4.1f, 294.6f, 45.0f, 94.5f);
	loader->setGasGiant(true);
	loader->setVisual("models/planet.obj", "planets/a_saturn.jpg", "planets/n_mars.jpg");
	addChild(loader);

		//saturns ring
		moon = new Moon(4.1f, 0.1f, 0.0f, 94.5f, loader);
		moon->setVisual("models/ring_saturn.obj", "rings/ring_saturn.png", "moons/n_moon.jpg");
		moon->setRing(true);
		loader->addChild(moon);

	//Uranus
	loader = new Planet(5.4f, 840.6f, 48.0f, 40.0f);
	loader->setGasGiant(true);
	loader->setVisual("models/planet.obj", "planets/a_uranus.jpg", "planets/n_mars.jpg");
	addChild(loader);

		//uranus ring
		moon = new Moon(5.4f, 0.1f, 0.0f, 40.0f, loader);
		moon->setVisual("models/ring_uranus.obj", "rings/ring_uranus.png", "moons/n_moon.jpg");
		moon->setRing(true);
		loader->addChild(moon);


	//Neptune
	loader = new Planet(6.2f, 1648.1f, 50.0f, 38.8f);
	loader->setGasGiant(true);
	loader->setVisual("models/planet.obj", "planets/a_neptune.jpg", "planets/n_mars.jpg");
	addChild(loader);

	//Pluto
	loader = new Planet(10.0f, 2477.0f, 60.0f, 1.6f);
	loader->setVisual("models/planet.obj", "planets/a_pluto.jpg", "planets/n_pluto.jpg");
	addChild(loader);

		//that one giant moon about the size of pluto
		moon = new Moon(10.0f, 1.0f, 0.12f, 1.2f, loader);
		moon->setVisual("models/planet.obj", "planets/a_mercury.jpg", "moons/n_moon.jpg");
		loader->addChild(moon);
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

	  //pause and resume
	  if (event.key == SDLK_SPACE)
	  {
	  	multiplier = 0;
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
