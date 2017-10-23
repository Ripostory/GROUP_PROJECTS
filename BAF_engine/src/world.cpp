#include "world.h"

GLuint World::lightPosArray = 0;
GLuint World::lightRadArray = 0;
GLuint World::lightSize = 0;

World::World()
{
	  //initialize ground plane
	  size = 1.0f;
	  initPhys();

	  //TODO load world here
	  PhysObject *child = new PhysObject();
	  child->loadModel("models/newBoard.obj");
	  child->translate(glm::vec3(0,-10,0));
	  child->rotate(0, glm::vec3(1,0,0));
	  child->setCollisionMesh(PHYS_S_MESH, "models/newBoard.obj");
	  this->addChild(child);

	  Light *light = new Light();
	  light->translate(glm::vec3(20 ,0, 0));
	  light->setSize(80.0f);
	  addLight(light);
	  light = new Light();
	  light->translate(glm::vec3(-20,-5,0));
	  light->setSize(20.0f);
	  addLight(light);
	  light = new Light();
	  light->translate(glm::vec3(0,-5,20));
	  light->setSize(20.0f);
	  addLight(light);
}

World::~World()
{
	listener.getWorld()->removeCollisionObject(planeCollider);

	//clear lights
	for(int i = 0; i < lights.size(); i++)
	{
		delete lights[i];
	}

	lights.clear();
}

void World::setLightPointer(GLuint pos, GLuint rad, GLuint siz)
{
	lightPosArray = pos;
	lightRadArray = rad;
	lightSize = siz;
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
			newItem->translate(glm::vec3(0,40,0));
			newItem->initPhysics();
			this->addChild(newItem);
		}
		if (event.key == SDLK_l)
		{
			//spawn item
			PhysObject *newItem = new PhysObject();
			newItem->loadModel("models/cube.obj");
			newItem->loadTexture("textures/a_earth.jpg");
			newItem->setCollisionMesh(PHYS_BOX, glm::vec3(1,1,1));
			newItem->translate(glm::vec3(1,40,0));
			newItem->initPhysics();
			this->addChild(newItem);
		}
		if (event.key == SDLK_p)
		{
			//add light
			  Light *light = new Light();
			  light->translate(glm::vec3(0 ,0, 0));
			  light->setSize(20.0f);
			  addLight(light);
		}
	}
}

void World::Update(unsigned int dt)
{
	  model = mtranslate * mscale * mrotate;
	  //update keyboard
	  for (int i = 0; i < listener.getSize(); i++)
	  {
		  keyboard(listener.getEvent(i));
	  }

	  //update lights
	  for (int i = 0; i < lights.size(); i++)
	  {

		  lights[i]->Update(dt);
	  }

	  //update children
	  for (int i = 0; i < children.size(); i++)
	  {
		  children[i]->setMultiplier(multiplier);
		  children[i]->Update(dt);
	  }

	  //output world amount
	  ImGui::Text("Object Count: %lu", children.size());
}

void World::Render()
{
	//ignore rendering self, but pass in light array
}

void World::addLight(Light *light)
{
	lights.push_back(light);
}

LightData* World::getLightData(int index)
{
	return lights[index]->getLight();
}

int World::getLightCount()
{
	return lights.size();
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
