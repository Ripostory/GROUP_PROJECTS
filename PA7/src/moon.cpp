/*
 * moon.cpp
 *
 *  Created on: Sep 4, 2017
 *      Author: Ronn Quijada
 */

#include "moon.h"

Moon::Moon()
{
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
	rotationSpeed = rotSpeed;
	orbitSpeed = orbSpeed;
	distance  = dist;
	parent = parentModel;
	size = siz;
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
	orbit -= orbitSpeed * dt * multiplier * M_PI/1000;
	xPos = glm::sin(orbit);
	yPos = glm::cos(orbit);
	model = glm::translate(model, glm::vec3(xPos*distance, 0.0, yPos*distance));

	//original rotate code modified to take initial translated matrix
	angle += rotationSpeed * dt * multiplier  * M_PI/1000;
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
