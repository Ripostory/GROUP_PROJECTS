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
	void Update(unsigned int);
	std::vector<Object*> getChildren();
	~Planet();

	void addMoon(Object*);
	void clearMoons();

	void setSize(float);
};



#endif /* PLANET_H_ */
