#include "world.h"

void World::loadWorld()
{
	  //TODO load world here
	  //PhysObject *child = new PhysObject();
	  //child->loadModel("newBoard.obj");
	  //child->loadNormal("cleanNormal.png");
	  //child->translate(glm::vec3(0,0,0));
	  //child->rotate(0, glm::vec3(1,0,0));
	  //child->setCollisionMesh(PHYS_S_MESH, "newBoard.obj");
	  //this->addChild(child);

	  //load Gun and muzzle flash
	  Light *muzzleFlash = new Light();
	  gun = new Gun(currentCam, muzzleFlash);
	  addChild(gun);
	  addLight(muzzleFlash);

	  //load sun/moon
	  Light *sun;
	  sun = new Light(LIGHT_DIR);
	  sun->translate(glm::vec3(1,0.1,-0.05));
	  sun->setSize(50.0f);
	  sun->setColor(glm::vec3(0.6863,0.902,0.9373));
	  addLight(sun);

	  //load local light
	  Light *light;
	  light = new Light();
	  light->translate(glm::vec3(30,60,-40));
	  light->setSize(100.0f);
	  light->setColor(glm::vec3(2.0,2.2,4.2));
	  addLight(light);

	  //generate light list
	  Light *planeLights;
	  for (int i = 0; i < 15; i++)
	  {
		  planeLights = new Light();
		  planeLights->translate(glm::vec3(0,-1000,0));
		  addLight(planeLights);
	  }

	  currentWave = new Squadron(lights,difficulty);
	  addChild(currentWave);

	  //add boat
	  Object *testBoat = new Object();
	  testBoat->loadModel("Ship.obj");
	  testBoat->loadTexture("a_ship.png");
	  testBoat->loadNormal("cleanNormal.png");
	  testBoat->translate(glm::vec3(800,0,0));
	  addChild(testBoat);

	  testBoat = new Object();
	  testBoat->loadModel("Ship.obj");
	  testBoat->loadTexture("a_ship.png");
	  testBoat->loadNormal("cleanNormal.png");
	  testBoat->translate(glm::vec3(200,0,-400));
	  addChild(testBoat);

	  testBoat = new Object();
	  testBoat->loadModel("Ship.obj");
	  testBoat->loadTexture("a_ship.png");
	  testBoat->loadNormal("cleanNormal.png");
	  testBoat->translate(glm::vec3(300,0,400));
	  addChild(testBoat);

	  testBoat = new Object();
	  testBoat->loadModel("Ship.obj");
	  testBoat->loadTexture("a_ship.png");
	  testBoat->loadNormal("cleanNormal.png");
	  testBoat->translate(glm::vec3(750,0,-600));
	  addChild(testBoat);
}

World::World()
{
	  //initialize ground plane
	  size = 1.0f;
	  currentCam = NULL;
	  initPhys();
	  loadModel("cube.obj");
	  loadWorld();
	  loadCubeMap("beach", skybox);
}

World::World(camera *final)
{
	  //initialize ground plane
	  size = 1.0f;
	  currentCam = final;
	  currentWave = NULL;
	  difficulty = 1;
	  initPhys();
	  loadModel("cube.obj");
	  loadWorld();
	  loadCubeMap("night", skybox);
	  deathAnimation = false;
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

void World::keyboard(eventType event)
{
	if (event.eventVer == SDL_KEYDOWN)
	{
		//TODO add any additional binds
		if (event.key == SDLK_p)
		{
			//clean itself from children and spawn new squadron
			vector<Object*>::iterator it;
			for (it = children.begin(); it != children.end(); it++)
			{
				if (*it == currentWave)
				{
					children.erase(it);
					break;
				}
			}

			delete currentWave;
			difficulty++;
			currentWave = new Squadron(lights, difficulty);
			addChild(currentWave);

		}
		if (event.key == SDLK_o)
		{
			gun->kill();
		}
	}
}

void World::Update(unsigned int dt)
{

	  //TODO add gameloop here
	  if (currentWave->isDefeated())
	  {
			//clean itself from children and spawn new squadron
			vector<Object*>::iterator it;
			for (it = children.begin(); it != children.end(); it++)
			{
				if (*it == currentWave)
				{
					children.erase(it);
					break;
				}
			}

			delete currentWave;
			difficulty += 0.8;
			currentWave = new Squadron(lights, difficulty);
			addChild(currentWave);
	  }

	  if (currentWave->isPlaneEscape() && !deathAnimation)
	  {
		  //kill gun
		  //do deathAnimation
		  gun->kill();
		  deathAnimation = true;
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
	  }

	  animator.Update(dt);

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
	//render skybox
	  glBindBuffer(GL_ARRAY_BUFFER, modelData.VB);
	  glEnableVertexAttribArray(0);
	  glEnableVertexAttribArray(1);
	  glEnableVertexAttribArray(2);

	  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,color));
	  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,texCoord));

	  glBindTexture(GL_TEXTURE_2D, skybox);
	  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, modelData.IB);
	  glDrawElements(GL_TRIANGLES, modelData.size, GL_UNSIGNED_INT, 0);

	  glDisableVertexAttribArray(0);
	  glDisableVertexAttribArray(1);
	  glDisableVertexAttribArray(2);
}

void World::addLight(Light *light)
{
	lights.push_back(light);
}

Light* World::getLightData(int index)
{
	return lights[index];
}

int World::getLightCount()
{
	return lights.size();
}

GLuint World::getSkybox()
{
	return skybox;
}

void World::initPhys()
{
	btCollisionShape* groundShape =
			new btStaticPlaneShape(btVector3(0, 1, 0), -999);
	btDefaultMotionState* groundMotionState =
	        new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));
	btRigidBody::btRigidBodyConstructionInfo
	        groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
	groundRigidBodyCI.m_restitution = 0.5f;

	//add collider
	planeCollider = new btRigidBody(groundRigidBodyCI);
	listener.getWorld()->addRigidBody(planeCollider);
}

void World::setCam(camera *incam)
{
	currentCam = incam;
}
