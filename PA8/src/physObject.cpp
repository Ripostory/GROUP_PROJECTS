#include "physObject.h"

PhysObject::PhysObject()
{
	physics = NULL;
	mass = 0.0f;
	inertia = 0.0f;

	//TODO remove temp
	initPhyiscs();
}

PhysObject::~PhysObject()
{
	  listener.getWorld()->removeCollisionObject(physics);
	  delete physics;
}

void PhysObject::Update(unsigned int dt)
{
	  //update physics object
	  if (physics != NULL)
		  physics->getMotionState()->getWorldTransform(transform);
	  model = glm::translate(glm::mat4(1.0f), glm::vec3(transform.getOrigin().x(), transform.getOrigin().y(), transform.getOrigin().z()));
	  model = glm::rotate(model, transform.getRotation().getX(), glm::vec3(1,0,0));
	  model = glm::rotate(model, transform.getRotation().getY(), glm::vec3(0,1,0));
	  model = glm::rotate(model, transform.getRotation().getZ(), glm::vec3(0,0,1));
	  //update children
	  for (int i = 0; i < children.size(); i++)
	  {
		  children[i]->setMultiplier(multiplier);
		  children[i]->Update(dt);
	  }
}

void PhysObject::initPhyiscs()
{
	btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);
	btDefaultMotionState* groundMotionState =
	                new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));
	btRigidBody::btRigidBodyConstructionInfo
	                groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
	groundRigidBodyCI.m_restitution = 0.5f;
	btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
	listener.getWorld()->addRigidBody(groundRigidBody);


	transform.setIdentity();
	transform.setOrigin(btVector3(0, 25, 0));
	mass = 1.0f;
	btVector3 inertia(0,0,0);
	btCollisionShape* shape = new btSphereShape(1);
	shape->calculateLocalInertia(mass, inertia);

	btDefaultMotionState* objMotionState =
	                new btDefaultMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo
	                objCI(mass, objMotionState, shape, inertia);
	objCI.m_restitution = 0.8f;

	physics = new btRigidBody(objCI);
	listener.getWorld()->addRigidBody(physics);

}
