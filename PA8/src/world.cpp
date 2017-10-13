#include "world.h"

World::World()
{
	  //initialize ground plane
	  initPhys();

	  PhysObject *child = new PhysObject();
	  child->loadModel("models/planet.obj");
	  child->translate(glm::vec3(0,15,1));
	  this->addChild(child);
	  child = new PhysObject();
	  child->loadModel("models/cube.obj");
	  child->setCollisionMesh(PHYS_BOX, glm::vec3(1,1,1));
	  child->translate(glm::vec3(1,13,0));
	  this->addChild(child);
	  child = new PhysObject();
	  child->loadModel("models/planet.obj");
	  //child->setCollisionMesh(PHYS_CAPSULE, 1, 4);
	  child->translate(glm::vec3(0,9,0));
	  this->addChild(child);
	  child = new PhysObject();
	  child->loadModel("models/board.obj");
	  child->translate(glm::vec3(0,5,0));
	  child->setCollisionMesh(PHYS_HULL, "models/board.obj");
	  this->addChild(child);
}

World::~World()
{
	listener.getWorld()->removeCollisionObject(planeCollider);
}

void World::keyboard(eventType event)
{
	if (event.eventVer == SDL_KEYDOWN)
	{
		if (event.key == SDLK_k)
		{
			//spawn item
			PhysObject *newItem = new PhysObject();
			newItem->loadModel("models/planet.obj");
			newItem->setCollisionMesh(PHYS_SPHERE, 1);
			newItem->translate(glm::vec3(0,6,0));
			newItem->initPhyiscs();
			this->addChild(newItem);
		}
		if (event.key == SDLK_l)
		{
			//spawn item
			PhysObject *newItem = new PhysObject();
			newItem->loadModel("models/cube.obj");
			newItem->setCollisionMesh(PHYS_BOX, glm::vec3(1,1,1));
			newItem->translate(glm::vec3(0,6,0));
			newItem->initPhyiscs();
			this->addChild(newItem);
		}
	}
}

void World::initPhys()
{
	btCollisionShape* groundShape =
			new btStaticPlaneShape(btVector3(0, 1, 0), -3);
	btDefaultMotionState* groundMotionState =
	        new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));
	btRigidBody::btRigidBodyConstructionInfo
	        groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
	groundRigidBodyCI.m_restitution = 0.5f;

	//add collider
	planeCollider = new btRigidBody(groundRigidBodyCI);
	listener.getWorld()->addRigidBody(planeCollider);
}
