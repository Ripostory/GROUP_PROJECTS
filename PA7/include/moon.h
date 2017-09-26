/*
 * moon.h
 *
 *  Created on: Sep 4, 2017
 *      Author: Ronn Quijada
 */

#ifndef MOON_H_
#define MOON_H_

#include "planet.h"

class Moon : public Planet
{
private:
	Object *parent;
public:
	Moon();
	Moon(float, float, float, Object*);
	~Moon();

	void Update(unsigned int dt);
};



#endif /* MOON_H_ */
