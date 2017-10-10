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
    float zPos;

    float distance;
    float orbitSpeed;
    float rotationSpeed;
    float tilt;
    float offset;
	Object *parent;
public:
	Moon();
	Moon (std::istream& is, Object* parent);
	Moon(float, float, float, float, Object*);
	Moon(float, float, float, float, float, float, Object*);
	~Moon();

	void setParent(Object*);
	void setSize(float);
	void Update(unsigned int dt);

	friend std::ostream& operator<< (std::ostream& os, const Moon& moon);
	friend std::istream& operator>> (std::istream& is, Moon& moon);

};



#endif /* MOON_H_ */
