/*
 * moon.cpp
 *
 *  Created on: Sep 4, 2017
 *      Author: Ronn Quijada
 */

#include "moon.h"

Moon::Moon()
{
	angle = 0.0f;
	orbit = 0.0f;
	rotationSpeed = 1.0f;
	orbitSpeed = 1.0f;
	distance  = 1.0f;
	parent = NULL;
	size = 1.0f;
	multiplier = 1.0f;
	isPlanet = true;
}

Moon::Moon(float rotSpeed, float orbSpeed, float dist, float siz, Object *parentModel)
{
	angle = 0.0f;
	orbit = rotSpeed * siz * dist;
	rotationSpeed = rotSpeed * ROTATION_MULT;
	orbitSpeed = orbSpeed * ORBIT_MULT;
	distance  = dist * DISTANCE_MULT;
	parent = parentModel;
	size = siz * SIZE_MULT;
	multiplier = 1.0f;
	isPlanet = true;
}

Moon::~Moon()
{

}

void Moon::Update(unsigned int dt)
{
	//get vec3 translation from parent mat4
	glm::vec4 parentPos = parent->GetModel() * glm::vec4(0.0, 0.0, 0.0, 1.0);

	//translate it based on the parent position
	model = glm::translate(glm::mat4(1.0f), (glm::vec3) parentPos);

	//calculate orbit and convert to position vector
	orbit -= (multiplier * dt * M_PI/1000) /orbitSpeed;
	xPos = glm::sin(orbit);
	yPos = glm::cos(orbit);
	model = glm::translate(model, glm::vec3(xPos*distance, 0.0, yPos*distance));

	//original rotate code modified to take initial translated matrix
	angle += (multiplier *  dt * M_PI/1000) /rotationSpeed;
	model = glm::rotate(model, (angle), glm::vec3(0.0, 1.0, 0.0));

	//make moon smaller
	model = glm::scale(model, glm::vec3(size));
}

void Moon::setParent(Object *par)
{
	parent = par;
}

void Moon::setSize(float siz)
{
	size = siz;
}
