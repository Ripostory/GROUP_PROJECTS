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
public:
	Planet();
	~Planet();
};



#endif /* PLANET_H_ */
