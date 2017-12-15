#include "gun.h"

Gun::Gun()
{
	Cam = NULL;
	//add barrel children
	barrel1 = new Object();
	initBarrel(barrel1);
	barrel1->translate(glm::vec3(0,-1,0));

	barrel2 = new Object();
	initBarrel(barrel2);
	barrel2->translate(glm::vec3(0,-1,0));
}

Gun::Gun(camera *cam)
{
	Cam = cam;
	loadModel("gun_invNorm.obj");
	loadNormal("cleanNormal.png");
	loadTexture("s_earth.jpg", 2);

	//add barrel children
	barrel1 = new Object();
	initBarrel(barrel1);
	barrel1->translate(glm::vec3(0,-1,0));

	barrel2 = new Object();
	initBarrel(barrel2);
	barrel2->translate(glm::vec3(0,-1,0));
}

Gun::~Gun()
{
	delete barrel1;
	delete barrel2;
}

void Gun::Update(unsigned int dt)
{
	//update kinematic object
	KinematicObject::Update(dt);

	//move to camera
	glm::vec3 base = Cam->GetPosition();
	glm::vec3 offset = (Cam->getLookat()-lastLookat)*0.2f;
	lastLookat += offset;
	translate(glm::vec3(base.x-offset.x,base.y-offset.y,base.z-offset.z));
	mrotate = glm::lookAt(glm::vec3(0), lastLookat, glm::vec3(0,1,0));
	model = mtranslate * glm::inverse(mrotate);

	//clean tracers
	//assume the most recent tracer is the first tracer to despawn
	if (children.size() != 0)
	{
		if (!children.front()->animator.isPending())
		{
			delete *children.begin();
			children.erase(children.begin());
		}
	}

	//update barrels
	barrel1->translate(glm::vec3(0,-1,0));
	barrel2->translate(glm::vec3(0,1,0));
	barrel1->Update(dt);
	barrel2->Update(dt);
}

void Gun::Render()
{
	Object::Render();
	barrel1->Render();
	barrel2->Render();
}

void Gun::keyboard(eventType event)
{
	if (event.eventVer == SDL_KEYDOWN)
	{
		if (event.key == SDLK_a)
			spawnTracer();
	}
}

void Gun::spawnTracer()
{
	
	KinematicObject *tracer = new KinematicObject();
	tracer->loadModel("Plane.obj");
	tracer->loadNormal("cleanNormal.png");
	tracer->translate(glm::vec3(Cam->GetPosition().x-5,Cam->GetPosition().y,Cam->GetPosition().z));
	tracer->lerpTo(lastLookat*5000.0f, 5);
	tracer->rotateTo(glm::vec3(lastLookat.z,lastLookat.y,lastLookat.x), glm::vec3(0,1,0));
	tracer->setCollisionMesh(PHYS_SPHERE, 3);
	tracer->initPhysics();
	addChild(tracer);
}

void Gun::initBarrel(Object* newBarrel)
{
	newBarrel->loadModel("gun_barrel.obj");
	newBarrel->loadNormal("cleanNormal.png");
	newBarrel->loadTexture("s_earth.jpg", 2);
	newBarrel->setParent(this);
}
