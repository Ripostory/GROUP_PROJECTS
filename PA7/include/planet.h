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

#define ROTATION_MULT	1.0f
#define ORBIT_MULT		0.002f
#define DISTANCE_MULT	10.0f
#define SIZE_MULT		0.2f

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
    float size;

public:
	Planet();
	Planet(float rotSpeed, float orbSpeed, float dist, float siz);
	void Update(unsigned int);
	~Planet();

	void addMoon(Object*);
	void clearMoons();

	void setSize(float);
};



#endif /* PLANET_H_ */
