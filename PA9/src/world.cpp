#include "world.h"

GLuint World::lightPosArray = 0;
GLuint World::lightRadArray = 0;
GLuint World::lightSize = 0;
GLuint World::lightTypeArray = 0;
GLuint World::lightRotArray = 0;


World::World()
{
	  //initialize ground plane
	  size = 1.0f;
	  pos = NULL;
		rot = NULL;
		rad = NULL;
		types = NULL;

	  initPhys();

	  //TODO load world here
	  PhysObject *child = new PhysObject();
	  child->loadModel("models/newBoard.obj");
	  child->translate(glm::vec3(0,-10,0));
	  child->rotate(-0.4, glm::vec3(1,0,0));
	  child->setCollisionMesh(PHYS_S_MESH, "models/newBoard.obj");
	  this->addChild(child);

	  Light *light = new Light();
	  light->translate(glm::vec3(10,-5, 0));
	  addLight(light);

	  light = new Light (5, 1, Light_Point);
	  light->translate(glm::vec3(-10, 5,0));
	  addLight(light);

		light = new Light (5, 1, Light_Spot);
		light->translate (glm::vec3(-10, 9, 0));
		spot = light;
		addLight (light);

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

	if (pos != NULL)
		delete[] pos;
}

void World::setLightPointer(GLuint pos, GLuint rad, GLuint siz, GLuint rot, GLuint types)
{
	lightPosArray = pos;
	lightRadArray = rad;
	lightSize = siz;
	lightRotArray = rot;
	lightTypeArray = types;
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


			lastBall = newItem;
			spot -> translate (glm::vec3(0, 0, 0));
		}
		if (event.key == SDLK_l)
		{
			//spawn item
			PhysObject *newItem = new PhysObject();
			newItem->loadModel("models/cube.obj");
			newItem->setCollisionMesh(PHYS_BOX, glm::vec3(1,1,1));
			newItem->translate(glm::vec3(1,40,0));
			newItem->initPhysics();
			this->addChild(newItem);

		}
	}
}

void World::Update(unsigned int dt)
{

		if(lastBall != NULL) {

			spot -> translate (((PhysObject*)lastBall) -> btToGlm(((PhysObject*)lastBall) -> transform.getOrigin()));

		}

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
		  pos[i] = lights[i]->getLight()->pos;
	  }

	  //update children
	  for (int i = 0; i < children.size(); i++)
	  {
		  children[i]->setMultiplier(multiplier);
		  children[i]->Update(dt);
	  }
}

void World::Render()
{
	//ignore rendering self, but pass in light array

	glUniform1i(lightSize, lights.size());
	glUniform3fv(lightPosArray,16,(const float*) pos);
	glUniform3fv(lightRadArray,16,(const float*) rad);
	glUniform3fv(lightRotArray,16,(const float*) rot);
	glUniform1iv(lightTypeArray,16,(const int*) types);
}

void World::addLight(Light *light)
{
	lights.push_back(light);

	//update list
	rebuildDataArray();
}

void World::rebuildDataArray()
{

	//Rebuild positions array
	if (pos != NULL)
		delete[] pos;

	if (lights.size() < 16)
		pos = new glm::vec3[16]();
	else
		pos = new glm::vec3[lights.size()]();

	for (int i = 0; i < lights.size(); i++)
	{
		pos[i] = lights[i]->getLight()->pos;
	}

	//Rebuild rotations array
	if (rot != NULL)
		delete[] rot;

	if (lights.size() < 16)
		rot = new glm::vec3[16]();
	else
		rot = new glm::vec3 [lights.size()]();

	for (int i = 0; i < lights.size(); i++) {

		rot[i] = lights[i] -> getLight () -> rotation;
	}

	if (rad != NULL)
		delete[] rad;

	if (lights.size() < 16)
		rad = new glm::vec3[16] ();
	else
		rad = new glm::vec3 [lights.size()]();

	for (int i = 0; i < lights.size(); i++) {

		rad[i] = lights[i] -> getLight () -> rotation;
	}

	//Rebuild Types array
	if (types != NULL)
		delete types;

	if (lights.size() < 16)
		types = new int [16];
	else
		types = new int[lights.size()];

	for (int i = 0; i < lights.size(); i++) {

		types [i] = lights[i] -> getLight () -> type;
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
