/*
 * physObject.h
 *
 *  Created on: Oct 11, 2017
 *      Author: nog
 */

#ifndef PHYSOBJECT_H_
#define PHYSOBJECT_H_

#include <bullet/btBulletDynamicsCommon.h>
#include "object.h"

#define PHYS_BOX 		0
#define PHYS_SHPERE 	1
#define PHSY_CASPSULE 	2
#define PHYS_CYLINDER	3
#define PHYS_CONE		4


class PhysObject : public Object {
public:
	PhysObject();
	virtual ~PhysObject();
	virtual void Update(unsigned int dt);
	virtual void Begin();
    void initPhyiscs();
    void setCollisionMesh(int);
    void setCollisionMesh(string filename);
protected:
    glm::vec3 btToGlm(btVector3);
    glm::mat4 btToGlm(btTransform);
    btVector3 glmToBt(glm::vec3);

private:
    btRigidBody *physics;
    btCollisionShape *shape;
    btTransform transform;

    float mass;
};



#endif /* PHYSOBJECT_H_ */
