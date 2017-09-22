/*
 * planet.cpp
 *
 *  Created on: Sep 4, 2017
 *      Author: Ronn Quijada
 */

#include "planet.h"

Planet::Planet()
{

}

Planet::~Planet()
{

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
		  loadNewModel("assets/planetTEMP.obj");
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


