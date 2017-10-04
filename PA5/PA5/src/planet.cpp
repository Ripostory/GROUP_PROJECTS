/*
 * planet.cpp
 *
 *  Created on: Sep 4, 2017
 *      Author: Ronn Quijada
 */

#include "planet.h"

Planet::Planet()
{
	  //updated file loading, default to cube object
	  loadNewModel(Object::defaultPath);

	  angle = 0.0f;
	  orbit = 0.0f;

	  rotationSpeed = 0.3f;
	  orbitSpeed = 0.0f;
	  distance = 0.0f;
	  size = 1.0f;
}

Planet::Planet(string filename, float rotSpeed, float orbSpeed, float dist, float siz)
{
	  loadNewModel(filename);
	  angle = 0.0f;
	  orbit = 0.0f;

	  rotationSpeed = rotSpeed;
	  orbitSpeed = orbSpeed;
	  distance = dist;
	  size = siz;
}

Planet::~Planet()
{

}

void Planet::Update(unsigned int dt)
{
	  //listen for event
	  for (int i = 0 ; i < listener.getSize(); i++) {
		  keyboard(listener.getEvent(i));
	  }
	  //calculate orbit and convert to position matrix
	  orbit -= orbitSpeed * dt * M_PI/1000;
	  xPos = glm::sin(orbit);
	  yPos = glm::cos(orbit);
	  model = glm::translate(glm::mat4(1.0f), glm::vec3(xPos*distance, 0.0, yPos*distance));

	  //original rotate code modified to take initial translated matrix
	  angle += rotationSpeed * dt * M_PI/1000;
	  model = glm::rotate(model, (angle), glm::vec3(0.0, 1.0, 0.0));

	  //scale model based on size;
	  model = glm::scale(model, glm::vec3(size));
}

void Planet::keyboard(eventType event)
{
	if (event.eventVer == SDL_KEYDOWN)
	{
	  // increase orbit speed
	  if (event.key == SDLK_a)
	  {
	   	orbitSpeed += SPEED_STEP;
	  }

	  // decrease orbit speed
	  if (event.key == SDLK_d)
	  {
	  	orbitSpeed -= SPEED_STEP;
	  }

	  // increase rotation speed
	  if (event.key == SDLK_w)
	  {
	  	rotationSpeed += SPEED_STEP;
	  }

	  // decrease rotation speed
	  if (event.key == SDLK_s)
	  {
	   	rotationSpeed -= SPEED_STEP;
	  }

	  // toggle orbit direction
	  if (event.key == SDLK_q)
	  {
	   	if (orbitSpeed > 0)
	   		orbitSpeed = -0.5f;
	   	else
	  		orbitSpeed = 0.5f;
	  }

	  // toggle rotation direction
	  if (event.key == SDLK_e)
	  {
	   	if (rotationSpeed > 0)
	   		rotationSpeed = -0.5f;
	   	else
	  		rotationSpeed = 0.5f;
	  }

	  //choose model
	  if (event.key == SDLK_t)
	  {
		  loadNewModel("assets/teapot.obj");
	  }

	  if (event.key == SDLK_y)
	  {
		  loadNewModel("assets/board.obj");
	  }

	  if (event.key == SDLK_u)
	  {
		  loadNewModel("assets/planet.obj");
	  }
	  if (event.key == SLDK_i) {
		  loadNewModel (Object::defaultPath);
	  }

	  // orbit clockwise
	  if (event.key == SDLK_LEFT)
	  {
	   	if (orbitSpeed > 0)
	   		orbitSpeed = 0 - orbitSpeed;
	  }

	  // orbit counterclockwise
	  if (event.key == SDLK_RIGHT)
	  {
	   	if (orbitSpeed < 0)
	   		orbitSpeed = 0 - orbitSpeed;
	  }
	}
	else if (event.eventVer == SDL_MOUSEBUTTONDOWN)
	{
		//toggle orbit
		if (event.mButton == SDL_BUTTON_LEFT)
		{
			if (orbitSpeed != 0)
				orbitSpeed = 0.0f;
			else
				orbitSpeed = 0.5f;
		}

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


