/*
 * moon.h
 *
 *  Created on: Sep 4, 2017
 *      Author: Ronn Quijada
 */

#ifndef MOON_H_
#define MOON_H_

#include "planet.h"

class Moon : public Object
{
private:
    float angle;
    float orbit;
    float xPos;
    float yPos;

    float distance;
    float orbitSpeed;
    float rotationSpeed;
    float size;
    float multiplier;
	Object *parent;
public:
	Moon();
	Moon(float, float, float, Object*);
	~Moon();

	void setParent(Object*);
	void setSize(float);
	void Update(unsigned int dt);
};



#endif /* MOON_H_ */
