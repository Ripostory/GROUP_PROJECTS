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
	  size = 5.0f;
	  angle = 0.0f;
	  orbit = 0.0f;

	  rotationSpeed = 0.3f;
	  orbitSpeed = 0.2f;
	  distance = 20.0f;
	  multiplier = 1.0f;
	  isPlanet = true;
}

Planet::Planet(float rotSpeed, float orbSpeed, float dist, float siz)
{
	  //seed starting angle
	  angle = 0.0f;
	  orbit = rotSpeed * siz * dist;

	  //modify with multipliers
	  rotationSpeed = rotSpeed * ROTATION_MULT;
	  orbitSpeed = orbSpeed * ORBIT_MULT;
	  distance = dist * DISTANCE_MULT;
	  size = siz * SIZE_MULT;
	  multiplier = 1.0f;
	  isPlanet = true;
}

Planet::~Planet()
{
}

void Planet::Update(unsigned int dt)
{
	  //calculate orbit and convert to position matrix
	  orbit -= (multiplier * dt * M_PI/1000) /orbitSpeed;
	  xPos = glm::sin(orbit);
	  yPos = glm::cos(orbit);
	  model = glm::translate(glm::mat4(1.0f), glm::vec3(xPos*distance, 0.0, yPos*distance));

	  //original rotate code modified to take initial translated matrix
	  angle += (multiplier *  dt * M_PI/1000) /rotationSpeed;
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

void Planet::setSize(float siz)
{
	size = siz;
}
