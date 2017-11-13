#include "world.h"

GLuint World::lightPosArray = 0;
GLuint World::lightRadArray = 0;
GLuint World::lightSize = 0;
World* World::instance = NULL;

World::World()
{

		World::instance = this;

	  //initialize ground plane
	  size = 1.0f;
	  pos = NULL;
	  initPhys();

	  //load board
	  PhysObject *child;
	  child = new PhysObject(Layer_Table, Layer_All);
	  child->loadModel("models/boardSet1.obj");
	  child->loadTexture("textures/boardSet1.png");
	  child->setMeshCollider(Physics_Mesh_S_Mesh, "models/collision/c_extWall1.obj");
	  this->addChild(child);

	  child = new PhysObject(Layer_Table, Layer_All);
	  child->loadModel("models/boardSet2.obj");
	  child->loadTexture("textures/boardSet2.png");
	  child->setMeshCollider(Physics_Mesh_S_Mesh, "models/collision/c_extWall2.obj");
	  this->addChild(child);

	  child = new PhysObject(Layer_Table, Layer_All);
	  child->loadModel("models/backDivider.obj");
	  child->loadTexture("textures/intWall1.png");
	  child->setMeshCollider(Physics_Mesh_S_Mesh, "models/collision/c_divider2.obj");
	  this->addChild(child);

	  child = new PhysObject(Layer_Table, Layer_All);
	  child->loadModel("models/bumper.obj");
	  child->loadTexture("textures/bumper.png");
	  child->setMeshCollider(Physics_Mesh_S_Mesh, "models/collision/c_bumpers.obj");
	  child->setProperties(0.0,0.5,3.0);
	  this->addChild(child);

	  child = new PhysObject(Layer_Table, Layer_All);
	  child->loadModel("models/intWall1.obj");
	  child->loadTexture("textures/intWall1.png");
	  child->setMeshCollider(Physics_Mesh_S_Mesh, "models/collision/c_intWalls.obj");
	  this->addChild(child);

	  Object *base = new Object();
	  base->loadModel("models/intwall2.obj");
	  base->loadTexture("textures/intWall2.png");
	  this->addChild(base);

	  //TODO move bumpers to their own class
	  child = new PhysObject(Layer_Table, Layer_All);
	  child->loadModel("models/cylBumper.obj");
	  child->loadTexture("textures/cylBumper.png");
	  child->setBoxCollider(Physics_Mesh_Cylinder, glm::vec3(1.5, 2, 1.5));
	  child->setProperties(0,0,2);
	  child->translate(glm::vec3(-27,0,0));
	  this->addChild(child);

	  child = new PhysObject(Layer_Table, Layer_All);
	  child->loadModel("models/cylBumper.obj");
	  child->loadTexture("textures/cylBumper.png");
	  child->setBoxCollider(Physics_Mesh_Cylinder, glm::vec3(1.5, 2, 1.5));
	  child->setProperties(0,0,2);
	  child->translate(glm::vec3(-43,0,4));
	  this->addChild(child);

	  child = new PhysObject(Layer_Table, Layer_All);
	  child->loadModel("models/cylBumper.obj");
	  child->loadTexture("textures/cylBumper.png");
	  child->setBoxCollider(Physics_Mesh_Cylinder, glm::vec3(1.5, 2, 1.5));
	  child->setProperties(0,0,2);
	  child->translate(glm::vec3(-39,0,-4));
	  this->addChild(child);



		child = new PhysObject(Layer_All, Layer_All);
		child -> loadModel ("models/planet.obj");
		child -> setMeshCollider (Physics_Mesh_Hull, "models/planet.obj");
		child -> translate (glm::vec3(-17.0f, 0.0f, -21.46273f));
		child -> setProperties (0.3f, 0.0f, 0.05f);
		this -> addChild (child);

	  child = new Paddle(pleft, 'g');
	  this->addChild(child);
	  child = new Paddle(pright, 'h');
	  this->addChild(child);
	
		child = new Plunger ('b');
		this -> addChild (child);

	  Light *light = new Light();
	  light->translate(glm::vec3(10,10, 0));
	  addLight(light);
	  light = new Light();
	  light->translate(glm::vec3(-80, 10,0));
	  addLight(light);
	  light = new Light();
	  light->translate(glm::vec3(-50,10,-20));
	  addLight(light);

	  //load visual objects
	  Object *models = new Object();
	  models->loadModel("models/board.obj");
	  models->loadTexture("textures/board.png");
	  this->addChild(models);
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
		if (event.key == SDLK_k) {

			//spawn item
			PhysObject *newItem = new PhysObject(Layer_All, Layer_All);
			newItem->loadModel("models/planet.obj");
			newItem->setSphereCollider(Physics_Mesh_Sphere, 1);
			newItem->translate(glm::vec3(-80,40,0));
			newItem->setProperties(0.1, 0.5, 0.5);
			newItem->initPhysics();
			this->addChild(newItem);
		}
		if (event.key == SDLK_l) {

			//spawn item
			PhysObject *newItem = new PhysObject(Layer_Table, Layer_Table);
			newItem->loadModel("models/cube.obj");
			newItem->setBoxCollider(Physics_Mesh_Box, glm::vec3(1,1,1));
			newItem->translate(glm::vec3(1,40,0));
			newItem->initPhysics();
			this->addChild(newItem);
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
}

void World::addLight(Light *light)
{
	lights.push_back(light);

	//update list
	rebuildDataArray();
}

void World::rebuildDataArray()
{
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
