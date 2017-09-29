/*
 * planet.h
 *
 *  Created on: Sep 4, 2017
 *      Author: Ronn Quijada
 */

#ifndef PLANET_H_
#define PLANET_H_

#include "object.h"
#include "moon.h"

class Planet : public Object
{
private:
protected:
    float angle;
    float orbit;
    float xPos;
    float yPos;

    float distance;
    float orbitSpeed;
    float rotationSpeed;

public:
	Planet();
	Planet(float rotSpeed, float orbSpeed, float dist, float siz);
	void Update(unsigned int);
	~Planet();

	void addMoon(Object*);
	void clearMoons();
	float getRadius();
	void setSize(float);
};



#endif /* PLANET_H_ */
