/*
 * planet.h
 *
 *  Created on: Sep 4, 2017
 *      Author: Ronn Quijada
 */

#ifndef PLANET_H_
#define PLANET_H_

#include "object.h"

class Planet : public Object
{
private:
protected:
	virtual void keyboard(eventType);
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
	Planet(string, float, float, float, float);
	virtual void Update(unsigned int);
	~Planet();
};



#endif /* PLANET_H_ */
