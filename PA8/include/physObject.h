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
#define PHYS_SPHERE 	1
#define PHYS_CAPSULE 	2
#define PHYS_CYLINDER	3
#define PHYS_CONE		4
#define PHYS_HULL		5
#define PHYS_S_MESH		6

class PhysObject : public Object {
public:
	PhysObject();
	virtual ~PhysObject();
	virtual void Begin();
	virtual void Update(unsigned int dt);
    void initPhysics();
    void setCollisionMesh(int base);
    void setCollisionMesh(int box, glm::vec3 size);
    void setCollisionMesh(int sphere, float radius);
    void setCollisionMesh(int capCylCone, float, float);
    void setCollisionMesh(int mesh, string filename);
    void setProperties(float mass, float friction, float restitution);

		void SetGravity (bool gravity);
		void Rotate (float yaw, float pitch, float roll);
		void ClearForces ();

protected:
    glm::vec3 btToGlm(btVector3);
    glm::mat4 btToGlm(btTransform);
    btVector3 glmToBt(glm::vec3);

private:
		bool gravity;

    btRigidBody *physics;
    btCollisionShape *shape;
    btTransform transform;
    vector<btVector3> physMesh;
    float mass;
    float friction;
    float restitution;
    bool isStatic;
};



#endif /* PHYSOBJECT_H_ */
