/*
 * kineObject.h
 *
 *  Created on: Dec 2, 2017
 *      Author: Ronn Quijada
 */

#ifndef INCLUDE_KINEOBJECT_H_
#define INCLUDE_KINEOBJECT_H_

#include "physObject.h"

class KinematicObject : public PhysObject {
public:
	KinematicObject();
	~KinematicObject();
	void Update(unsigned int dt);
	void initPhysics();
private:
};



#endif /* INCLUDE_KINEOBJECT_H_ */
