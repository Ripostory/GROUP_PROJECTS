#include "world.h"

World::World()
{
	  //initialize ground plane
	  initPhys();

	  PhysObject *child = new PhysObject();
	  child->loadTexture("textures/ERROR_TEXTURE.jpg");
	  child->loadNormal("textures/ERROR_TEXTURE.jpg");
	  child->loadModel("models/planet.obj");
	  child->translate(glm::vec3(0,15,1));
	  this->addChild(child);
	  child = new PhysObject();
	  child->loadTexture("textures/ERROR_TEXTURE.jpg");
	  child->loadNormal("textures/ERROR_TEXTURE.jpg");
	  child->loadModel("models/planet.obj");
	  child->translate(glm::vec3(1,13,0));
	  this->addChild(child);
	  child = new PhysObject();
	  child->loadTexture("textures/ERROR_TEXTURE.jpg");
	  child->loadNormal("textures/ERROR_TEXTURE.jpg");
	  child->loadModel("models/planet.obj");
	  child->translate(glm::vec3(0,9,0));
	  this->addChild(child);
	  child = new PhysObject();
	  child->loadTexture("textures/ERROR_TEXTURE.jpg");
	  child->loadNormal("textures/ERROR_TEXTURE.jpg");
	  child->loadModel("models/board.obj");
	  child->translate(glm::vec3(0,5,0));
	  child->setCollisionMesh("models/board.obj");
	  this->addChild(child);
}

World::~World()
{
	listener.getWorld()->removeCollisionObject(planeCollider);
}

void World::initPhys()
{
	btCollisionShape* groundShape =
			new btStaticPlaneShape(btVector3(0, 1, 0), 1);
	btDefaultMotionState* groundMotionState =
	        new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));
	btRigidBody::btRigidBodyConstructionInfo
	        groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
	groundRigidBodyCI.m_restitution = 0.5f;

	//add collider
	planeCollider = new btRigidBody(groundRigidBodyCI);
	listener.getWorld()->addRigidBody(planeCollider);
}
