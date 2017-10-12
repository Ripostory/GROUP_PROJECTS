#include "world.h"

World::World()
{
	  PhysObject *child = new PhysObject();
	  child->loadTexture("textures/ERROR_TEXTURE.jpg");
	  child->loadNormal("textures/ERROR_TEXTURE.jpg");
	  child->loadModel("models/planet.obj");
	  child->translate(glm::vec3(0,0,5));
	  this->addChild(child);
	  child = new PhysObject();
	  child->loadTexture("textures/ERROR_TEXTURE.jpg");
	  child->loadNormal("textures/ERROR_TEXTURE.jpg");
	  child->loadModel("models/planet.obj");
	  child->translate(glm::vec3(3,0,0));
	  this->addChild(child);
	  child = new PhysObject();
	  child->loadTexture("textures/ERROR_TEXTURE.jpg");
	  child->loadNormal("textures/ERROR_TEXTURE.jpg");
	  child->loadModel("models/planet.obj");
	  child->translate(glm::vec3(0,3,0));
	  this->addChild(child);
}

World::~World()
{

}


