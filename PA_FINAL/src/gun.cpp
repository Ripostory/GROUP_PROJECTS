#include "gun.h"

Gun::Gun()
{
	Cam = NULL;
	muzzle = NULL;
	barrel1 = NULL;
	barrel2 = NULL;
	activeBarrel = true;
	barrel1z = 0;
	barrel2z = 0;
	keyBind = SDLK_SPACE;
	firing = false;
	recoil = 0;
}

Gun::Gun(camera *cam, Light *flash)
{
	Cam = cam;
	loadModel("gun_invNorm.obj");
	loadNormal("cleanNormal.png");
	loadTexture("s_earth.jpg", 2);
	this->setCollisionMesh(PHYS_SPHERE,0.001);

	//add barrel children
	barrel1 = new Object();
	initBarrel(barrel1);
	barrel1->translate(glm::vec3(0.7,0.25,0));
	addChild(barrel1);

	barrel2 = new Object();
	initBarrel(barrel2);
	barrel2->translate(glm::vec3(0.7,-0.2,0));
	addChild(barrel2);

	muzzle = flash;
	muzzle->setParent(this);
	muzzle->setSize(20.0);
	muzzle->translate(glm::vec3(0.5,0,-6));
	muzzle->setColor(glm::vec3(0));
	activeBarrel = true;
	firing = false;
	keyBind = SDLK_SPACE;

	sample = new Sound ();
	sample -> LoadAudio ("assets/sounds/Sample.wav", 10);
}

Gun::~Gun()
{
	delete muzzle;
}

void Gun::Update(unsigned int dt)
{
	//update kinematic object
	KinematicObject::Update(dt);

	//move to camera
	glm::vec3 base = Cam->GetPosition();
	glm::vec3 offset = (Cam->getLookat()-lastLookat)*0.2f;
	lastLookat += offset;
	glm::vec3 barrelOffset = Cam->getLookat() * -recoil;

	translate(base-offset + barrelOffset);
	rotateTo(lastLookat, glm::vec3(0,1,0));

	barrel1->rotate(barrel1z, glm::vec3(1,0,0));
	barrel2->rotate(barrel2z, glm::vec3(1,0,0));

	//fire tracers
	if (firing)
	{
		if (!animator.isPending(100))
		{

			sample -> PlayAudio ();

			spawnTracer();
			animator.timer(0.15, 100);
			PhysObject* hitObj = Raycast(glmToBt(base), glmToBt(base+lastLookat*10000.0f), true);
			if (hitObj != NULL)
			{
				//plane hit
				hitObj->OnRaycastHit();
			}
		}
	}
	else sample -> KillAudio ();

	//clean tracers
	//assume the most recent tracer is the first tracer to despawn
	if (children.size() != 2)
	{
		if (!children[2]->animator.isPending())
		{
			vector<Object*>::iterator it = children.begin();
			delete *(it+2);
			children.erase(it+2);
		}
	}
}

void Gun::Render()
{
	Object::Render();
}

void Gun::keyboard(eventType event)
{
	if (event.eventVer == SDL_KEYDOWN)
	{
		if (event.key == keyBind)
			firing = true;
	}
	else if (event.eventVer == SDL_KEYUP)
	{
		if (event.key == keyBind)
			firing = false;
	}
}

void Gun::spawnTracer()
{
	
	Object *tracer = new Object();
	glm::vec3 spawnPos = Cam->GetPosition() + lastLookat*25.0f;
	tracer->loadModelFB("planet.obj");
	tracer->loadNormal("cleanNormal.png");
	tracer->translate(spawnPos);
	tracer->lerpTo(lastLookat*5000.0f, 5);
	tracer->rotateTo(-lastLookat, glm::vec3(0,1,0));
	//tracer->setCollisionMesh(PHYS_SPHERE, 3);
	//tracer->initPhysics();
	addChild(tracer);

	//muzzle flash
	muzzle->animator.interrupt(20);
	muzzle->animator.interrupt(10);
	muzzle->animator.interrupt(11);
	muzzle->animator.interrupt(12);
	muzzle->setColor(glm::vec3(10*4,8*4,3*4));
	muzzle->changeColor(glm::vec3(1,0.2,0), 0.1, linear);
	muzzle->changeColor(glm::vec3(0), 0.1, linear);
	recoil = 0.1;
	animator.interrupt(200);
	animator.animateFloat(&recoil, 0, 0.4, easeout, 200);
	//set animation barrel
	if (activeBarrel)
	{
		//shoot top
		muzzle->translate(glm::vec3(0.5,1,-6));
		muzzle->lerpTo(glm::vec3(-0.2,1,-6),0.2);

		barrel1->animator.interrupt(12);
		barrel1->lerpZ(0.7, 0.07, easeout);
		barrel1->lerpZ(0.0, 0.7, easeout);

		//animate z barrel animation
		animator.interrupt(20);
		animator.animateFloat(&barrel1z, 1*3.14/180, 0.1, easeout, 20);
		animator.animateFloat(&barrel1z, -1*3.14/180, 0.2, easeinout, 20);
		animator.animateFloat(&barrel1z, 0.5*3.14/180, 0.3, easeinout, 20);
		animator.animateFloat(&barrel1z, 0, 0.8, easeinout, 20);
		activeBarrel = false;
	}
	else
	{
		//shoot bottom
		muzzle->translate(glm::vec3(0.5,-1,-6));
		muzzle->lerpTo(glm::vec3(-0.2,-1,-6),0.2);

		barrel2->animator.interrupt(12);
		barrel2->lerpZ(0.7, 0.07, easeout);
		barrel2->lerpZ(0.0, 0.7, easeout);

		//animate z barrel animation
		animator.interrupt(21);
		animator.animateFloat(&barrel2z, 1*3.14/180, 0.1, easeout, 21);
		animator.animateFloat(&barrel2z, -1*3.14/180, 0.2, easeinout, 21);
		animator.animateFloat(&barrel2z, 0.5*3.14/180, 0.3, easeinout, 21);
		animator.animateFloat(&barrel2z, 0, 0.8, easeinout, 21);
		activeBarrel = true;
	}

}

void Gun::initBarrel(Object* newBarrel)
{
	newBarrel->loadModel("gun_barrel.obj");
	newBarrel->loadNormal("cleanNormal.png");
	newBarrel->loadTexture("s_earth.jpg", 2);
	newBarrel->setParent(this);
}
