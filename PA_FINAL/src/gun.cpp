#include "gun.h"

Gun::Gun()
{
	Cam = NULL;
}

Gun::Gun(camera *cam)
{
	Cam = cam;
	loadModel("newBoard.obj");
	loadNormal("cleanNormal.png");

	spawnTracer();
}

Gun::~Gun()
{

}

void Gun::Update(unsigned int dt)
{
	//move to camera
	translate(Cam->GetPosition());
	lastLookat += (Cam->getLookat()-lastLookat)*0.2f;
	mrotate = glm::lookAt(glm::vec3(0), lastLookat, glm::vec3(0,1,0));
	model = mtranslate * glm::inverse(mrotate);

	//get current model quat
	KinematicObject::Update(dt);
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
	tracer->loadModel("planet.obj");
	tracer->loadNormal("cleanNormal.png");
	tracer->translate(glm::vec3(Cam->GetPosition().x-5,Cam->GetPosition().y,Cam->GetPosition().z));
	tracer->lerpTo(lastLookat*1000.0f, 5);
	tracer->setCollisionMesh(PHYS_SPHERE, 3);
	tracer->initPhysics();
	addChild(tracer);
}
