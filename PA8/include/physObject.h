/*
 * physObject.h
 *
 *  Created on: Oct 11, 2017
 *      Author: nog
 */

#ifndef PHYSOBJECT_H_
#define PHYSOBJECT_H_

#include "object.h"

class PhysObject : public Object {
public:
	PhysObject();
	virtual ~PhysObject();
	virtual void Update(unsigned int dt);
    void initPhyiscs();
private:
    btRigidBody *physics;
    btTransform transform;

    float mass;
    float inertia;
};



#endif /* PHYSOBJECT_H_ */
