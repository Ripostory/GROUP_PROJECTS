#include "world.h"

GLuint World::lightPosArray = 0;
GLuint World::lightRadArray = 0;
GLuint World::lightColArray = 0;
GLuint World::lightSize = 0;
World* World::m_instance = NULL;

World::World()
{

		World::m_instance = this;

	  //initialize ground plane
	  size = 1.0f;
	  pos = NULL;
	  color = NULL;
	  initPhys();

	  //load board
	  PhysObject *child;
	  child = new PhysObject(Layer_Table, Layer_All ^ Layer_Paddle);
	  child->loadModel("models/boardSet1.obj");
	  child->loadTexture("textures/boardSet1.png");
	  child->setMeshCollider(Physics_Mesh_S_Mesh, "models/collision/c_extWall1.obj");
	  this->addChild(child);

	  child = new PhysObject(Layer_Table, Layer_All ^ Layer_Paddle);
	  child->loadModel("models/boardSet2.obj");
	  child->loadTexture("textures/boardSet2.png");
	  child->setMeshCollider(Physics_Mesh_S_Mesh, "models/collision/c_extWall2.obj");
	  this->addChild(child);

	  child = new PhysObject(Layer_Table, Layer_All ^ Layer_Paddle);
	  child->loadModel("models/backDivider.obj");
	  child->loadTexture("textures/intWall1.png");
	  child->setMeshCollider(Physics_Mesh_S_Mesh, "models/collision/c_divider2.obj");
	  this->addChild(child);

		child = new Bumper (glm::vec3 (0, 0, 0), glm::vec3 (0.5, 0.25, 0.25), BumperTriangle);
		this -> addChild (child);

	  child = new PhysObject(Layer_Table, Layer_All ^ Layer_Paddle);
	  child->loadModel("models/intWall1.obj");
	  child->loadTexture("textures/intWall1.png");
	  child->setMeshCollider(Physics_Mesh_S_Mesh, "models/collision/c_intWalls.obj");
	  this->addChild(child);

	  child = new PhysObject(Layer_Table, Layer_All ^ Layer_Paddle);
	  child->setRenderable(false);
	  child->setMeshCollider(Physics_Mesh_S_Mesh, "models/board.obj");
	  child->translate(glm::vec3(0,4,0));
	  this->addChild(child);

	  Object *base = new Object();
	  base->loadModel("models/intwall2.obj");
	  base->loadTexture("textures/intWall2.png");
	  this->addChild(base);

		child = new Bumper (glm::vec3 (-27, 0, 0), glm::vec3 (0.5, 0.25, 0.25), BumperCylinder);
		this -> addChild (child);

		child = new Bumper (glm::vec3 (-43, 0, 4), glm::vec3 (0.25, 0.25, 0.5), BumperCylinder);
		this -> addChild (child);

		child = new Bumper (glm::vec3 (-39, 0, -4), glm::vec3 (0.25, 0.5, 0.25), BumperCylinder);
		this -> addChild (child);

	  child = new Paddle(pleft, 'g');
	  this->addChild(child);

	  child = new Paddle(pright, 'h');
	  this->addChild(child);
	
		child = new Plunger ('b');
		this -> addChild (child);

	  Light *light = new Light();
	  light->translate(glm::vec3(10,10, 0));
	  light->setColor(glm::vec3(1,0.6,0.7));
	  addLight(light);

	  light = new Light();
	  light->translate(glm::vec3(-80, 10,0));
	  light->setColor(glm::vec3(0.7,1,1));
	  addLight(light);

	  light = new Light();
	  light->translate(glm::vec3(-50,10,-20));
	  light->setColor(glm::vec3(0.5,1,1));
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
	if (color != NULL)
		delete[] color;
}

void World::setLightPointer(GLuint pos, GLuint rad, GLuint siz, GLuint col)
{
	lightPosArray = pos;
	lightRadArray = rad;
	lightColArray = col;
	lightSize = siz;
}

void World::keyboard(eventType event)
{
	if (event.eventVer == SDL_KEYDOWN)
	{
		if (event.key == SDLK_k) {

			if (GUI::getInstance () -> GetLives () > -100) {

				//spawn item
				PhysObject *newItem = new Ball (glm::vec3(-10, 1.1, -20.9));
				this->addChild(newItem);
		
				GUI::getInstance () -> ChangeLives (-1);

	
			}
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
	glUniform3fv(lightColArray,16,(const float*) color);
}

void World::addLight(Light *light)
{
	lights.push_back(light);

	//update list
	rebuildDataArray();
}

void World::removeLight (Light* value) {
/*
	std::vector<int>::iterator position = std::find(myVector.begin(), myVector.end(), 8);
if (position != myVector.end()) // == myVector.end() means the element was not found
    myVector.erase(position);*/

	std::vector<Light*>::iterator it = std::find (lights.begin (), lights.end (), value);
	if (it != lights.end())
		lights.erase (it);


	rebuildDataArray ();
}

void World::rebuildDataArray()
{
	if (pos != NULL)
		delete[] pos;
	if (color != NULL)
		delete[] color;

	if (lights.size() < 16)
	{
		pos = new glm::vec3[16]();
		color = new glm::vec3[16]();
	}
	else
	{
		pos = new glm::vec3[lights.size()]();
		color = new glm::vec3[lights.size()]();
	}

	for (int i = 0; i < lights.size(); i++)
	{
		pos[i] = lights[i]->getLight()->pos;
		color[i] = lights[i]->getLight()->color;
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

World* World::GetInstance () {

	return m_instance;
}
