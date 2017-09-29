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
	  loadNewModel("planet.obj");
	  loadNewTexture("a_earth.jpg");
	  loadNewNormal("n_earth.jpg");
	  setSize(5.0f);
	  angle = 0.0f;
	  orbit = 0.0f;

	  rotationSpeed = 0.3f;
	  orbitSpeed = 0.2f;
	  distance = 20.0f;
	  multiplier = 1.0f;
	  isPlanet = true;
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
	  multiplier = 1.0f;
}

Planet::~Planet()
{
}

void Planet::Update(unsigned int dt)
{
	  //calculate orbit and convert to position matrix
	  orbit -= orbitSpeed * multiplier * dt * M_PI/1000 ;
	  xPos = glm::sin(orbit);
	  yPos = glm::cos(orbit);
	  model = glm::translate(glm::mat4(1.0f), glm::vec3(xPos*distance, 0.0, yPos*distance));

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

void Planet::setSize(float siz)
{
	size = siz;
}
