#include "world.h"

World::World()
{
	  PhysObject *child = new PhysObject();
	  child->loadNewTexture("textures/ERROR_TEXTURE.jpg");
	  child->loadNewNormal("textures/ERROR_TEXTURE.jpg");
	  child->loadNewModel("models/planet.obj");
	  child->translate(glm::vec3(0,0,5));
	  this->addChild(child);
	  child = new PhysObject();
	  child->loadNewTexture("textures/ERROR_TEXTURE.jpg");
	  child->loadNewNormal("textures/ERROR_TEXTURE.jpg");
	  child->loadNewModel("models/planet.obj");
	  child->translate(glm::vec3(3,0,0));
	  this->addChild(child);
	  child = new PhysObject();
	  child->loadNewTexture("textures/ERROR_TEXTURE.jpg");
	  child->loadNewNormal("textures/ERROR_TEXTURE.jpg");
	  child->loadNewModel("models/planet.obj");
	  child->translate(glm::vec3(0,3,0));
	  this->addChild(child);
}

World::~World()
{

}


