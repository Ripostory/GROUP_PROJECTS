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
	  Gun *gun = new Gun(currentCam, muzzleFlash);
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
	  cursor.y = -6;

	  //generate light list
	  Light *planeLights;
	  for (int i = 0; i < 15; i++)
	  {
		  planeLights = new Light();
		  planeLights->translate(glm::vec3(0,-1000,0));
		  addLight(planeLights);
	  }

	  Squadron *wave = new Squadron(lights,1);
	  addChild(wave);

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
	  initPhys();
	  loadModel("cube.obj");
	  loadWorld();
	  loadCubeMap("night", skybox);
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
		if (event.key == SDLK_j)
		{
			//spawn item
			PhysObject *newItem = new PhysObject();
			newItem->loadModel("planet.obj");
			newItem->loadNormal("n_earth.jpg");
			newItem->loadTexture("a_earth.jpg");
			newItem->setCollisionMesh(PHYS_SPHERE, 1);
			newItem->translate(glm::vec3(0,40,0));
			newItem->scale(1);
			newItem->initPhysics();
			this->addChild(newItem);
		}
		if (event.key == SDLK_k)
		{
			//spawn item
			PhysObject *newItem = new PhysObject();
			newItem->loadModel("planet.obj");
			newItem->loadNormal("cleanNormal.png");
			newItem->setCollisionMesh(PHYS_SPHERE, 3);
			newItem->translate(glm::vec3(0,40,0));
			newItem->scale(3);
			newItem->setProperties(3,0.5,0.2);
			newItem->initPhysics();
			this->addChild(newItem);
		}
		if (event.key == SDLK_l)
		{
			//spawn item
			PhysObject *newItem = new PhysObject();
			newItem->loadModel("cube.obj");
			newItem->loadNormal("cleanNormal.png");
			newItem->loadTexture("a_earth.jpg");
			newItem->setCollisionMesh(PHYS_BOX, glm::vec3(1,1,1));
			newItem->translate(glm::vec3(1,40,0));
			newItem->initPhysics();
			this->addChild(newItem);
		}
		if (event.key == SDLK_p)
		{
			//add light
			  Light *light = new Light();
			  light->translate(cursor);
			  light->setSize(10.0f);
			  addLight(light);
		}
		if (event.key == SDLK_UP)
		{
			  cursor.z--;
		}
		if (event.key == SDLK_DOWN)
		{
			  cursor.z++;
		}
		if (event.key == SDLK_LEFT)
		{
			  cursor.x--;
		}
		if (event.key == SDLK_RIGHT)
		{
			  cursor.x++;
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

	  animator.Update(dt);

	  //update children
	  for (int i = 0; i < children.size(); i++)
	  {
		  children[i]->setMultiplier(multiplier);
		  children[i]->Update(dt);
	  }

	  //output world amount
	  ImGui::Text("Object Count: %lu", children.size());
	  ImGui::Text("Cursor Pos: <%.1f, %.1f, %.1f>", cursor.x, cursor.y, cursor.z);

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
