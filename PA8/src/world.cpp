#include "world.h"

World::World()
{
	  //initialize ground plane
	  size = 1.0f;
	  initPhys();

	  PhysObject *child = new PhysObject();
	  child->loadModel("models/planet.obj");
	  child->translate(glm::vec3(0,17,1));
	  this->addChild(child);

	  child = new PhysObject();
	  child->loadModel("models/cube.obj");
	  child->setCollisionMesh(PHYS_BOX, glm::vec3(1,1,1));
	  child->translate(glm::vec3(3,15,0));
	  this->addChild(child);

	  child = new PhysObject();
	  child->loadModel("models/planet.obj");
	  child->translate(glm::vec3(0,12,0));
	  this->addChild(child);

	  child = new PhysObject();
	  child->loadModel("models/newBoard.obj");
	  child->translate(glm::vec3(0,-12,0));
	  child->rotate(-0.4, glm::vec3(1,0,0));
	  child->setCollisionMesh(PHYS_S_MESH, "models/newBoard.obj");
	  this->addChild(child);

	  child  = new PhysObject();
	  child->setCollisionMesh(PHYS_CYLINDER, glm::vec3(3,3,3));
	  child->loadModel("models/cylinder.obj");
	  child->setProperties(0,1,1);
	  child->rotate(360, glm::vec3(1,0,0));
	  child->rotate(360,glm::vec3(0,0,1));
	  child->translate(glm::vec3(0,9,0));
	  child->scale(3);
	  this->addChild(child);

		child = new PhysObject ();
		child -> setCollisionMesh (PHYS_HULL, "models/paddle.obj");
		child -> loadModel ("models/paddle.obj");
		child -> rotate (-0.4, glm::vec3 (1,0,0));
		child -> rotate (-0.6, glm::vec3 (0,1,0));
		child -> setProperties (20, 0, 0);
		child -> SetGravity (false);
		paddle = child;
		this -> addChild (child);


}

World::~World()
{
	listener.getWorld()->removeCollisionObject(planeCollider);
}

void World::keyboard(eventType event)
{

	static bool x_pressed;
	static bool z_pressed;

	if (event.eventVer == SDL_KEYDOWN)
	{
		if (event.key == SDLK_k)
		{
			//spawn item
			PhysObject *newItem = new PhysObject();
			newItem->loadModel("models/planet.obj");
			newItem->setCollisionMesh(PHYS_SPHERE, 1);
			newItem->translate(glm::vec3(2,40,1));
			newItem->initPhysics();
			this->addChild(newItem);
		}
		if (event.key == SDLK_l)
		{
			//spawn item
			PhysObject *newItem = new PhysObject();
			newItem->loadModel("models/cube.obj");
			newItem->setCollisionMesh(PHYS_BOX, glm::vec3(1,1,1));
			newItem->translate(glm::vec3(0,40,0));
			newItem->initPhysics();
			this->addChild(newItem);
		}

		if (event.key == SDLK_z) {
			paddle -> Rotate (0, 15, 0);
		}
		if (event.key == SDLK_x) {
			paddle -> Rotate (0, -15, 0);
		}
		if (event.key == SDLK_c)
			paddle -> Rotate (0, 0, 0);
	}
}

void World::initPhys()
{
	btCollisionShape* groundShape =
			new btStaticPlaneShape(btVector3(0, 1, 0), -20);
	btDefaultMotionState* groundMotionState =
	        new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));
	btRigidBody::btRigidBodyConstructionInfo
	        groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
	groundRigidBodyCI.m_restitution = 0.5f;

	//add collider
	planeCollider = new btRigidBody(groundRigidBodyCI);
	listener.getWorld()->addRigidBody(planeCollider);
}
